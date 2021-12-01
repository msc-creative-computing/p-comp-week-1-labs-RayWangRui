/******************************************************************************
 * sound_distance.ino
 * Connections:
 * The Sound Detector is connected to the Adrduino as follows:
 * (Sound Detector -> Arduino pin)
 * GND → GND
 * VCC → 5V
 * Gate → Pin 2
 * Envelope → A0
 * 
 ******************************************************************************/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
 
 // Define hardware connections
 
#define PIN_GATE_IN 2
#define IRQ_GATE_IN  0
#define PIN_LED_OUT 13
#define PIN_MOTOR_OUT 6 //
#define PIN_ANALOG_IN A0
int VoiceVal=0;
int VoiceMAX=550;
int NoiseVal=40;

// 控制 WS2812 灯条的引脚编号
#define PIN        5
 
//定义控制的 LED 数量
#define NUMPIXELS 6//16
 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
 
//相邻 LED 之间的延迟，单位毫秒
#define DELAYVAL 500

const int trigPin = 12;
const int echoPin = 15;  //A2

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;
int DistanceVal=0;
int VoiceVal=0;
// soundISR()
// This function is installed as an interrupt service routine for the pin
// change interrupt.  When digital input 2 changes state, this routine
// is called.
// It queries the state of that pin, and sets the onboard LED to reflect that 
// pin's state.
void soundISR()
{
  int pin_val;
  
  pin_val = digitalRead(PIN_GATE_IN);
  digitalWrite(PIN_LED_OUT, pin_val);   
}

void setup()
{
  Serial.begin(9600);
  
  //  Configure LED pin as output
  pinMode(PIN_LED_OUT, OUTPUT);
  pinMode(PIN_MOTOR_OUT, OUTPUT);
  
  // configure input to interrupt
  pinMode(PIN_GATE_IN, INPUT);
  attachInterrupt(IRQ_GATE_IN, soundISR, CHANGE);

  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code. 
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)    
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  
  // Display status
  Serial.println("Initialized");
}

void loop()
{
  int VoiceValue;
  
  // Check the envelope input
  VoiceValue = analogRead(PIN_ANALOG_IN);
  // Convert envelope value into a message
  Serial.println("Voice Value: ");
  Serial.println(String(VoiceValue));
  if(VoiceValue >= VoiceMAX)  {
    VoiceValue = VoiceMAX;
    analogWrite(PIN_MOTOR_OUT, 255);
  }else if(VoiceValue <= NoiseVal){
    VoiceValue = 0;
    analogWrite(PIN_MOTOR_OUT, 0);
  }else {
      VoiceVal = map(VoiceValue, 20,550, 0, 255);  // 
      analogWrite(PIN_MOTOR_OUT, VoiceVal);
  }
  

  /*
  if(value <= 10)
  {
    Serial.println("Quiet.");
  }
  else if( (value > 10) && ( value <= 30) )
  {
    Serial.println("Moderate.");
  }
  else if(value > 30)
  {
    Serial.println("Loud.");
  }
  */


   // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  /*
  if( distanceCm >20)
  {
    distanceCm=20;
  }
  else{    
    DistanceVal = map(distanceCm, 0,20, 0, 255);  // 0~20cm only
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(DistanceVal,0, 0)); 
      pixels.show();   // Send the updated pixel colors to the hardware. 
      delay(DELAYVAL); // Pause before next pass through loop
    }
  }  
 */
    // Convert to inches
    /*
  distanceInch = distanceCm * CM_TO_INCH;
  
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  */
//  pixels.clear(); // Set all pixel colors to 'off'
  

  if(distanceCm >= 400)   
  {
    DistanceVal = 0;  // >4m
    NUMPIXELS=6;
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
      pixels.show();   // Send the updated pixel colors to the hardware. 
      delay(DELAYVAL); // Pause before next pass through loop
    }
  }
  
  else if( (distanceCm > 300) && ( distanceCm <= 400) )
  {
    NUMPIXELS=5;
    DistanceVal = map(distanceCm, 200,300, 0, 255);  // 0~1m;  // 3~4m
    pixels.setPixelColor(0, pixels.Color(DistanceVal,0 , 0)); 
    pixels.show();   // Send the updated pixel colors to the hardware. 
    delay(DELAYVAL); // Pause before next pass through loop
  }
  else if( (distanceCm > 200) && ( distanceCm <=300) )
  {
    NUMPIXELS=4;
    DistanceVal = map(distanceCm, 200,300, 0, 255);  // 0~1m;  // 3~4m
    pixels.setPixelColor(1, pixels.Color(0, DistanceVal, 0)); 
    pixels.show();   // Send the updated pixel colors to the hardware. 
    delay(DELAYVAL); // Pause before next pass through loop
  }
  else if( (distanceCm > 100) && ( distanceCm <=200) )
  {
    NUMPIXELS=3;
    DistanceVal = map(distanceCm, 100,200, 0, 255);  // 0~1m;  // 3~4m
    pixels.setPixelColor(2, pixels.Color(0,0,DistanceVal)); 
    pixels.show();   // Send the updated pixel colors to the hardware. 
    delay(DELAYVAL); // Pause before next pass through loop
  }  
  else if( (distanceCm > 20) && ( distanceCm <=100) )
  {
    NUMPIXELS=2;
    DistanceVal = map(distanceCm, 0,20, 0, 255);  // 0~1m;  // 3~4m
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(DistanceVal,DistanceVal, 0)); 
      pixels.show();   // Send the updated pixel colors to the hardware. 
      delay(DELAYVAL); // Pause before next pass through loop
    }
  }
  else if( (distanceCm > 0) && ( distanceCm <=20) )
  {
    NUMPIXELS=1;
    DistanceVal = map(distanceCm, 0,20, 0, 255);  // 0~1m;  // 3~4m
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(DistanceVal,0, 0)); 
      pixels.show();   // Send the updated pixel colors to the hardware. 
      delay(DELAYVAL); // Pause before next pass through loop
    }
  }  


 /*
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, DistanceVal, 0)); 
    pixels.show();   // Send the updated pixel colors to the hardware. 
    delay(DELAYVAL); // Pause before next pass through loop
  }
  */  
  // pause for 1 second
  delay(1000);
}

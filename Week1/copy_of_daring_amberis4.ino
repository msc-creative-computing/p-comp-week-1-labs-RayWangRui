int switchState = 0;
void setup(){
 pinMode(10,OUTPUT);
 pinMode(9,OUTPUT);
 pinMode(8,OUTPUT);
 pinMode(7,OUTPUT);
 pinMode(6,OUTPUT);
 pinMode(3,OUTPUT);
 pinMode(4,OUTPUT);
 pinMode(5,OUTPUT);
 pinMode(2,INPUT);
 pinMode(11,INPUT);
 pinMode(12,INPUT);
}
void loop(){
 switchState = digitalRead(2);
  if (switchState == LOW)
 switchState = digitalRead(11);
  if (switchState == LOW)
 switchState = digitalRead(12);
  if (switchState == LOW) { 
 // the buton is not pressed
digitalWrite(3, HIGH); // green LED
digitalWrite(4, LOW); // red LED
digitalWrite(5, LOW); // red LED
digitalWrite(6, HIGH); // green LED
digitalWrite(7, HIGH); // red LED
digitalWrite(8, LOW); // red LED
digitalWrite(9, HIGH); // red LED
digitalWrite(10, LOW);
 } 
else { // the buton is pressed
digitalWrite(3, LOW); 
digitalWrite(4, LOW);
digitalWrite(5, HIGH);
digitalWrite(6, HIGH);
digitalWrite(7, LOW); 
digitalWrite(8, HIGH); 
digitalWrite(9, LOW); 
digitalWrite(10, HIGH);
delay(250); // wait for a quarter second
 // toggle the LEDs
digitalWrite(4, HIGH); 
digitalWrite(5, LOW);
digitalWrite(7, HIGH); 
digitalWrite(8, LOW); 
digitalWrite(9,HIGH); 
delay(250); // wait for a quarter second
 } 
} // go back to the beginning of the loop
 // this is a comment
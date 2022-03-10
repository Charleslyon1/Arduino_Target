/*
 Controlling a servo position using a potentiometer 
 (variable resistor)
*/
 
#include <Servo.h>
 
Servo myservo;  // create servo object to control a servo
 
int potpin = 0;  // analog pin used to connect the potentiometer
int buttonpin = 7; // analog pin used to connect the button
int val;    // variable to read the value from the analog pin

int buttonState= 0; // variable for reading pushbutton status
 
void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(buttonpin, INPUT);  //initializes pushbutton pin as input
}
 
void loop() {
  val = analogRead(potpin);        // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 180); // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val);              // sets the servo position according to the scaled value
  delay(15);                       // waits for the servo to get there

  buttonState = digitalRead(buttonpin);
  
  if(buttonState==HIGH){
      delay(300);
      myservo.write(180);
      delay(600);
      myservo.write(0);
      delay(300);
    }
}

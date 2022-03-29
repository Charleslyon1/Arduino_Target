/* Example code to use Force Sensitive Resistor (FSR) as toggle switch to control LED. More info: https://www.makerguides.com */
#include <Servo.h>
#include <SoftwareSerial.h>
Servo myservo;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
Servo myservo3;
Servo myservo4;

// Define pins:
#define fsrpin1 A0
#define fsrpin2 A1
#define fsrpin3 A2
#define fsrpin4 A3


// Define variables:
int fsrReading; // The current reading from the FSR
int fsrReading2; // The current reading from the FSR
int fsrReading3; // The current reading from the FSR
int fsrReading4; // The current reading from the FSR

int state = LOW; // The current state of the output pin
int previous = 0; // The previous reading from the FSR
int state2 = LOW;
int previous2 = 0;
int state3 = LOW;
int previous3 = 0;
int state4 = LOW;
int previous4 = 0;

bool isDropped1;
bool isDropped2;
bool isDropped3;
bool isDropped4;

//===============================
bool lightIsOn = true;
SoftwareSerial Bluetooth(7, 8);


// The follow variables are long's because the time, measured in miliseconds, will quickly become a bigger number than can be stored in an int:
long time = 0;  // The last time the output pin was toggled
long debounce = 40; // The debounce time, increase if the output flickers
void setup() {
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(9600);
  // Set ledpin as output:
 

  //--------------------------------------
  myservo.attach(13);  // attaches the servo on pin 11 to the servo object
  myservo2.attach(12);
  myservo3.attach(11);
  myservo4.attach(10);
  //=============
  Bluetooth.begin(9600);
}


void loop() {
  // Read the FSR pin and store the output as fsrreading:
  fsrReading = analogRead(fsrpin1);
  fsrReading2 = analogRead(fsrpin2);
  fsrReading3 = analogRead(fsrpin3);
  fsrReading4 = analogRead(fsrpin4);
  delay(100);
  // Print the fsrreading in the serial monitor:
//   Serial.print("sensor1: "); 
//  Serial.println(fsrReading);
//     Serial.print("sensor2: "); 
//  Serial.println(fsrReading2);
//       Serial.print("sensor3: "); 
//  Serial.println(fsrReading3);
//       Serial.print("sensor4: "); 
//  Serial.println(fsrReading4);
  
  // If the input just went from below 500 to above 500 and we've waited long enough to ignore any noise on the circuit, toggle the output pin and remember the time:
  if (fsrReading > 200 && previous < 200 && millis() - time > debounce) {
    isDropped1 = true;
    Serial.print("Shit happened 1");
    time = millis();
  }
  delay(debounce);
  previous = fsrReading;
  
  if (fsrReading2 > 200 && previous2 < 200 && millis() - time > debounce) {
      isDropped2 = true;
      Serial.print("Shit happened 2");
    time = millis();
  }

  delay(debounce);
  previous2 = fsrReading2;
  
    if (fsrReading3 > 200 && previous3 < 200 && millis() - time > debounce) {
     isDropped3 = true;
     Serial.print("Shit happened 3");
    time = millis();
  }
 
  delay(debounce);
  previous3 = fsrReading3;
  
  if (fsrReading4 > 200 && previous4 < 200 && millis() - time > debounce) {
      isDropped4 = true;
      Serial.print("Shit happened 4");
    time = millis();
  }
  //digitalWrite(ledpin4, state4);
  delay(debounce);
  previous4 = fsrReading4;



 //=============================
 int command;
 bool isReset = false;
 
 myservo.write(0);
 myservo2.write(0);
 myservo3.write(0);
 myservo4.write(0);
  
  //if (Bluetooth.available()) {
   // command = Bluetooth.read(); 
    //if (command == 48)
    if(isDropped1 && isDropped2 && isDropped3 && isDropped4){
    {
      isReset == true;
      Serial.println("isReset set to true");
      Serial.println("0 / ON");
        myservo.write(130);
       // Serial.println(myservo.read());
       // Serial.println(myservo2.read());
        lightIsOn = true;
        //delay (1000);
        Serial.println("1 / OFF");
      
      myservo2.write(130);
      myservo3.write(130);
      myservo4.write(130);
      delay (500);
       myservo.write(0);
       //delay (500);
       myservo2.write(0);
       //delay (500);
       
      // delay (500);
       myservo3.write(0);
      // delay (500);
       
       //delay (500);
       myservo4.write(0);
     // Serial.println(myservo.read());
     // Serial.println(myservo2.read());
      lightIsOn = false;
      delay(300);

      isDropped1 = false;
      isDropped2 = false;
      isDropped3 = false;
      isDropped4 = false;
      
    }

//    else if (command == 49)
//    {
//      isReset == false;
//      Serial.println("isReset set to false");
//    }
//
//    switch (isReset) {
//      case true:
//      Serial.println("0 / ON");
//        myservo.write(90);
//        myservo2.write(10);
//       // Serial.println(myservo.read());
//       // Serial.println(myservo2.read());
//        lightIsOn = true;
//        delay (100);
//        break;
//        case false: 
//          // A zero value input will turn off the LED
//      Serial.println("1 / OFF");
//      myservo.write(10);
//      myservo2.write(90);
//     // Serial.println(myservo.read());
//     // Serial.println(myservo2.read());
//      lightIsOn = false;
//      delay (100);  
//      }
    

  }
 
}
//}

#include <SoftwareSerial.h>
int led = 2;
bool lightIsOn = true;
SoftwareSerial Bluetooth(7, 8);
void setup() {  
  Serial.begin(9600);
  Bluetooth.begin(9600);
  pinMode(led, OUTPUT);
}
void loop() { 
  if (lightIsOn == true) {
      digitalWrite(led, HIGH);
  }
  else {
      digitalWrite(led, LOW);

    }
  int command;
  
  if (Bluetooth.available()) {
    command = Bluetooth.read();  
    Serial.println("Input received:");
    Serial.println(command);
    if (command == 48)
    {
      // A non-zero input will turn on the LED
      Serial.println("0 / ON");
//      digitalWrite(led, HIGH);
        lightIsOn = true;
        delay (100);
       //  Serial.println(lightIsOn);
    }
    
    else if (command == 49)
    {
      // A zero value input will turn off the LED
      Serial.println("1 / OFF");
      Serial.println(command);
      digitalWrite(led, LOW);
      lightIsOn = false;
      delay (100);
      // Serial.println(lightIsOn);
    } 
    
   
  }
}

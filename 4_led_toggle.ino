
/* Example code to use Force Sensitive Resistor (FSR) as toggle switch to control LED. More info: https://www.makerguides.com */
// Define pins:
#define fsrpin1 A0
#define fsrpin2 A1
#define fsrpin3 A2
#define fsrpin4 A3

#define ledpin1 2
#define ledpin2 3
#define ledpin3 4
#define ledpin4 5

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

// The follow variables are long's because the time, measured in miliseconds, will quickly become a bigger number than can be stored in an int:
long time = 0;  // The last time the output pin was toggled
long debounce = 40; // The debounce time, increase if the output flickers
void setup() {
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(9600);
  // Set ledpin as output:
  pinMode(ledpin1, OUTPUT);
  pinMode(ledpin2, OUTPUT);
  pinMode(ledpin3, OUTPUT);
  pinMode(ledpin4, OUTPUT);
}
void loop() {
  // Read the FSR pin and store the output as fsrreading:
  fsrReading = analogRead(fsrpin1);
  fsrReading2 = analogRead(fsrpin2);
  fsrReading3 = analogRead(fsrpin3);
  fsrReading4 = analogRead(fsrpin4);
  delay(100);
  // Print the fsrreading in the serial monitor:
   Serial.print("sensor1: "); 
  Serial.println(fsrReading);
     Serial.print("sensor2: "); 
  Serial.println(fsrReading2);
       Serial.print("sensor3: "); 
  Serial.println(fsrReading3);
       Serial.print("sensor4: "); 
  Serial.println(fsrReading4);
  
  // If the input just went from below 500 to above 500 and we've waited long enough to ignore any noise on the circuit, toggle the output pin and remember the time:
  if (fsrReading > 2 && previous < 2 && millis() - time > debounce) {
    if (state == LOW)
      state = HIGH;
    else
      state = LOW;
    time = millis();
  }
  digitalWrite(ledpin1, state);
  delay(debounce);
  previous = fsrReading;
  
  if (fsrReading2 > 2 && previous2 < 2 && millis() - time > debounce) {
    if (state2 == LOW)
      state2 = HIGH;
    else
      state2 = LOW;
    time = millis();
  }
  digitalWrite(ledpin2, state2);
  delay(debounce);
  previous2 = fsrReading2;
  
    if (fsrReading3 > 2 && previous3 < 2 && millis() - time > debounce) {
    if (state3 == LOW)
      state3 = HIGH;
    else
      state3 = LOW;
    time = millis();
  }
  digitalWrite(ledpin3, state3);
  delay(debounce);
  previous3 = fsrReading3;
  
  if (fsrReading4 > 2 && previous4 < 2 && millis() - time > debounce) {
    if (state4 == LOW)
      state4 = HIGH;
    else
      state4 = LOW;
    time = millis();
  }
  digitalWrite(ledpin4, state4);
  delay(debounce);
  previous4 = fsrReading4;
 
}

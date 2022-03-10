/* Example code to use Force Sensitive Resistor (FSR) as toggle switch to control LED. More info: https://www.makerguides.com */
// Define pins:
#define fsrpin A0
#define ledpin 2
// Define variables:
int fsrreading; // The current reading from the FSR
int state = LOW; // The current state of the output pin
int previous = 0; // The previous reading from the FSR
// The follow variables are long's because the time, measured in miliseconds, will quickly become a bigger number than can be stored in an int:
long time = 0;  // The last time the output pin was toggled
long debounce = 40; // The debounce time, increase if the output flickers
void setup() {
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(9600);
  // Set ledpin as output:
  pinMode(ledpin, OUTPUT);
}
void loop() {
  // Read the FSR pin and store the output as fsrreading:
  fsrreading = analogRead(fsrpin);
  // Print the fsrreading in the serial monitor:
  Serial.println(fsrreading);
  // If the input just went from below 500 to above 500 and we've waited long enough to ignore any noise on the circuit, toggle the output pin and remember the time:
  if (fsrreading > 2 && previous < 2 && millis() - time > debounce) {
    if (state == LOW)
      state = HIGH;
    else
      state = LOW;
    time = millis();
  }
  digitalWrite(ledpin, state);
  previous = fsrreading;
}

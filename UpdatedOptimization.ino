/* Example code to use Force Sensitive Resistor (FSR) as toggle switch to control LED. More info: https://www.makerguides.com */
#include <Servo.h>
#include <SoftwareSerial.h>

//======================================================================================================

// Create servo object to control a servo:
Servo myServo1;
Servo myServo2;
Servo myServo3;
Servo myServo4;

// Define pins for force sensitive resistors:
#define fsrPin1 A0
#define fsrPin2 A1
#define fsrPin3 A2
#define fsrPin4 A3

//Current readings from FSR
int fsrReading1;
int fsrReading2;
int fsrReading3;
int fsrReading4;

//Previous readings from FSR
int previous1 = 0;
int previous2 = 0;
int previous3 = 0;
int previous4 = 0;

// Current state of output pins
int state1 = LOW; 
int state2 = LOW;
int state3 = LOW;
int state4 = LOW;

// Booleans measuring if FSR is triggered
bool isDropped1;
bool isDropped2;
bool isDropped3;
bool isDropped4;

// Initialize Bluetooth adapter
SoftwareSerial Bluetooth(7, 8);

// Time and debounce used to stop FSR from triggering too quickly
long toggleTime = 0;  
long debounce = 40; 

// Integer used to store incoming bluetooth values. Null for now
int command = 0;
int previousCommand = 0;

// Values needed for the sound sensor
int soundSensor = 2; 
int shotCounter = 0;
bool targetsDropped = false;

// Values needed for the timer
unsigned long startMillis;
unsigned long endMillis;
bool triggerTimer = false;


//======================================================================================================


void setup() {
  // Begin Serial communication at a baud rate of 9600:
  Serial.begin(9600);

  // Attach servos to pins for operation:
  myServo1.attach(13);
  myServo2.attach(12);
  myServo3.attach(11);
  myServo4.attach(10);

  // Begin Bluetooth communication at a baud rate of 9600:
  Bluetooth.begin(9600);

  // Set Servos to default location
   myServo1.write(0);
   myServo2.write(0);
   myServo3.write(0);
   myServo4.write(120);

   // Attach sound sensors to pin for operation
   pinMode(soundSensor, INPUT);
}




void loop() {
    // Manual reset from bluetooth
    if(Bluetooth.available()) {
      command = Bluetooth.read();
      Serial.println(command);
      if(command == 49){
        //Bluetooth input will force reset.
        forceReset();
        Serial.println("Bluetooth Signal Received");
      }
    } 
    // Automatic reset without bluetooth
    else{
      readFSRinputs();
      checkTargets();
      triggerTargetReset();
     // Serial.println("Printing works");
    }
    previousCommand = command;

    soundSensorRead();
    scoreCalculation();
}


// This method will read the current FSR inputs
void readFSRinputs(){
  // Reads the FSR pin and stores the output:
  fsrReading1 = analogRead(fsrPin1);
  fsrReading2 = analogRead(fsrPin2);
  fsrReading3 = analogRead(fsrPin3);
  fsrReading4 = analogRead(fsrPin4);
}




//This method will read sound input, to measure gunshots
void soundSensorRead(){
  int sensorValue = digitalRead(soundSensor);
  if(sensorValue == 1){
    if(shotCounter == 1){
      if(!triggerTimer){
        startMillis = millis();
        triggerTimer = true;  
      }
    }
    delay(50);
    shotCounter++;
    Serial.print("Shots fired: ");
    Serial.println(shotCounter);
  }
}




// This method will check if the target has been knocked down
void checkTargets(){
  /*
   * These IF statements work as follows: We need to check if the FSR has
   * detected force. If it detects force, we need to make sure it did
   * NOT detect force in the precious iteration, to prevent the code from
   * running again. Once the force has been detected for long enough to 
   * rule out bounce, we then declare the target dropped and save the time. 
   */

  // Checks values of the first Force Sensitive Resistor-------------------
  if (fsrReading1 > 200 && previous1 < 200 && millis() - toggleTime > debounce) {
    isDropped1 = true;
    toggleTime = millis();
  }
  previous1 = fsrReading1;


  // Checks values of the second Force Sensitive Resistor------------------
  if (fsrReading2 > 200 && previous2 < 200 && millis() - toggleTime > debounce) {
    isDropped2 = true;
    toggleTime = millis();
  }
  previous2 = fsrReading2;


  // Checks values of the third Force Sensitive Resistor-------------------
  if (fsrReading3 > 200 && previous3 < 200 && millis() - toggleTime > debounce) {
    isDropped3 = true;
    toggleTime = millis();
  }
  previous3 = fsrReading3;


  // Checks values of the fourth Force Sensitive Resistor------------------
  if (fsrReading4 > 200 && previous4 < 200 && millis() - toggleTime > debounce) {
    isDropped4 = true;
    toggleTime = millis();
  }
  previous4 = fsrReading4;
}




// This method will check to see if targets need to be reset, and resets them
void triggerTargetReset(){
    // Check if all FSRs sense that the target has fallen
    if(isDropped1 && isDropped2 && isDropped3 && isDropped4){

        // Stops Timer
        endMillis = millis();
      
        // Resets targets
        myServo1.write(120);
        delay (800);
        myServo2.write(125);
        delay (800);
        myServo3.write(120);
        delay(800);
        myServo4.write(0);
        delay (800);

        // Resets motors back to their respective zeros
        myServo1.write(0);
        myServo2.write(0);
        myServo3.write(0);
        myServo4.write(120);
        delay(300);
        
        // Resets target values
        isDropped1 = false;
        isDropped2 = false;
        isDropped3 = false;
        isDropped4 = false;

        // Used for score calculation
        targetsDropped = true;
    }
}




// Doesn't check FSR values. Just resets the target
void forceReset(){
    // Resets targets
    myServo1.write(120);
    delay (800);
    myServo2.write(125);
    delay (800);
    myServo3.write(120);
    delay(800);
    myServo4.write(0);
    delay (800);

    // Resets motors back to their respective zeros
    myServo1.write(0);
    myServo2.write(0);
    myServo3.write(0);
    myServo4.write(120);
    delay(300);
        
    // Resets target values
    isDropped1 = false;
    isDropped2 = false;
    isDropped3 = false;
    isDropped4 = false;
}




// Method that calculates score based on shots sensed
void scoreCalculation(){
    if(targetsDropped){
      double math = (4.0/(double)shotCounter)*100.0;
      Serial.print("Accuracy Score: ");
      Serial.print(math);
      Serial.println("%");
      delay(100);
      targetsDropped = false;
      shotCounter = 0;
      timerCalc();
      triggerTimer = false;
    }
}




// Method that handles a simple timer
void timerCalc(){
  double math = ((double)endMillis - (double)startMillis)/1000.00;
  Serial.print("Time Taken: ");
  Serial.print(math);
  Serial.println(" seconds");
  
}


/* NOTES (To be deleted before FINAL code is submitted):
 * ---------------------------------------------------------
 * - May need a delay at end of readFSRinput method.
 * 
 * - Serial may not be needed for final verion. I think we
 *   only need it for the serial monitor, which won't be
 *   needed after we finish testing and know it works. 
 * 
 * - We need to look into what this TIME variable is actually
 *   doing. Is this a vestigal idea that is no longer required?
 *   Do we need to be saving it for the debounce stuff after the
 *   IF statement is executed?
 *
 * - I changed the servos to move to their default location in the
 *   Setup method. This may cause issues, and will need to be tested
 *   before added to the final code. Should work though. 
 */

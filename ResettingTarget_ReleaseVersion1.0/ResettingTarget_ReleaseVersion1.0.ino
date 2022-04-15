/* Code Created by Brayden Holden: https://github.com/Blady125
 * Notable Contributions by Charles Lyon: https://github.com/Charleslyon1
 * Assistance/Review by Kyle Reelfs: https://github.com/KyleReelfsSUU
 * Code Repository: https://github.com/Charleslyon1/Arduino_Target
 * 
 * This code was created for a final project in our Electronic Engineering Technology Capstone
 * Project at Southern Utah University. 
 * 
 * 
 * The project works as follows:
 * We created a device with four metal targets rated for a .22 caliber firearm. Each target is 
 * able to pivot, allowing it to stand or lie down on its back. We have attached four Force 
 * Sensitive Resistors (FSR) that sent out an analog value based on the amount of force that is
 * pressing down on the face of the FSR. This will determine whether or not the targets have
 * been knocked over. We also have 4 servo motors with arms, that will stand the targets back
 * up when triggered. Each servo has a small wooden arm attached to it that will extend the 
 * reach of the servo. The main idea is, once all four targets are shot down, they will 
 * automatically reset themselves to the starting position. This is done with the checkTargets
 * and triggerTargetReset methods. After acheiving this, we also wanted to add a few more pieces 
 * of functionality. We thought it would be neat to include a bluetooth module, which would allow 
 * us to manually reset the targets. Of course, you could walk to the target to do this, but the 
 * whole purpose of having a resetting target is eliminating that walk across the range. By using 
 * this bluetooth module, we were able to use a simple toggle switch that sends a "1" or "0" to 
 * the board. When the toggle switch is off, or the bluetooth is not connected, the board will 
 * function as an automatically resetting target. When the bluetooth is connected and the toggle 
 * switch is on, the board will launch a different method that will reset the targets, as well as 
 * any values that may have already been changed. This is handled by the "command" variable in the
 * main, and the forceReset method. The final extra we added was the sound sensor. This sensor 
 * gives us the ability to give the user more feedback on their statistics. The current code only
 * gives feedback through the serial monitor, which is not ideal. Our idea was to come up with a
 * way to send the feedback to your phone after each round. We did not have the time or the 
 * knowledge to complete this within the time frame, but you can at least see that the statistics
 * are correctly calculated in the serial monitor. Finally, we used a small external power bank
 * containing 4 AA batteries to help power the servos, as well as a 9-volt battery to power the 
 * arduino board itself, allowing this to be fully wireless. 
 * 
 * 
 * A future version of this project would ideally include:
 * - Two-way bluetooth funtionality
 * - Larger AR-500 steel targets rated for centerfire handguns or rifles
 * - Stronger servos to support heavier AR-500 steel plates
 * - Higher quality force sensitive resistors
 * - Limit switches or flex sensors instead of force sensitive resistors
 * - IOS and Android compatible application for bluetooth
 * - Higher quality chassis, ideally angled AR-500 steel, to prevent damage
 * 
 * 
 * Pictures and videos of the device will be uploaded onto the repository soon, if they are not
 * already uploaded. Our version is mostly a proof of concept. With more money and time, the
 * device would not be in danger of death due to poor marksmanship (:
 * 
 * --- Components and Parts List ---
 * Board: Arduino Uno R3
 * Bluetooth Module: DSD Tech HM-18
 * Servo Motors (x4): MG 996R 
 * Sound Sensor: KY-038
 * Targets: Any .22 Targets should work. Consult pictures.
 * 
 * 
 * **********DISCLAIMER**********
 * We are not responsible for any injuries you may sustain in recreating and using this device.
 * Guns are tools, not toys, and you should be extremely careful when using them if you are
 * not already comfortably familiar with them. Guns are not something you should be afraid of,
 * but something you should respect. Please, watch these videos below if you want to learn more
 * about gun safety from a professional. I would HIGHLY recommend it for new (or old!) gun owners. 
 * New Shooters: https://youtu.be/dGYf9AZlSyU
 * Gun Safety: https://youtu.be/kQrlDUIZ3f0
 * All three of us have extensive knowledge and training on how to properly and safely handle
 * firearms. If you wish to do this, please make sure you are doing it safely. 
 * If you want more resources on firearms, please reach out to the author (Blady125)
 */
//======================================================================================================

// Import needed libraries
#include <Servo.h>
#include <SoftwareSerial.h>

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
int command;
int previousCommand;


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
}





void loop() {
  command = Bluetooth.read();
  // Manual reset from bluetooth
  if(command == 49 && previousCommand != 49){
    //Bluetooth input will force reset.
    forceReset();
  } 
  // Automatic reset without bluetooth
  else{
    readFSRinputs();
    checkTargets();
    triggerTargetReset();
  }
  previousCommand = command;
}




// This method will read the current FSR inputs
void readFSRinputs(){
  // Reads the FSR pin and stores the output:
  fsrReading1 = analogRead(fsrPin1);
  fsrReading2 = analogRead(fsrPin2);
  fsrReading3 = analogRead(fsrPin3);
  fsrReading4 = analogRead(fsrPin4);
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
  delay(debounce);
  previous1 = fsrReading1;


  // Checks values of the second Force Sensitive Resistor------------------
  if (fsrReading2 > 200 && previous2 < 200 && millis() - toggleTime > debounce) {
    isDropped2 = true;
    toggleTime = millis();
  }

  delay(debounce);
  previous2 = fsrReading2;


  // Checks values of the third Force Sensitive Resistor-------------------
  if (fsrReading3 > 200 && previous3 < 200 && millis() - toggleTime > debounce) {
    isDropped3 = true;
    toggleTime = millis();
  }

  delay(debounce);
  previous3 = fsrReading3;


  // Checks values of the fourth Force Sensitive Resistor------------------
  if (fsrReading4 > 200 && previous4 < 200 && millis() - toggleTime > debounce) {
    isDropped4 = true;
    toggleTime = millis();
  }
  //digitalWrite(ledpin4, state4);
  delay(debounce);
  previous4 = fsrReading4;
}




// This method will check to see if targets need to be reset, and resets them
void triggerTargetReset(){
    // Check if all FSRs sense that the target has fallen
    if(isDropped1 && isDropped2 && isDropped3 && isDropped4){

        // Resets targets
        myServo1.write(120);
        delay (800);
        myServo2.write(120);
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
}




// Doesn't check FSR values. Just resets the target
void forceReset(){
    // Resets targets
    myServo1.write(120);
    delay (800);
    myServo2.write(120);
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

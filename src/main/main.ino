#include <Stepper.h> // Stepper Motor Library Full-Step

// Define the pins for the joystick
const int pinVRx = 34; // X-axis
const int pinVRy = 35; // Y-axis
const int pinSW  = 32; // Switch button

// DC Motor pins
int dcmotor1Pin1 = 27; 
int dcmotor1Pin2 = 26; 
int dcenable1Pin = 14;

// Stepper Motor settings
const int STEPS_PER_REV = 2048; // Full-Step Mode
Stepper myStepper(STEPS_PER_REV, 18, 13, 19, 12);
// Pins Sequence: (STEPS_PER_REV, IN1, IN3, IN2, IN4)

// Declaration of Emergency Stop function
void stopAllMotors();

void setup(){

  myStepper.setSpeed(10); // RPM

  pinMode(pinSW, INPUT_PULLUP);

  pinMode(dcmotor1Pin1, OUTPUT);
  pinMode(dcmotor1Pin2, OUTPUT);
  pinMode(dcenable1Pin, OUTPUT);

}

void loop() {

  // 1. Check the emergency stop button
  if(digitalRead(pinSW) == LOW){
  stopAllMotors();
  return;
  }

  // 2. Reading values ​​(from 0 to 4095 in ESP32)
  int xVal = analogRead(pinVRx);
  int yVal = analogRead(pinVRy);

  // 3. DC Motor Control (X-axis)
  // Approximately the center 2048 (dead zone between 1900 and 2200)
  if(xVal > 2200){
    // Turn forward
    digitalWrite(dcmotor1Pin1,LOW);
    digitalWrite(dcmotor1Pin2,HIGH);
    int speed = map(xVal, 2201, 4095, 0, 255);
    analogWrite(dcenable1Pin, speed);
  }else if(xVal < 1600){
    //Backward rotation
    digitalWrite(dcmotor1Pin1,HIGH);
    digitalWrite(dcmotor1Pin2,LOW);
    int speed = map(xVal, 1599, 0, 0, 255);
    analogWrite(dcenable1Pin, speed);
  }else{
    // Stop
    analogWrite(dcenable1Pin, 0);
    digitalWrite(dcmotor1Pin1,LOW);
    digitalWrite(dcmotor1Pin2,LOW);    
  }

  // 4. Stepper Motor Control (Y-axis)
  if(yVal > 2300){
    myStepper.step(15); // Turn forward
  }else  if(yVal <1600){
    myStepper.step(-15); //Backward rotation
  }

} 

// Function of stopping all engines immediately
void stopAllMotors(){
  // Turn off the DC motor
  analogWrite(dcenable1Pin, 0);
  digitalWrite(dcmotor1Pin1, LOW);
  digitalWrite(dcmotor1Pin2, LOW);

  // Disconnect power from the stepper coils
  digitalWrite(18, LOW);
  digitalWrite(13, LOW);
  digitalWrite(19, LOW);
  digitalWrite(12, LOW);
}

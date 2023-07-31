#include "MeMegaPi.h"

// Create Object Names
MeLineFollower lineFinder;
MeRGBLed LED1, LED2;
MeUltrasonicSensor US1, US2, US3;
MeLimitSwitch LS1, LS2;

// Initialize motors
MeMegaPiDCMotor motor1(PORT1A);
MeMegaPiDCMotor motor2(PORT1B);
MeMegaPiDCMotor motor3(PORT2A);
MeMegaPiDCMotor motor4(PORT2B);

// Set motor speeds
uint8_t highSpeed = 100;
uint8_t lowSpeed = 50;

//Mode 0 = Line Tracing
//Mode 1 = Extraction Zone/Obstacle Detection
//Mode 2 = Gesture Recognition
uint8_t mode = 0;

//Counter for Impact Switch
uint8_t impactCount = 0;

void setup() {
  //Define Pins of Objects
  US1.setpin(A6);
  US2.setpin(A7);
  US3.setpin(A8);
  lineFinder.setpin(A9, A10);
  LS1.setpin(A11);
  LS2.setpin(A12);
  LED1.setpin(A13);
  LED2.setpin(A14);
}

void loop() {
  // Read the state of the line sensors
  int ltSensorState = lineFinder.readSensors();

  if (LS1.touched() && LS2.touched()) {
    impactCount++;
    if (impactCount == 3) {
      if (mode == 0 && ltSensorState == S1_OUT_S2_OUT) {
        mode = 1;
      }
      impactCount = 0;
    }
  }

  switch (mode) {
    case 0:
      //Set Initial LED Colors and Show them
      LED1.setColor(0, 255, 255, 255);
      LED2.setColor(0, 255, 255, 255);
      // LED1.show();
      // LED2.show();

      // If both sensors are over the line, move forward
      if (ltSensorState == S1_IN_S2_IN) {
        motor1.run(highSpeed);
        motor2.run(highSpeed);
        motor3.run(-highSpeed);
        motor4.run(-highSpeed);
      }
      // If the left sensor is over the line, turn left
      else if (ltSensorState == S1_IN_S2_OUT) {
        motor1.run(lowSpeed);
        motor2.run(lowSpeed);
        motor3.run(lowSpeed);
        motor4.run(lowSpeed);
      }
      // If the right sensor is over the line, turn right
      else if (ltSensorState == S1_OUT_S2_IN) {
        motor1.run(-lowSpeed);
        motor2.run(-lowSpeed);
        motor3.run(-lowSpeed);
        motor4.run(-lowSpeed);
      }
      // If neither sensor is over the line, stop
      else {
        motor1.stop();
        motor2.stop();
        motor3.stop();
        motor4.stop();
      }
      break;

    case 1:
    //Still need to add driving aspect
      if (US1.distanceCm(400)<=10 || US2.distanceCm(400)<=10 || US3.distanceCm(400)<=10) {
        motor1.stop();
        motor2.stop();
        motor3.stop();
        motor4.stop();
        delay(750);
      }
  }
}

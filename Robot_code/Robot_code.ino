#include "MeMegaPi.h"
MeLineFollower lineFinder;
MeRGBLed LED1, LED2;
MeUltrasonicSensor US1, US2, US3;

// Initialize motors
MeMegaPiDCMotor motor1(PORT1A);
MeMegaPiDCMotor motor2(PORT1B);
MeMegaPiDCMotor motor3(PORT2A);
MeMegaPiDCMotor motor4(PORT2B);

// Set motor speed
uint8_t motorSpeed = 100;

void setup() {
  //Set Initial Pins
  lineFinder.setpin(A9,A10);
  LED1.setpin(A13);
  LED2.setpin(A14);
  US1.setpin(A6);
  US2.setpin(A7);
  US3.setpin(A8);

  //Mode 0 = Line Tracing
  //Mode 1 = Obstacle Detection
  //Mode 2 = Gesture Recognition
  int mode = 0;
  //Counter for Impact Switch
  int count = 0;
}

void loop() {

  switch(mode) {

  case 0:
  // Read the state of the line sensors
  int ltsensorState = lineFinder.readSensors();

  //Set Initial LED Colors and Show them
  LED1.setColor(0,255,255,255);
  LED2.setColor(0,255,255,255);
  // LED1.show();
  // LED2.show();

  // If both sensors are over the line, move forward
  if(ltsensorState == S1_IN_S2_IN) {
    motor1.run(motorSpeed);
    motor2.run(motorSpeed);
    motor3.run(-motorSpeed);
    motor4.run(-motorSpeed);
  }
  // If the left sensor is over the line, turn left
  else if(ltsensorState == S1_IN_S2_OUT) {
    motor1.run(motorSpeed);
    motor2.run(motorSpeed);
    motor3.run(motorSpeed);
    motor4.run(motorSpeed);
  }
  // If the right sensor is over the line, turn right
  else if(ltsensorState == S1_OUT_S2_IN) {
    motor1.run(-motorSpeed);
    motor2.run(-motorSpeed);
    motor3.run(-motorSpeed);
    motor4.run(-motorSpeed);
  }
  // If neither sensor is over the line, stop
  else {
    motor1.stop();
    motor2.stop();
    motor3.stop();
    motor4.stop();
  }

  }
}

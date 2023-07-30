#include "MeMegaPi.h"
MeLineFollower lineFinder;

// Initialize motors
MeMegaPiDCMotor motor1(PORT1A);
MeMegaPiDCMotor motor2(PORT1B);
MeMegaPiDCMotor motor3(PORT2A);
MeMegaPiDCMotor motor4(PORT2B);

// Set motor speed
uint8_t motorSpeed = 100;

void setup() {
  lineFinder.setpin(A9,A10);
}

void loop() {
  // Read the state of the line sensors
  int sensorState = lineFinder.readSensors();

  // If both sensors are over the line, move forward
  if(sensorState == S1_IN_S2_IN) {
    motor1.run(motorSpeed);
    motor2.run(motorSpeed);
    motor3.run(-motorSpeed);
    motor4.run(-motorSpeed);
  }
  // If the left sensor is over the line, turn left
  else if(sensorState == S1_IN_S2_OUT) {
    motor1.run(motorSpeed);
    motor2.run(motorSpeed);
    motor3.run(motorSpeed);
    motor4.run(motorSpeed);
  }
  // If the right sensor is over the line, turn right
  else if(sensorState == S1_OUT_S2_IN) {
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

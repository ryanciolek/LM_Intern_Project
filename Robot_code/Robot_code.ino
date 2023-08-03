#include "MeMegaPi.h"
#include "MeBarrierSensor.h"

// Create Object Names
MeLineFollower lineFinder;
MeRGBLed LED1, LED2;
MeBarrierSensor US1(A6), US2(A7), US3(A8);
MeLimitSwitch LS1, LS2;

// Initialize motors
MeMegaPiDCMotor motor1(PORT1A);
MeMegaPiDCMotor motor2(PORT1B);
MeMegaPiDCMotor motor3(PORT2A);
MeMegaPiDCMotor motor4(PORT2B);

// Set motor speeds
uint8_t highSpeed = 100;
uint8_t lowSpeed = 75;

//Mode 0 = Line Tracing
//Mode 1 = Extraction Zone/Obstacle Detection
//Mode 2 = Gesture Recognition
uint8_t mode = 0;

//Counter for Impact Switch and Touch Check
uint8_t impactCount = 0;
uint8_t wasTouched = 0;

void moveForward(int speed) {
        motor1.reset(PORT1A);
        motor2.reset(PORT1B);
        motor3.reset(PORT2A);
        motor4.reset(PORT2B);
        motor1.run(speed);
        motor2.run(speed);
        motor3.run(-speed);
        motor4.run(-speed);
}

void moveBackward(int speed) {
        motor1.reset(PORT1A);
        motor2.reset(PORT1B);
        motor3.reset(PORT2A);
        motor4.reset(PORT2B);
        motor1.run(-speed);
        motor2.run(-speed);
        motor3.run(speed);
        motor4.run(speed);
}

void moveRight(int speed) {
        motor1.reset(PORT1A);
        motor2.reset(PORT1B);
        motor3.reset(PORT2A);
        motor4.reset(PORT2B);
        motor1.run(-speed);
        motor2.run(speed);
        motor3.run(speed);
        motor4.run(-speed);
}

void moveLeft(int speed) {
        motor1.reset(PORT1A);
        motor2.reset(PORT1B);
        motor3.reset(PORT2A);
        motor4.reset(PORT2B);
        motor1.run(speed);
        motor2.run(-speed);
        motor3.run(-speed);
        motor4.run(speed);
}

void turnLeft(int speed) {
        motor1.reset(PORT1A);
        motor2.reset(PORT1B);
        motor3.reset(PORT2A);
        motor4.reset(PORT2B);
        motor1.run(speed);
        motor2.run(speed);
        motor3.run(speed);
        motor4.run(speed);
}

void turnRight(int speed) {
        motor1.reset(PORT1A);
        motor2.reset(PORT1B);
        motor3.reset(PORT2A);
        motor4.reset(PORT2B);
        motor1.run(-speed);
        motor2.run(-speed);
        motor3.run(-speed);
        motor4.run(-speed);
}

void stop() {
        motor1.stop();
        motor2.stop();
        motor3.stop();
        motor4.stop();
}

void setLEDColors(int red, int green, int blue) {
      LED1.setColor(0, red, green, blue);
      LED2.setColor(0, red, green, blue);
      LED1.show();
      LED2.show();
}

void setup() {
  //Define Pins of Objects
  lineFinder.setpin(A9, A10);
  LS1.setpin(A11);
  LS2.setpin(A12);
  LED1.setpin(A13);
  LED2.setpin(A14);
}

void loop() {

  // Read the state of the line sensors
  int ltSensorState = lineFinder.readSensors();

  if ((LS1.touched() && LS2.touched()) || wasTouched == 1) {
    wasTouched = 1;
    if(LS1.touched() == 0 && LS2.touched() == 0) {
          impactCount++;
          wasTouched = 0;
    if (impactCount == 3) {
      if (mode == 0 && ltSensorState == S1_OUT_S2_OUT) {
        mode = 1;
      }
      impactCount = 0;
    }
    }
  }

  switch (mode) {
    case 0:
      // if(test==1) {
      //   moveForward(highSpeed);
      // }
      //Set Initial LED Color to White
      setLEDColors(255, 255, 255);

      // If both sensors are over the line, move forward
      if (ltSensorState == S1_IN_S2_IN) {
        moveForward(highSpeed);
      }
      // If the left sensor is over the line, turn left
      else if (ltSensorState == S1_IN_S2_OUT) {
        turnLeft(highSpeed);
      }
      // If the right sensor is over the line, turn right
      else if (ltSensorState == S1_OUT_S2_IN) {
        turnRight(highSpeed);
      }
      // If neither sensor is over the line, stop
      else {
        stop();
      }
      break;

    case 1:
      // Set LEDs to green
      setLEDColors(0, 255, 0);

      //Once past white sheet, stop vehicle
      if (ltSensorState == S1_IN_S2_IN) {
        stop();
      }

      //Still need to add driving aspect
      if (US1.readSensor() == 0 || US2.readSensor() == 0 || US3.readSensor() == 0) {
          if(US1.readSensor() == 0 || (US1.readSensor() == 0 && US2.readSensor() == 0)) {
            moveRight(highSpeed);
            delay(1000);
            break;
          }
          if(US3.readSensor() == 0 || (US3.readSensor() == 0 && US2.readSensor() == 0)) {
            moveLeft(highSpeed);
            delay(1000);
            break;
          }
      } else {
        moveForward(lowSpeed);
      }
      
      break;
    case 2:
      //Gesture Recognition
  }
}

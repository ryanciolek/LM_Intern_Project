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
//Mode 2 = Gesture/Read Mode
//Mode 3 = Data Withdraw
uint8_t mode = 0;

//Counter for Impact Switch and Touch Check
uint8_t impactCount = 0;
uint8_t wasTouched = 0;

//For Obstacle Detection
bool movedRight = false;

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

void wrongWave() {
        setLEDColors(255,0,0);
        delay(750);
}

void correctWave() {
        setLEDColors(0,255,0);
        delay(750);
        setLEDColors(255,255,255);
        delay(750);
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
    if(mode == 3) {
      if(impactCount == 1) {
        setLEDColors(255,255,255);
      } else if(impactCount == 2) {
        mode = 0;
      }
    }
    }
  }

  switch (mode) {
    case 0:

      //Set Initial LED Color to White
      setLEDColors(255, 255, 255);

      // If both sensors are over the line, move forward
      if (ltSensorState == S1_IN_S2_IN) {
        moveForward(highSpeed);
      }
      // If the left sensor is over the line, turn left
      else if (ltSensorState == S1_IN_S2_OUT) {
        turnLeft(lowSpeed);
      }
      // If the right sensor is over the line, turn right
      else if (ltSensorState == S1_OUT_S2_IN) {
        turnRight(lowSpeed);
      }
      // If neither sensor is over the line, stop
      else {
        stop();
      }
      break;

    //Obstacle Detection Mode
    case 1:
      // Set LEDs to cyan
      setLEDColors(0, 100, 100);

      //Once past white sheet, stop vehicle and wait for wave
      if (ltSensorState == S1_IN_S2_IN) {
        mode = 2;
        stop();
        delay(3000);
      }

      if (ltSensorState == S1_OUT_S2_OUT) {
        if (US1.readSensor() == 0 || US2.readSensor() == 0 || US3.readSensor() == 0) {
          if(US1.readSensor() == 0 || (US1.readSensor() == 0 && US2.readSensor() == 0)) {
            moveRight(highSpeed);
            movedRight = true;
            delay(750);
            break;
          }
          if(US3.readSensor() == 0 || (US3.readSensor() == 0 && US2.readSensor() == 0)) {
            moveLeft(highSpeed);
            movedRight = false;
            delay(750);
            break;
          }
          if(US3.readSensor() == 0 && (US3.readSensor() == 0 && US2.readSensor() == 0)) {
            if(movedRight == true) {
              moveLeft(highSpeed);
            } else if(movedRight == false) {
              moveRight(highSpeed);
            }
            delay(750);
            break;
          }
        } else {
          moveForward(lowSpeed);
        }
      }
      
      
      break;

    case 2:
      //Read Mode
        setLEDColors(255,255,255); 
        delay(500);
        setLEDColors(0,0,0);
        delay(500);
        if(US1.readSensor() == 0) {
          if(US2.readSensor() == 0) {
            if(US3.readSensor() == 0) {
              setLEDColors(0,255,0);
              mode = 3;
              wasTouched = 0;
              impactCount = 0;
            }
          }
        } else if(US3.readSensor() == 0 || US2.readSensor() == 0) {
          setLEDColors(255,0,0);
          delay(1500);
        }
    break;

    case 3:
      //Data Extraction Mode
      if ((LS1.touched() && LS2.touched()) || wasTouched == 1) {
        stop();
        wasTouched = 1;
        delay(3000);
        if (LS1.touched() && LS2.touched()) { 
          mode = 0;
        }
      }
    break;
  }
}

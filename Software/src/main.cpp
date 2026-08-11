#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <pid.h>
#include <common.h>
#include <light.h>
#include <tssp.h>
#include <drive.h>

Drive move;
sensors_event_t gyro;
PID compassPID(IMUKP, IMUKI, IMUKD);
// LSystem light;
TSSP tssp;
Adafruit_BNO055 compass = Adafruit_BNO055(55, BNO055_ADDRESS_B, &Wire2);

// enum FSMStates {
//   STATE_IDLE,
//   STATE_ATTACK,
//   STATE_DEFEND,
//   STATE_LINE_AVOID
// }

// FSMStates initbehaviour = STATE_ATTACK;

void setup() {
  // Compass Setup
  while(!compass.begin())  {
    Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
  }
  delay(500);
  compass.setExtCrystalUse(true);
  delay(500);

  // Drive System Setup
  move.init();

  // TSSP Setup
  tssp.init();

}

void loop() {
  // Correction
  compass.getEvent(&gyro); //reads compass
  float heading = gyro.orientation.x > 180.0f ? gyro.orientation.x - 360.0f : gyro.orientation.x; //converts reading from 0 - 360 to -180 to 180 in order to orientate values around the front of the robot for correction
  float correction = compassPID.update(heading, 0.0f); //PID function that calculates a value to be added onto the directional movement component of the motor values in order to keep robot straight

  // TSSP
  tssp.update(); // reads TSSP plus does some calcs
  float ballDir = tssp.get_ball_direction(); // fetches the value of the direction of the ball
  float orbit = tssp.get_orbit_direction(); // fetches the value of the desired movement direction

  // Move
  move.omnimovement(-orbit, 100.0, correction); // calculates speed and direction for all motors and tells them to run
}


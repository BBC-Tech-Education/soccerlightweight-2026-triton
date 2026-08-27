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
PID defenderPID(DEFKP, DEFKI, DEFKD);
PID orbitspeedPID(SPEEDKP, SPEEDKI, SPEEDKD);
LSystem light;
TSSP tssp;
Adafruit_BNO055 compass = Adafruit_BNO055(55, BNO055_ADDRESS_B, &Wire2);

// enum FSMStates {
//   STATE_IDLE,
//   STATE_ATTACK,
//   STATE_DEFEND,
//   STATE_LINE_AVOID
// }

// FSMStates initbehaviour = STATE_ATTACK;

void setup()
{

  // Compass Setup

  while (!compass.begin(OPERATION_MODE_IMUPLUS))
  {
    Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
  }
  delay(500);
  compass.setExtCrystalUse(true);
  delay(500);

  Drive System;
  move.init();

  // TSSP Setup
  tssp.init();

  // LSB setup
  light.init();

  Serial.println("abhis gay");
}

float speed;
float dir;

void loop()
{
  // Correction
  compass.getEvent(&gyro);                                                                        // reads compass
  float heading = gyro.orientation.x > 180.0f ? gyro.orientation.x - 360.0f : gyro.orientation.x; // converts reading from 0 - 360 to -180 to 180 in order to orientate values around the front of the robot for correction
  float correction = compassPID.update(heading, 0.0f);                                            // PID function that calculates a value to be added onto the directional movement component of the motor values in order to keep robot straight
  // Serial.println(heading);

  // // TSSP
  tssp.update(); // reads TSSP plus does some calcs
  float ballDir = tssp.get_ball_direction();
  float orbit = tssp.get_orbit_direction(); // fetches the value of the desired movement direction
  float dir = orbit * -1.00;
  float speed = abs(orbitspeedPID.update(ballDir, 0));
  Serial.println(ballDir);

  if(ballDir < 15.0 || ballDir > -15.0) {
    speed = 240;
  }

  // Lights
  // light.update();
  // float lineDir = light.get_line_direction();
  // // Serial.println(lineDir);

  // defender
  float defenderX = defenderPID.update(ballDir, 0);

  move.omnimovement(dir, speed, correction); // calculates speed and direction for all motors and tells them to run
}

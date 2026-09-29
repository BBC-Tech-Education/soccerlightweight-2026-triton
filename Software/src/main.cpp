#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <pid.h>
#include <common.h>
#include <light.h>
#include <tssp.h>
#include <drive.h>
#include <camera.h>

Drive motor;
sensors_event_t gyro;
PID compassPID(IMUKP, IMUKI, IMUKD);
PID attackGoal(ATTKP, ATTKI, ATTKD);
PID orbitspeedPID(SPEEDKP, SPEEDKI, SPEEDKD);
PID defenderHori(DEFXKP, DEFXKI, DEFXKD);
PID defenderVert(DEFYKP, DEFYKI, DEFYKD);
PID defendGoal(DEFKP, DEFKI, DEFKD);
LSystem light;
TSSP tssp;
Adafruit_BNO055 compass = Adafruit_BNO055(55, BNO055_ADDRESS_B, &Wire2);
Camera cam;

enum logicStates {
  STATE_IDLE,
  STATE_ATTACK,
  STATE_DEFEND,
  STATE_LINE_AVOID
};

enum subStates {
  ATTACK_SURGE,
  ATTACK_ORBIT,
  LINE_AVOID,
  LINE_REMEMBER,
  DEFEND_FORWARD,
  DEFEND_BEHIND
};

void setup()  {
  // Compass Setup
  while (!compass.begin(OPERATION_MODE_IMUPLUS))
  {
    Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
  }
  delay(500);
  compass.setExtCrystalUse(true);
  delay(500);


  // Camera setup
  cam.init();

  // Drive System
  motor.init();

  // TSSP Setup
  tssp.init();

  // LSB setup
  light.init();

  // Soft swtiches
  pinMode(goalSwitch, INPUT);
  pinMode(stateSwitch, INPUT);
}

float speed;
float dir;

void loop() {
  // Correction
  compass.getEvent(&gyro);  // reads compass
  float heading = gyro.orientation.x > 180.0f ? gyro.orientation.x - 360.0f : gyro.orientation.x; // converts reading from 0 - 360 to -180 to 180 in order to orientate values around the front of the robot for correction
  float correction = compassPID.update(heading, 0.0f);  // PID function that calculates a value to be added onto the directional movement component of the motor values in order to keep robot straight
  // Serial.println(heading);

  // TSSP
  tssp.update(); // reads TSSP plus does some calcs
  float ballDir = tssp.get_ball_direction();

  // Camera
  cam.update(int(digitalRead(goalSwitch)));
  float goal = cam.get_goal_angle();
  // Serial.println(goal);
  float goalCorrect = attackGoal.update(goal, 95.0f);

  // Lights
  light.update();
  float lineDir = light.get_line_direction();
  // Serial.println(lineDir);

  logicStates currentState = STATE_IDLE;
  subStates currentSubState = ATTACK_SURGE;

  // state logic
  if(ballDir == 1000.0f) {
    currentState = STATE_IDLE;
  } else if(digitalRead(stateSwitch)) { // ADD SOMETHING FOR BLUETOOTH MODULE
    currentState = STATE_ATTACK;
    if(ballDir < 15.0 && ballDir > -15.0) { 
      currentSubState = ATTACK_SURGE;
    } else {currentSubState = ATTACK_ORBIT;}
  } else {
    currentState = STATE_DEFEND;
    if(ballDir > -90.0 && ballDir < 90.0) {
      currentSubState = DEFEND_FORWARD;
    } else {currentSubState =  DEFEND_BEHIND;}
  } if(lineDir != -1.00) {
    currentState = STATE_LINE_AVOID; //ADD LINE REMEMBERANCE
  }

  switch (currentState) {
    case STATE_IDLE:
      speed = 0.0f;
      break;
    case STATE_ATTACK:
      switch(currentSubState) {
        case ATTACK_ORBIT:
          dir = -1.00 * tssp.get_orbit_direction();
          speed = abs(orbitspeedPID.update(ballDir, 0));
          break;
        case ATTACK_SURGE:
          dir = 0.0f;
          speed = 240.0f;
          break;
      }
      break;
    case STATE_DEFEND:
      switch(currentSubState) {
        case DEFEND_FORWARD:
          dir = atan2(defenderVert.update(ballDir, 0), defenderHori.update(ballDir, 0));
          speed = sqrt(defenderHori.update(ballDir, 0) * defenderHori.update(ballDir, 0) + defenderVert.update(ballDir, 0) * defenderVert.update(ballDir, 0));
          break;
        case DEFEND_BEHIND:
          dir = -1.00 * tssp.get_orbit_direction();
          speed = abs(orbitspeedPID.update(ballDir, 0));
          break;
      }
    break;
    case STATE_LINE_AVOID:
      dir = lineDir;
      speed = 255.0f;
      break;
  }

  if(lineDir == 1000.0) {
    speed = abs(orbitspeedPID.update(ballDir, 0));
    dir = -1.00 * tssp.get_orbit_direction();
    if (ballDir < 25.0 && ballDir > -25.0) {
      dir = 0.0;
      speed = 150.0;
    } 
    if(ballDir == 1000.0f) {
      speed = 0.0;
    }
  } else {
    lineDir += 180;
    if (lineDir > 180) {
      lineDir -= 360;
   } else if (lineDir < -180) {
      lineDir += 360;
    }
    speed = 240.0;
    dir = -1.00 * lineDir;
  }

  Serial.println(dir);

  motor.omnimovement(0.0f, 150.0, correction); // calculates speed and direction for all motors and tells them to run
}
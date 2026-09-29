#include "drive.h"

Drive::Drive() {};

/*!
* @brief Initialises pins
*/
void Drive::init(){
    for(int i = 0; i < 4; i++) {
        pinMode(INApin[i], OUTPUT); //Set Pin mode/pin number
        pinMode(INBpin[i], OUTPUT);
        pinMode(PWMpin[i], OUTPUT);

        digitalWrite(INApin[i], HIGH);
        digitalWrite(INBpin[i], HIGH);
        delayMicroseconds(100);
        analogWriteFrequency(PWMpin[i], 15000.0f);
    }
};


/*!
*   @brief Makes a motor speed account for correction, ball direction and speed simultaneously.
*   @param direction
*          Direction of ball relative to the front of the robot
*   @param speed
*          Speed multipier i.e makes sure not to overshoot orbit
*   @param correction
*          PID to get the robot to always facing direction it was turned on at
*/
void Drive::omnimovement(float direction, float speed, float correction) {
    float movementSpeed[4];
    float highest = 0;
    
    // If ball is not seen, do not move
    if(direction == 1000) {
        speed = 0.0f;
    }

    // Calculates individual motor speeds using the parameters
    for(uint8_t i = 0; i < 4; i++) {
        movementSpeed[i] = cosf(DEG_TO_RAD * (direction - MotorDirection[i])) * speed;
        movementSpeed[i] += correction;
        if (fabs(movementSpeed[i]) >= highest) {
            highest = fabs(movementSpeed[i]);
        }
    }

    // Scales all motor speeds down if the highest speed is greater than the max speed (255)
    if(highest > 255.0f) {
        for(uint8_t i = 0; i < 4; i++) {
            movementSpeed[i] *= 255.0f / highest;
        }
    }

    // Serial.println(movementSpeed[1]);

    // Gives motors INA, INB, and PWM and gets them to run
    for(uint8_t i = 0; i < 4; i++) {
        analogWrite(PWMpin[i], round(fabs(movementSpeed[i])));
        digitalWrite(INApin[i], movementSpeed[i] > 0);
        digitalWrite(INBpin[i], movementSpeed[i] < 0);
    }
}
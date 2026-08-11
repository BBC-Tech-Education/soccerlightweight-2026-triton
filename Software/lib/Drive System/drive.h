#ifndef DRIVE_H
#define DRIVE_H

#include <pins.h>
#include <Arduino.h>
#include <pins.h>

class Drive {
public:
    Drive();
    void omnimovement(float direction, float speed, float correction);
    void init();

private:
    int INApin[4] = {FRINA, BRINA, BLINA, FLINA};
    int INBpin[4] = {FRINB, BRINB, BLINB, FLINB};
    int PWMpin[4] = {FRPWM, BRPWM, BLPWM, FLPWM};
    float MotorDirection[4] = {135.0f, 225.0f, 315.0f, 45.0f};
};

#endif
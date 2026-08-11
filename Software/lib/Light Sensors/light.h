#ifndef LIGHTSYSTEM_H
#define LIGHTSYSTEM_H

#include <Arduino.h>
#include <math.h>  
#include <pins.h>
#include <common.h>

class LSystem {
public:
    LSystem() {};
    void init();
    void update();
    float LineAvoid();
    // int lineRemember {switches between different "modes" of the line positioning.}

private:
    int muxPins[4] = {MUX1, MUX2, MUX3, MUX4};
    int muxPosition[32];
    int errorValue[16] = {100, 100, 100, 100, 700, 100, 100, 100, 100, 700, 100, 100, 100, 100, 100, 700};
    int lightReading[32];
    int lineDetection[32];
    int error[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int greenValue[32];
    float lineAngle[32] = {0, 11.25, 22.5, 33.75, 45, 56.25, 67.5, 78.75, 90, 101.25, 112.5, 123.75, 135, 146.25, 157.5, 168.75, 180, 191.25, 202.5, 213.75, 225, 236.25, 247.5, 258.75, 270, 281.25, 292.5, 303.75, 315, 326.25, 337.5, 348.75};
    float lineDirection;
    int clusters[4][32];
};

#endif
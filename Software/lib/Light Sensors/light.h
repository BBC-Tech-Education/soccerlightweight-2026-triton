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
    float get_line_direction();
    // int lineRemember {switches between different "modes" of the line positioning.}

private:
    uint8_t muxPins[4] = {MUX1, MUX2, MUX3, MUX4};
    uint8_t muxPosition[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 24, 23, 22, 21, 20, 19, 17, 18, 32, 31, 30, 29, 28, 27, 26, 25};
    uint16_t lightReading[32] = {0};
    uint8_t lineDetection[32] = {0};
    uint16_t error[32] = {0, 10, 100, 100, 100, 20, 100, 100, 100, 100, 20, 150, 20, 150, 20, 20, 20, 0, 20, 20, 20, 20, 20, 20, 20, 20, 20, 0, 0, 20, 20, 20};
    uint16_t greenValue[32];
    float lineAngle[32] = {0, 11.25, 22.5, 33.75, 45, 56.25, 67.5, 78.75, 90, 101.25, 112.5, 123.75, 135, 146.25, 157.5, 168.75, 180, 191.25, 202.5, 213.75, 225, 236.25, 247.5, 258.75, 270, 281.25, 292.5, 303.75, 315, 326.25, 337.5, 348.75};
    float lineDirection;
    int clusters[4][32];

    // int greenReading[4];
    // int LSPins[4] = {MUX3, MUX1, LSOUT1, LSOUT2};
    // float lightDirection;
};

#endif
#ifndef TSSP_H
#define TSSP_H

#include <pins.h>
#include <Arduino.h>

class TSSP {
public:
    TSSP();
    void init();
    void update();
    float get_ball_direction();
    float get_ball_strength();
    float get_orbit_direction();
    float get_orbit_speed();

private:
    int TSSPPin[16] = {TSSP1, TSSP2, TSSP3, TSSP4, TSSP5, TSSP6, TSSP7, TSSP8, TSSP9, TSSP10, TSSP11, TSSP12, TSSP13, TSSP14, TSSP15, TSSP16};
    float ballStrength;
    float orbitDirection;
    float orbitSpeed;
    float ballDirection;
};

#endif
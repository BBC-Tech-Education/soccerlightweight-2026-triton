#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <common.h>
#include <vect.h>

class Camera {
public:
    void init();
    void update();
private:
    float calc_distance();
    float calc_angle();
    int centre();
}

#endif
#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <common.h>
#include <pins.h>

class Camera {
public:
    void init();
    void update(int targetGoal);
    float get_goal_angle();
    float get_goal_distance();
private:
    float calc_distance(int x, int y);
    float calc_angle(int x, int y);
    int centre(int var);
    float goalAngle;
    float goalDistance;
};

#endif
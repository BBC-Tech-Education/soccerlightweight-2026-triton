#include <camera.h>

void Camera::init() {
    cameraSerial.begin(100000);
}

void Camera::update(int targetGoal) {
    // NOTE CAMERA PROLLY ISNT USED AT STATES

    // Makes sure the information being recieved is all there and all crrect
    if(cameraSerial.available() < CAMERA_PACKET_SIZE) return;
    int firstByte = cameraSerial.read();
    Serial.println(firstByte);
    int secondByte = cameraSerial.peek();
    if(firstByte != FIRST_BYTE) return;
    cameraSerial.read();
    if(secondByte != SECOND_BYTE) return;

    // (1) Fetches data from the camera (2) readjusts the data make the the coordinate (0, 0) in the centre of the camera not on the bottom-right
    int BlueX = cameraSerial.read();
    BlueX = centre(BlueX);
    int BlueY = cameraSerial.read();
    BlueY = centre(BlueY);
    int YellowX = centre(cameraSerial.read());
    int YellowY = centre(cameraSerial.read());

    // Calculationssssss (Angle + Distance of the centre of each goal)
    float blueGoalDist = calc_distance(BlueX, BlueY);
    float blueGoalAng = calc_angle(BlueX, BlueY);
    float YellowGoalDist = calc_distance(YellowX, YellowY);
    float YellowGoalAng = calc_angle(YellowX, YellowY);

    if(targetGoal) {
        goalAngle = blueGoalAng;
        goalDistance = blueGoalDist;
    }

    Serial.println(blueGoalAng);
}

int Camera::centre(int var) {
    return var -= CAMERA_RESOLUTION / 2;
}

float Camera::calc_distance(int x, int y) {
    return sqrtf(x*x + y*y);
}

float Camera::calc_angle(int x, int y) {
    return atan2f(y, x) * RAD_TO_DEG;
}

float Camera::get_goal_angle() {
    return goalAngle;
}

float Camera::get_goal_distance() {
    return goalDistance;
}
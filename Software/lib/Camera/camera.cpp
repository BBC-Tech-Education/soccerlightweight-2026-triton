#include <camera.h>

void Camera::init() {
    cameraSerial.begin(100000);
}

void Camera::update() {
    // NOTE CAMERA PROLLY ISNT USED AT STATES

    // Makes sure the information being recieved is all there and all crrect
    if(cameraSerial.available() < CAMERA_PACKET_SIZE) return;
    int firstByte = cameraSerial.read();
    int secondByte = cameraSerial.peak();
    if(firstByte != FIRST_BYTE) return;
    cameraSerial.read();
    if(secondByte != SECOND_BYTE) return;

    // (1) Fetches data from the camera (2) readjusts the data make the the coordinate (0, 0) in the centre of the camera not on the bottom-right
    int BlueX = centre(cameraSerial.read());
    int BlueY = centre(cameraSerial.read());
    int YellowX = centre(cameraSerial.read());
    int YellowY = centre(cameraSerial.read());

    // Calculationssssss (Angle + Distance of the centre of each goal)
    float blueGoalDist = calc_distance(BlueX, BlueY);
    float blueGoalAng = calc_angle(BlueX, BlueY);
    float YellowGoalDist = calc_distance(YellowX, YellowY);
    float YellowGoalAng = calc_angle(YellowX, YellowY);
}

int Camera::centre(int var) {
    return var -= CAMERA_RESOLUTION / 2;
}

int Camera::calc_distance(int x, int y) {
    return sqrtf(x*x + y*y);
}

int Camera::calc_angle(int x, int y) {
    return atan2f(y, x) * RAD_TO_DEG;
}
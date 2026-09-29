#include <camera.h>

void Camera::init() {
    cameraSerial.begin(115200);
}

void Camera::update(int targetGoal) {
    // Makes sure the information being recieved is all there and all crrect
    if (cameraSerial.available() < CAMERA_PACKET_SIZE) {
        return;
    }

    uint8_t first_byte = cameraSerial.read();
    uint8_t second_byte = cameraSerial.peek();

    if (first_byte != FIRST_BYTE || second_byte != SECOND_BYTE) {
        return;
    }

    // Serial.print(first_byte);
    // Serial.print("\t");
    // Serial.print(second_byte);

    (void)cameraSerial.read();

    // (1) Fetches data from the camera (2) readjusts the data make the the coordinate (0, 0) in the centre of the camera not on the bottom-right
    int8_t blue_x   = centre(cameraSerial.read());
    int8_t blue_y   = centre(cameraSerial.read());
    int8_t yellow_x = centre(cameraSerial.read());
    int8_t yellow_y = centre(cameraSerial.read());

    // Serial.print("\t");
    // Serial.print(blue_x);
    // Serial.print("\t");
    // Serial.print(blue_y);
    // Serial.print("\t");

    if(targetGoal) {
        goalAngle = calc_angle(blue_x, blue_y);
        goalDistance = calc_distance(blue_x, blue_y);
    } else {
        goalAngle = calc_angle(yellow_x, yellow_y);
        goalDistance = calc_distance(yellow_x, yellow_y);
    }

    // Serial.println(goalAngle);
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
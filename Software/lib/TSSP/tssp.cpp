#include "tssp.h"

TSSP::TSSP() {};

void TSSP::init() {
    for(int i = 0; i < 16; i++) {
        pinMode(TSSPPin[i], INPUT);
    }
}

void TSSP::update() {
    int TSSPStr[TSSPNUMBER];

    // Reset TSSP readings
    for (uint8_t i = 0; i < TSSPNUMBER; i++) {
        TSSPStr[i] = 0;
    }

    // Establishes timers for TSSP reading
    unsigned long start_time = millis();
    unsigned long current_time = millis();

    // Reads all TSSPs for a set amount of time (1.5ms) and sums a value for ball strength per TSSP
    while(current_time - start_time < 1.5) {
        for (uint8_t j = 0; j < TSSPNUMBER; j++) {
            TSSPStr[j] += 1 - digitalRead(TSSPPin[j]);
        }
        current_time = millis();
    }

    // Sets broken TSSPs to zero
     
    TSSPStr[10] = 0;
    TSSPStr[15] = 0;

    // Calculates approx. values for broken TSSPs by averaging the neighbouring TSSP's strength values
    for (uint8_t i = 0; i < TSSPNUMBER; i++) {
        if(TSSPStr[i] == 0) {
            int lower =  (i - 1 == -1) ? 15 : (i - 1);
            int higher = (i + 1 == 16) ? 0 : (i + 1);
            TSSPStr[i] = (TSSPStr[lower] + TSSPStr[higher]) / 2;
        }
    }
    
    // Prints all values

    for (uint8_t i = 0; i < TSSPNUMBER; i++) {
        Serial.print(TSSPStr[i]);
        Serial.print("     ");
    }

    Serial.println();

    // Converts each angular strength component into perpendicular i and j vectors using trig
    
    float iVector[TSSPNUMBER];
    float jVector[TSSPNUMBER];
    uint16_t highest = 0;
    uint8_t highestSensor = 0;
    
    for(int i = 0; i < TSSPNUMBER; i++) {
        iVector[i] = TSSPStr[i] * cosf(i * 30.0f * DEG_TO_RAD);
        jVector[i] = TSSPStr[i] * sinf(i * 30.0f * DEG_TO_RAD);
        if (TSSPStr[i] > highest) {
            highestSensor = i;
            highest = TSSPStr[i];
        }
    }


    // Calculates the total horizontal and vertical TSSP strength components by adding the components of the highest TSSP reading and the one's next to it

    uint8_t lower = highestSensor == 0 ? 11 : (highestSensor - 1) % TSSPNUMBER;
    float i = (iVector[highestSensor] + iVector[(highestSensor + 1) % TSSPNUMBER] + iVector[lower]);
    float j = (jVector[highestSensor] + jVector[(highestSensor + 1) % TSSPNUMBER] + jVector[lower]);

    // Uses trig to find a final ball direction
    ballDirection = atan2f(j, i) * RAD_TO_DEG;

    // Finds the distance between the ball and the robot by averaging the ball strength of the highest TSSP reading and it's neighbours
    ballStrength = (2 * highest + TSSPStr[(highestSensor + 1) % TSSPNUMBER] + TSSPStr[lower]) / 4.0f;

    // Calculates desired movement direction
    if (ballStrength != 0.0f) {
        // Accounts for the balls distance in widening / smallening (idk the word) of the orbit
        float strengthMult = min(ballStrength / 2000.0f, 1.0f);
        // Finds a value to add to the ball direction by subbing ball Dir into a cubic in order to get rough movement direction
        float orbitadd = strengthMult * constrain(0.00072 * pow(ballDirection, 3), -90.0f, 90.0f);
        orbitDirection = orbitadd + ballDirection;
        // Makes sure movement direction in between 0 and 360 since ball direction is originall between -180 and 180
        if (orbitDirection < 0.0f) {
            orbitDirection += 360.0f;
        }
        // Calculates movement speed in order ot minimising overshooting
        orbitSpeed = 60.0f + (90.0f - fabs(orbitadd)) / 3.0f;
    } else {
        // Tells robot not to move if no ball
        orbitDirection = 1000.0;
        orbitSpeed = 0.0;
    }

    // Tells robot to ignore direction movement and just move forward if the ball is between 30 and -30 degrees
    if(ballDirection < 30 && ballDirection > -30) {
        orbitDirection = 0;
    }

    // MODS IT
    orbitDirection = fmod((360.0f - orbitDirection), 360.0f);
}

float TSSP::get_ball_direction() {
    return ballDirection;
}

float TSSP::get_ball_strength() {
    return ballStrength;
}

float TSSP::get_orbit_direction() {
    return orbitDirection;
}

float TSSP::get_orbit_speed() {
    return orbitSpeed;
}

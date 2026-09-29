#include <light.h>

void LSystem::init() {
    pinMode(LSOUT1, INPUT);
    pinMode(LSOUT2, INPUT);
    
    for(int i = 0; i < 4; i++) {
        pinMode(muxPins[i], OUTPUT);
    }

    for(uint8_t n = 0; n < 16; n++) {
       for(uint8_t i = 0; i < 4; i++) {
            // Calculates mux inputs
            digitalWrite(muxPins[i], ((n >> i) & 1));
        }
        delayMicroseconds(10);
        greenValue[muxPosition[n]] = analogRead(LSOUT1);
        greenValue[muxPosition[n + 16]] = analogRead(LSOUT2);
    }
}

void LSystem::update() {
    // // Reads phototransistors and orders values into directional order
    for(uint8_t n = 0; n < 16; n++) {
        for(uint8_t i = 0; i < 4; i++) {
            // Calculates mux inputs
            digitalWrite(muxPins[i], ((n >> i) & 0x01));
        }
        delayMicroseconds(10);
        lightReading[muxPosition[n]] = analogRead(LSOUT1);
        lightReading[muxPosition[n + 16]] = analogRead(LSOUT2);
    }

    // Serial.println("A");


    for(uint8_t i = 0; i < 32; i++) {
        lineDetection[i] = lightReading[i] > (greenValue[i] + error[i]);
        if(lightReading[i] == 0) {
            lineDetection[i] = 2;
        }
    }

    // Serial.println();

    // VECTOR METHOD
    float ilvector = 0;
    float jlvector = 0;

    for(int i = 0; i < 32; i++) {
        if(lineDetection[i] == 1) {
            ilvector += cos((i * 11.25) * DEG_TO_RAD);
            jlvector += sin((i * 11.25) * DEG_TO_RAD);
        }
    }
 
    lineDirection = atan2(jlvector, ilvector) * RAD_TO_DEG;

    if(ilvector == 0.0 && jlvector == 0.0) {
        lineDirection = 1000.0;
        if(remember == 1 && prevLine != 1000.0) {
            lineDirection = prevLine;
            remember = 1;
        }
    } else { 
        lineDirection = atan2(jlvector, ilvector) * RAD_TO_DEG; 
        remember = 0;       
        if(abs(lineDirection - prevLine) > 50 && prevLine != 1000.0) {
            lineDirection = prevLine;
            remember = 1;
        }
    }

    prevLine = lineDirection;

    // Serial.println(lineDirection);
    
    // CLUSTER METHOD (PROLLY NOT)
    
    // // int clusterNumber = -1;
    // // int clusterPosition[4] = {0, 0, 0, 0};

    // // // Seperates readings into clusters
    // // // A cluster is a group of directionally consequetive phototransistors that are all seeing white.
    // // // Finds both number of groups and what phototransistors are in what cluster or if they aren't in a cluster
    // // for(int n = 0; n < LSNUMBER; n++) {
    // //     if(lineDetection[n] == 1) {
    // //         int prev = (n == 0) ? 31 : n - 1;
    // //         if (lineDetection[prev] == 0) {
    // //             clusterNumber++;
    // //             clusterPosition[clusterNumber] = 0;
    // //         } else {clusterPosition[clusterNumber]++;}

    // //         if(clusterNumber >= 0 && clusterNumber < 4) {
    // //             clusters[clusterNumber][clusterPosition[clusterNumber]] = n;
    // //         }
    // //     }
    // // }

    // // float clusterAverage[4] = {0, 0, 0, 0};

    // // // Calculates the average phototransistor direction of each cluster
    // // for(int j = 0; j <= clusterNumber; j++) {
    // //     float clusterSum = 0;
    // //     for(int i = 0; i <= clusterPosition[j]; i++) {
    // //         clusterSum += clusters[j][i] * (360.0 / 32.0);
    // //     }
    // //     clusterAverage[j] = clusterSum / (clusterPosition[j] + 1);
    // // }

    // // // Calculates the direction of the 
    // // if(clusterNumber == 0) {
    // //     lineDirection = clusterAverage[0];
    // // } else if (clusterNumber == 1) {
    // //     if((abs(clusterAverage[0] - clusterAverage[1])) < 180.0) {
    // //         lineDirection = (clusterAverage[0] + clusterAverage[1]) / 2.0;
    // //     } else {
    // //         lineDirection = fmod(((clusterAverage[0] + (clusterAverage[1] - 360)) / 2.0) + 360, 360.0);
    // //     }
    // // } else if (clusterNumber == 2) {
    // //     if(abs((clusterAverage[0] - clusterAverage[2])) < 180) {
    // //         lineDirection = (clusterAverage[0] + clusterAverage[2]) / 2;
    // //     } else {
    // //         lineDirection = fmod(((clusterAverage[0] + (clusterAverage[2] - 360.0)) / 2.0) + 360, 360.0);
    // //     }
    // // } else if (clusterNumber == 3) {
    // //     if(abs((clusterAverage[0] - clusterAverage[3])) < 180) {
    // //         lineDirection = (clusterAverage[0] + clusterAverage[3]) / 2;
    // //     } else {
    // //         lineDirection = fmod(((clusterAverage[0] + (clusterAverage[3] - 360.0)) / 2.0) + 360, 360.0);
    // //     } 
    // // } 

    // Serial.println(lightDirection);
}

float LSystem::get_line_direction() {
    return lineDirection;
}
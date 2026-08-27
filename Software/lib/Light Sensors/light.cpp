#include <light.h>

void LSystem::init() {
    // pinMode(LSOUT1, INPUT);
    // pinMode(LSOUT2, INPUT);
    
    // for(int i = 0; i < 4; i++) {
    //     pinMode(muxPins[i], OUTPUT);
    // }

    // for(uint8_t n = 0; n < 16; n++) {
    //    for(uint8_t i = 0; i < 4; i++) {
    //         // Calculates mux inputs
    //         digitalWrite(muxPins[i], ((n >> i) & 1));
    //     }
    //     delayMicroseconds(10);
    //     greenValue[muxPosition[n]] = analogRead(LSOUT1);
    //     greenValue[muxPosition[n + 16]] = analogRead(LSOUT2);
    // }
    pinMode(LSOUT1, INPUT);
    pinMode(LSOUT2, INPUT);
    pinMode(MUX2, INPUT);
    pinMode(MUX4, INPUT);


    for(int i = 0; i < 4; i++) {
        greenReading[i] = analogRead(LSPins[i]);
    }
}

void LSystem::update() {
    // // Reads phototransistors and orders values into directional order
    // for(uint8_t n = 0; n < 16; n++) {
    //     for(uint8_t i = 0; i < 4; i++) {
    //         // Calculates mux inputs
    //         digitalWrite(muxPins[i], ((n >> i) & 0x01));
    //     }
    //     delayMicroseconds(10);
    //     lightReading[muxPosition[n]] = analogRead(LSOUT1);
    //     lightReading[muxPosition[n + 16]] = analogRead(LSOUT2);
    // }

    // // Serial.println("A");
    
    // // Differentiates phototransistor readings between green and white by finding the difference between current reading and original readings for green. If output is 1 it sees white otherwise it is green
    // // for(uint8_t i = 0; i < LSNUMBER; i++) {
    // //     lineDetection[i] = lightReading[i] > (greenValue[i] + error[i]);
    // // }

    // for(int i = 0; i < 2; i++) {
    //     Serial.print(lightReading[i]);
    //     Serial.print("\t");
    // }

    // Serial.println();
    
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
    int LSWhite[4];
    int error[4] = {10, 10, 10, 10};

    for(uint8_t i = 0; i < 4; i++) {
        LSWhite[i] = (analogRead(LSPins[i]) - greenReading[i] > error[i]);
        Serial.print(LSWhite[i]);
        Serial.print("\t");
    }
    Serial.println();

    lightDirection = -1;

    for(int n = 0; n < 4; n++) {
        if(LSWhite[n] != 0) {
            lightDirection = 90.0 * n;
        }
    }

    // Serial.println(lightDirection);
}

float LSystem::get_line_direction() {
    return lightDirection;
}
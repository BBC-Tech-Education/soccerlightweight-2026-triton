#include <light.h>

void LSystem::init() {
    pinMode(LSOUT1, INPUT);
    pinMode(LSOUT2, INPUT);
    
    for(int i = 0; i < 4; i++) {
        pinMode(muxPins[i], OUTPUT);
    }
}

void LSystem::update() {
    // Reads phototransistors and orders values into directional order
    for(int n = 0; n < 16; n++) {
        for(int i = 0; i < 4; i++) {
            // Calculates mux inputs
            digitalWrite(muxPins[i], ((n >> i) & 1));
        }
        analogRead(LSOUT1);
        analogRead(LSOUT2);
    }
    
    // Differentiates phototransistor readings between green and white by finding the difference between current reading and original readings for green. If output is 1 it sees white otherwise it is green
    for(int i = 0; i < LSNUMBER; i++) {
        if(abs(lightReading[i] - greenValue[i]) > error[i]) {
            lineDetection[i] = 1;
        } else {lineDetection[i] = 0;}
    }
    
    int clusterNumber = -1;
    int clusterPosition[4] = {0, 0, 0, 0};

    // Seperates readings into clusters
    // A cluster is a group of directionally consequetive phototransistors that are all seeing white.
    // Finds both number of groups and what phototransistors are in what cluster or if they aren't in a cluster
    for(int n = 0; n < LSNUMBER; n++) {
        if(lineDetection[n] == 1) {
            int prev = (n == 0) ? 31 : n - 1;
            if (lineDetection[prev] == 0) {
                clusterNumber++;
                clusterPosition[clusterNumber] = 0;
            } else {clusterPosition[clusterNumber]++;}

            if(clusterNumber >= 0 && clusterNumber < 4) {
                clusters[clusterNumber][clusterPosition[clusterNumber]] = n;
            }
        }
    }

    float clusterAverage[4] = {0, 0, 0, 0};

    // Calculates the average phototransistor direction of each cluster
    for(int j = 0; j <= clusterNumber; j++) {
        float clusterSum = 0;
        for(int i = 0; i <= clusterPosition[j]; i++) {
            clusterSum += clusters[j][i] * (360.0 / 32.0);
        }
        clusterAverage[j] = clusterSum / (clusterPosition[j] + 1);
    }

    // Calculates the direction of the 
    if(clusterNumber == 0) {
        lineDirection = clusterAverage[0];
    } else if (clusterNumber == 1) {
        if((abs(clusterAverage[0] - clusterAverage[1])) < 180.0) {
            lineDirection = (clusterAverage[0] + clusterAverage[1]) / 2.0;
        } else {
            lineDirection = fmod(((clusterAverage[0] + (clusterAverage[1] - 360)) / 2.0) + 360, 360.0);
        }
    } else if (clusterNumber == 2) {
        if(abs((clusterAverage[0] - clusterAverage[2])) < 180) {
            lineDirection = (clusterAverage[0] + clusterAverage[2]) / 2;
        } else {
            lineDirection = fmod(((clusterAverage[0] + (clusterAverage[2] - 360.0)) / 2.0) + 360, 360.0);
        }
    } else if (clusterNumber == 3) {
        if(abs((clusterAverage[0] - clusterAverage[3])) < 180) {
            lineDirection = (clusterAverage[0] + clusterAverage[3]) / 2;
        } else {
            lineDirection = fmod(((clusterAverage[0] + (clusterAverage[3] - 360.0)) / 2.0) + 360, 360.0);
        } 
    } 
}
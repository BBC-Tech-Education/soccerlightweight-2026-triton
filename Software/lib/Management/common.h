#ifndef COMMON_H
#define COMMON_H


#define CAMERA_RESOLUTION 240
#define CAMERA_PACKET_SIZE 6
#define FIRST_BYTE 255
#define SECOND_BYTE 254

#define DEFKP 1.75
#define DEFKI 0
#define DEFKD 0.065

#define SPEEDKP 5.0
#define SPEEDKI 0
#define SPEEDKD 0.04

#define ROBOT1 true

#if ROBOT1
    #define IMUKP 2.50
    #define IMUKI 0
    #define IMUKD 0.12


#else 
    #define IMUKP
    #define IMUKI 0 
    #define IMUKD
#endif

#endif


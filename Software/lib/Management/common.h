#ifndef COMMON_H
#define COMMON_H


#define CAMERA_RESOLUTION 240
#define CAMERA_PACKET_SIZE 6
#define FIRST_BYTE 255
#define SECOND_BYTE 250

#define ROBOT1 true

#if ROBOT1
    #define IMUKP 2.5 
    #define IMUKI 0
    #define IMUKD 0.11


#else 
    #define IMUKP
    #define IMUKI 0 
    #define IMUKD
#endif

#endif


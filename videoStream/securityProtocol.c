#include "securityProtocol.h"
#include "recognizeFace.h"
#include "servo.h"
#include "led.h"
#include <stdio.h>

const char* people[4] = {"Nick", "Kunal", "Shravan", "Alex"};

void runSecurityProtocol()
{
    int faceValue = recognizeFace();
    const char* name = people[faceValue];

    if (faceValue >= 0)
    {
        printf("Face was recognized as %s\n", name);
        displaySmile();
        SERVO_nodHead();        //blocking
        clearLED();
    }
    else
    {
        printf("Face was not recognized\n");
        displayAngry();
        SERVO_shakeHead();      //blocking
        clearLED();
    }

}
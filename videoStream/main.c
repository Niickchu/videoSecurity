#include "recognizeFace.h"
#include "servo.h"
#include "common.h"
#include "securityProtocol.h"
#include "sensor.h"
#include "led.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static void initializeModules();
static void servoDemo();
int retval = -1;

int main(){
    startPRU();
    startLoading(200);
    initializeModules();

    Sensor_start();
    stopLoading();
    displayCheckMark();
    sleepForMs(1000);
    startLoading(200);
    //clearLED();
    while (true)
    {
        sleepForMs(200);
        int dist = getDistance();
        printf("Your current distance: %d\n", dist);
        if(dist < 60){
            stopLoading();
            Sensor_stop();
            runSecurityProtocol();
            Sensor_start();
            startLoading(200);
        }
    }
    PRU_cleanup();
}

static void initializeModules()
{
    printf("Initializing all modules...\n");
    SERVO_init();
    Sensor_init();
    faceRecognition_init();
    printf("all modules initialized\n");
}

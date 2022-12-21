#include "servo.h"
#include "common.h"
#include "utilities.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct{  // here is a changes
    const char* periodFileName;
    const char* dutyCycleFileName;
    const char* enableFileName;
} servoFilePaths;

typedef struct{
    double x_angle;
    double y_angle;
} servoAngles;

static servoAngles currentServoAngles = {90.0, 0.0};        //x_angle, y_angle

static const servoFilePaths servoPaths[2] = {
    {"/sys/class/pwm/pwmchip3/pwm1/period", "/sys/class/pwm/pwmchip3/pwm1/duty_cycle", "/sys/class/pwm/pwmchip3/pwm1/enable"},  //x axis servo pin21
    {"/sys/class/pwm/pwmchip5/pwm0/period", "/sys/class/pwm/pwmchip5/pwm0/duty_cycle", "/sys/class/pwm/pwmchip5/pwm0/enable"}   //y axis servo pin14
};

static void writeIntToFile(const char* filename, int inputValue);
static void SERVO_moveToX(double x_angle);
static void SERVO_moveToY(double y_angle);

void SERVO_moveToFace(int inputX, int inputY){      //pass in center of face
    double x_angle = 0;
    double y_angle = 0;

    x_angle = (inputX/426.0)*60 + 60;     //convert camera pov to angle
    y_angle = (inputY/240.0)*20;
    SERVO_moveToXY(x_angle, y_angle);
}

void SERVO_shakeHead(){
    SERVO_moveToXY(90, 0);
    for(int i=0; i < 3; i++)
    {
        SERVO_slowlyMoveToXY(140, 0, 10);
        SERVO_slowlyMoveToXY(40, 0, 10);
    }

    SERVO_moveToXY(90, 0);
}

void SERVO_nodHead(){
    SERVO_moveToXY(90, 0);
    for (int i = 0; i < 3; i++)
    {
        SERVO_slowlyMoveToXY(90, 60, 10);
        SERVO_slowlyMoveToXY(90, 0, 10);
    }
    SERVO_moveToXY(90, 0);
}


void SERVO_moveXY(double x_angle, double y_angle){    //move the servo to the specified angle input can be negative
    double absoluteXPosition = currentServoAngles.x_angle + x_angle;
    double absoluteYPosition = currentServoAngles.y_angle + y_angle;
    SERVO_moveToXY(absoluteXPosition, absoluteYPosition);
}

void SERVO_moveToXY(double x_angle, double y_angle){
    SERVO_moveToX(x_angle);
    SERVO_moveToY(y_angle);
}

static void SERVO_moveToX(double x_angle){ //move servo to the inputted angle (absolute position)

    if (x_angle > 150){
        x_angle = 150;
    }
    else if (x_angle < 30){
        x_angle = 30;
    }

    currentServoAngles.x_angle = x_angle;
    int x_dutycycle = (x_angle * DELTA_X_DUTYCYCLE / 180) + MIN_X_DUTYCYCLE;        //DELTA_X_DUTYCYCLE = 1800000ns, MIN_X_DUTYCYCLE = 600000ns
    
    writeIntToFile(servoPaths[0].dutyCycleFileName, x_dutycycle);
}

static void SERVO_moveToY(double y_angle){
    if (y_angle > 120){
        y_angle = 120;
    }
    else if (y_angle < 0){
        y_angle = 0;
    }

    currentServoAngles.y_angle = y_angle;
    int y_dutycycle = (y_angle * DELTA_Y_DUTYCYCLE / 120) + MIN_Y_DUTYCYCLE;        //DELTA_Y_DUTYCYCLE / 120 = 10000, float input is okay
    
    writeIntToFile(servoPaths[1].dutyCycleFileName, y_dutycycle);
}

void SERVO_slowlyMoveToXY(double x_angle, double y_angle, int numOfPoints){    //move the servo to the specified angle input can be negative

	x_angle = x_angle - currentServoAngles.x_angle;
	y_angle = y_angle - currentServoAngles.y_angle;
	SERVO_slowlyMoveXY(x_angle, y_angle, numOfPoints);
}

void SERVO_slowlyMoveXY(double x_angle, double y_angle, int numOfPoints){
    double x_delta = x_angle / numOfPoints;
    double y_delta = y_angle / numOfPoints;
    for (int i = 0; i < numOfPoints; i++){
        SERVO_moveXY(x_delta, y_delta);
        sleepForMs(100);
    }
}

void SERVO_init()
{
    printf("Initializing servo... ");
    // config the pin to pwm -- maybe try to do with /boot/uEnv.txt file by adding proper cape overlays
    // previously the pwmchip could change upon boot, so we needed to find the pwmchip corresponding to the ehrpwm pins
    // this may or may not have changed with the new kernel
    const char* configPin21Command = "config-pin p9.21 pwm";
    const char* configPin14Command = "config-pin p9.14 pwm";
    runCommandConst(configPin21Command);
    runCommandConst(configPin14Command);

    // set the period to 20ms = 20000000ns
    writeIntToFile(servoPaths[0].periodFileName, DEFAULT_PERIOD);
    writeIntToFile(servoPaths[1].periodFileName, DEFAULT_PERIOD);

    // set duty cycle to default position
    writeIntToFile(servoPaths[0].dutyCycleFileName, DEFAULT_X_DUTYCYCLE);
    writeIntToFile(servoPaths[1].dutyCycleFileName, DEFAULT_Y_DUTYCYCLE);

    // set enable to 1
    writeIntToFile(servoPaths[0].enableFileName, 1);
    writeIntToFile(servoPaths[1].enableFileName, 1);
    printf("Done\n");
}

static void writeIntToFile(const char* filename, int inputValue){
    FILE* file = fopen(filename, "w");
    if(file == NULL){
        printf("Error opening file %s", filename);
        exit(-1);
    }

    int charWritten = fprintf(file, "%d", inputValue);
    if (charWritten <= 0){
        printf("Error writing to file %s", filename);
        exit(-1);
    }
    fclose(file);
}

void SERVO_printCurrentAngles(){
    printf("Current Servo Angles: x: %lf, y: %lf\n", currentServoAngles.x_angle, currentServoAngles.y_angle);
}

void SERVO_shutdownServos(){
    writeIntToFile(servoPaths[0].enableFileName, 0);
    writeIntToFile(servoPaths[1].enableFileName, 0);
} //shutdown the servos

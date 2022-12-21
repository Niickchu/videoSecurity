#ifndef SERVO_H
#define SERVO_H

#define DEFAULT_PERIOD 20000000

#define DEFAULT_X_DUTYCYCLE 1500000     // 1.5ms 
#define MAX_X_DUTYCYCLE 2400000         // 2400000ns = 2.4ms full left
#define MIN_X_DUTYCYCLE 600000          // 600000ns = 0.6ms full right
#define DELTA_X_DUTYCYCLE 1800000

#define DEFAULT_Y_DUTYCYCLE 900000
#define MAX_Y_DUTYCYCLE 2100000
#define MIN_Y_DUTYCYCLE 900000
#define DELTA_Y_DUTYCYCLE 1200000

//void SERVO_moveToFace();
void SERVO_moveXY(double x_angle, double y_angle); // move the servo an additional inputted agnle from its current position
void SERVO_slowlyMoveXY(double x_angle, double y_angle, int numOfPoints);

void SERVO_moveToXY(double x_angle, double y_angle);   //move servo to the inputted angle (absolute position)
void SERVO_slowlyMoveToXY(double x_angle, double y_angle, int numOfPoints);
void SERVO_moveToFace(int inputX, int inputY);

void SERVO_init();       // initialize the servos
void SERVO_shutdownServos(); //shutdown the servos
void SERVO_printCurrentAngles();

void SERVO_shakeHead();
void SERVO_nodHead();

#endif
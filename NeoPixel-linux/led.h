#ifndef _LED_H_
#define _LED_H_

#include "sharedDataStruct.h"
#include "utilities.h"

//GRB
#define green 0xff0000
#define red 0x00ff00
#define blue 0x0000ff
#define orange 0x5aff00
#define yellow 0xffff00

void startPRU();
void displayXmark();
void displayCheckMark();
void displayFace(int x, int y, int radius);
void displaySmile();
void displayAngry();
void startLoading(int speed);
void stopLoading();

void fillMatrix(int level);
void clearLED();
void PRU_cleanup();

#endif 
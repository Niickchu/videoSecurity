#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "../NeoPixel-linux/sharedDataStruct.h"
#define STR_LEN 24

#define oneCyclesOn     700/5   // Stay on 700ns
#define oneCyclesOff    800/5
#define zeroCyclesOn    350/5
#define zeroCyclesOff   600/5
#define resetCycles     60000/5 
#define DELAY_1_MS 200

#define LED_MASK (1 << 5)
// Use pru0_rpu_r30_5 as an output (bit 5 = 0b0010 0000)
#define TRIGGER (1 << 1)

// Use pru0_pru_r31_3 as an echo (bit 3 = 0b0000 1000)
#define ECHO (1 << 3)

// Shared Memory Configuration
// -----------------------------------------------------------
#define THIS_PRU_DRAM       0x00000         // Address of DRAM
#define OFFSET              0x500           // Skip 0x100 for Stack, 0x100 for Heap (from makefile)
#define THIS_PRU_DRAM_USABLE (THIS_PRU_DRAM + OFFSET)

// This works for both PRU0 and PRU1 as both map their own memory to 0x0000000
volatile sharedMemStruct_t *pSharedMemStruct = (volatile void *)THIS_PRU_DRAM_USABLE;            
volatile register uint32_t __R30;
volatile register uint32_t __R31;

void sleepForMs(int ms)
{
    for(int i = 0; i < ms; i++)
    {
        __delay_cycles(1000000/5);//sleep for 1 ms
    }
}

void bit_on(void){
    __R30 |= LED_MASK;
    __delay_cycles(oneCyclesOn-1);
    __R30 &= ~LED_MASK;
    __delay_cycles(oneCyclesOff-2);
}
void bit_off(void){
    __R30 |= LED_MASK;
    __delay_cycles(zeroCyclesOn-1);
    __R30 &= ~LED_MASK;
    __delay_cycles(zeroCyclesOff-2);
}

int* makeCheckMark()
{
    int* matrix = malloc(8*sizeof(int));
    matrix[0] = 0b00000001;
    matrix[1] = 0b00000010;
    matrix[2] = 0b00000000;
    matrix[3] = 0b00000100;
    matrix[4] = 0b00000000;
    matrix[5] = 0b11001000;
    matrix[6] = 0b00100000;
    matrix[7] = 0b00010000;
    return matrix;
}

int* makeXMark()
{
    int* matrix = malloc(8*sizeof(int));
    matrix[0] = 0b10000001;
    matrix[1] = 0b01000010;
    matrix[2] = 0b00100100;
    matrix[3] = 0b00011000;
    matrix[4] = 0b00011000;
    matrix[5] = 0b00100100;
    matrix[6] = 0b01000010;
    matrix[7] = 0b10000001;
    return matrix;
}

void setOneLed(uint32_t custom_color)
{
    int i;
    for(i=23; i>=0; i--)
    {
        if(custom_color & (0x1<<i)) {
            bit_on();
        }
        else {
            bit_off();
        }
    }
}

void SetLedsByBit(uint32_t custom_color, int* num)
{
    int i, j, k;
    for(j=0; j<8; j++) {
        for(k=7; k>=0; k--)
        {
            if(num[j] & (0b1 << k))
            {
                setOneLed(custom_color);
            }
            else
            {
                for(i=23; i>=0; i--)
                {
                    bit_off();
                }
            }
        }
    }
    return;
}

void TurnAllColor(uint32_t custom_color, int num){
    int i, j;
    for(j=0; j<num; j++) {
        for(i=23; i>=0; i--) {
            if(custom_color & (0x1<<i)) {
                bit_on();
            }
            else {
                bit_off();
            }
        }
    }
}

void displayCustom()
{
    uint32_t color = pSharedMemStruct->color;
    for(int i=0; i< 64; i++)
    {
        if(pSharedMemStruct->data[i])
        {
            setOneLed(color);
        }
        else
        {
            for(int i=0; i<24; i++)
            {
                bit_off();
            }
        }
    }
}

void trigger()
{
    __R30 |= TRIGGER;
    __delay_cycles(10*DELAY_1_MS);
    __R30 &= ~TRIGGER;
}

int listenToEcho()
{
    int count = 0;
    while((__R31 & ECHO) == 0)
    {
       if(count == 300000)
       {
            break;
       }
       count++;
    }
    count = 0;
    while(__R31 & ECHO)
    {
        if(count == 2150000)
        {
            break;
        }
        count++;
    }
    return count;
}

void main(void)
{     
    //sample main program to display a checkmark for 5 sec, them Xmark for 5 sec and clear out
    uint32_t green = 0xff0000; 
    uint32_t red = 0x00ff00;
    
    pSharedMemStruct->doneExec = 0;
    pSharedMemStruct->dataReady = 0;
    pSharedMemStruct->sendTrigger = 0;
    while (true) 
    { 
        if (pSharedMemStruct->sendTrigger) 
        {
            pSharedMemStruct->sendTrigger = 0;
            trigger();
            int count = listenToEcho();
            pSharedMemStruct->distance = count;
            pSharedMemStruct->getResponse = 1;
        }

        if (pSharedMemStruct->dataReady) 
        {
            pSharedMemStruct->dataReady = 0;

            if(pSharedMemStruct->mode == 0)
            {
                int* xmark=makeXMark();
                SetLedsByBit(red, xmark);
                free(xmark);
            }
            else if(pSharedMemStruct->mode == 1)
            {
                int* check = makeCheckMark();
                SetLedsByBit(green, check);
                free(check);
            }
            else
            {
                displayCustom();
            }
            __delay_cycles(resetCycles);
            pSharedMemStruct->doneExec = 1; 
        }  
    }
}


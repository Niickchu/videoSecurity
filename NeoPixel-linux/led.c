#include "led.h"
#include "digits.h"
#include "fullfillment.h"
#include "faces.h"

volatile sharedMemStruct_t *pSharedPru0;
volatile void *pPruBase;
static int length = 0;
static int loadingSpeed = 300;
static char** digits;

static pthread_t idLoading;
static bool isDoneRunning = false;


void bit_on(void)
{
    pSharedPru0->data[length] = 1;
    length++;
}
void bit_off(void)
{
    pSharedPru0->data[length] = 0;
    length++;
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

void waitForLED()
{
    while(!pSharedPru0->doneExec)
    {
        sleepForMs(10);
    }
    pSharedPru0->doneExec=0;
}
void check()
{
    length = 0;
    for (int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            printf("%d", pSharedPru0->data[length]);
            length++;
        }
        printf("\n");
    }
}

void sendCustomLED(bool** matrix)
{
    length = 0;
    for (int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            pSharedPru0->data[length] = matrix[i][j];
            length++;
        }
        free(matrix[i]);
    }
    free(matrix);
    pSharedPru0->doneExec = 0;
    pSharedPru0->size = length;
    pSharedPru0->mode = 2;
    pSharedPru0->dataReady = 1;
    waitForLED();
}

void sendCustomInt(int* array)
{
    length = 0;
    for(int i=0; i<8; i++) 
    {
        for(int k=7; k>=0; k--)
        {
           pSharedPru0->data[length] = (array[i] & (1 << k));
           length++;
        }
    }
    pSharedPru0->doneExec = 0;
    pSharedPru0->size = length;
    pSharedPru0->mode = 2;
    pSharedPru0->dataReady = 1;
    waitForLED();
}

void displayFace(int x, int y, int radius)
{
    bool** matrix = malloc(8*sizeof(bool*));
    for(int i=0; i<8; i++)
    {
        matrix[i] = malloc(8);
    }
    for (int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            matrix[i][j] = 0;
        }
        
    }
    matrix[x][y] = 1;
    for(int i = x; i < x + radius; i++)
    {
        if(i<8 && i>=0)
        {
            for(int j = y; j > y - radius; j--)
            {
                if(j<8 && j>=0)
                {
                    matrix[i][j] = 1;
                }
            }
        }
    }
    pSharedPru0->color = 0x00000f;
    sendCustomLED(matrix);
}

void displaySmile()
{
    int* matrix = getSmile();
    pSharedPru0->color = green;
    sendCustomInt(matrix);
}

void displayAngry()
{
    int* matrix = getAngry();
    pSharedPru0->color = red;
    sendCustomInt(matrix);
}

void displayXmark()
{
    pSharedPru0->mode = 0;
    pSharedPru0->dataReady = 1;
    waitForLED();
}

static void* displayLoading(void *_)
{
    int* load1 = getLoad1();
    int* load2 = getLoad2();
    pSharedPru0->color = blue;
    int i=0;
    while(!isDoneRunning)
    {
        if(i%2)
        {
            sendCustomInt(load1);
        }            
        else
        {
            sendCustomInt(load2);
        }
        sleepForMs(loadingSpeed);
        i++;
    }
    free(load1);
    free(load2);
}

void startLoading(int speed)
{
    loadingSpeed = speed;
    pthread_create(&idLoading, NULL, &displayLoading, NULL);
}

void stopLoading()
{
    isDoneRunning = true;
    pthread_join(idLoading, NULL);
}

void displayCheckMark()
{
    
    pSharedPru0->mode = 1;
    pSharedPru0->dataReady = 1;
    waitForLED();
}

void fillMatrix(int level)
{
    if(level==1)
    {
        pSharedPru0->color = red;
    }
    else if(level==2)
    {
        pSharedPru0->color = orange;
    }
    else if(level==3)
    {
        pSharedPru0->color = yellow;
    }
    else if(level==4)
    {
        pSharedPru0->color = green;
    }
    sendCustomLED(fillLevels(level));
}

void check2()
{
    printf("size %d     mode %d     ready %d    finished %d\n", 
    pSharedPru0->size, pSharedPru0->mode, pSharedPru0->dataReady, pSharedPru0->doneExec);
}

void clearLED()
{
    bool** matrix = malloc(8*sizeof(bool*));
    for(int i=0; i<8; i++)
    {
        matrix[i] = calloc(8, 1);
    }
    sendCustomLED(matrix);
}

void startPRU()
{
    // Get access to shared memory for my uses
    pPruBase = getPruMmapAddr();
    pSharedPru0 = PRU0_MEM_FROM_BASE(pPruBase);
    runCommand("sudo config-pin P9_27 pruout");
    runCommand("echo start > /sys/class/remoteproc/remoteproc1/state");
    Digits_initialize();
    digits = copy_head();
}

void PRU_cleanup()
{
    freePruMmapAddr(pPruBase);
}
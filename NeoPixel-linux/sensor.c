#include "sensor.h"
#include "utilities.h"
#include <pthread.h>

#define AVG 0.95

volatile sharedMemStruct_t * pSharedPruSensor;
static volatile void *pPruBase;

static pthread_t idSensor;
static bool isDoneRunning = false;

int distance = 0;

static void* readDistance(void *_)
{
    while (!isDoneRunning)
    {   
        pSharedPruSensor->getResponse = 0;
        pSharedPruSensor->distance= 0;
        pSharedPruSensor->sendTrigger = 1;
        while (true)
        {
            if(pSharedPruSensor->getResponse)
            {
                if(pSharedPruSensor->distance)
                {
                    distance = (1-AVG)*pSharedPruSensor->distance + AVG*distance;
                }
                break;
            }
            sleepForMs(0.1);
        }   
        sleepForMs(1);
    }
}

int getDistance()
{
    return distance/2000;
}

void Sensor_start()
{
    // Get access to shared memory for my uses
    pPruBase = getPruMmapAddr();
    pSharedPruSensor = PRU0_MEM_FROM_BASE(pPruBase);
    runCommand("sudo config-pin P9_29 pruout");
    runCommand("sudo config-pin P9_28 pruin");
    pthread_create(&idSensor, NULL, &readDistance, NULL);
}

void Sensor_cleanUp()
{
    isDoneRunning = true;
    pthread_join(idSensor, NULL);
}

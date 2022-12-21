#include "led.h"
#include "sensor.h"


volatile sharedMemStruct_t * pSharedPruMain;

int main()
{
    printf("Sharing memory with PRU\n");
    startPRU();
    Sensor_start();
    startLoading(150);
    for(int i=0; i<20; i++)
    {
        printf("distance: %d \n",getDistance());
        sleepForMs(500);
    }
    //     if(i%2)
    //     {
    //         displaySmile();
    //     }
    //     else
    //     {
    //         displayAngry();
    //     }
    //     sleepForMs(2000);
    // }
    // fillMatrix(1);
    // fillMatrix(2);
    // fillMatrix(3);
    // fillMatrix(4);
    //sleepForMs(10000);
    stopLoading();
    displaySmile();
    sleepForMs(500);
    clearLED();
    Sensor_cleanUp();
    PRU_cleanup();
    return 0;
}
#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include <time.h>
#include <string.h>
#include "stdio.h"

#include "ADS1256.h"
#include "DAC8532.h"

#define SCT013_1 0
#define SCT013_2 1
#define SCT013_3 2
#define V_1 6
#define V_2 7

#define R1 48000.0
#define R2 4400.0

#define ADC_BITS 24
#define ADC_COUNTS  (1<<ADC_BITS)


void  Handler(int signo)
{
    //System Exit
    printf("\r\nEND                  \r\n");
    DEV_ModuleExit();

    exit(0);
}

int main(void)
{
    UDOUBLE ADC[8],i;
    float x;
    
    printf("demo\r\n");
    DEV_ModuleInit();
    
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);

    if(ADS1256_init() == 1){
        printf("\r\nEND                  \r\n");
        DEV_ModuleExit();
        exit(0);
    }
    
    long count = 0;
    double sum = 0;
    double IVAL = 0;
    double minI, maxI;
    while(1){
        count++;

        /*ADS1256_GetAll(ADC);
        for(i=0;i<8;i++) {
            printf("Volt A%d %f\r\n", i, ADC[i] * 5.0 / 0x7fffff);
        }*/

        // Calc current
        double ADC0 = ADS1256_GetChannalValue(0) * 5.0 / 0x7fffff;
        printf("Volt A0 %f\r\n", ADC0);
        if(minI > ADC0) minI = ADC0;
        if(maxI < ADC0) maxI = ADC0;

        if(count % 100 == 0) {
            //IVAL = sum / 100.0f;
            IVAL = (minI + maxI) / 2;
            minI = maxI = ADC0;
            sum = 0;
        }

        printf("IVAL : %f  \r\n", IVAL);
	    printf("\33[1A");//Move the cursor up 8 lines


        /*float v1 = (ADC[V_1]*5.0/0x7fffff) / (R2/(R1+R2));
        float v2 = (ADC[V_2]*5.0/0x7fffff) / (R2/(R1+R2));
        printf("V1=%f\r\n",v1);
        printf("V2=%f\r\n",v2);
        float a1 = (ADC[SCT013_1]*5.0/0x7fffff);
        float a2 = (ADC[SCT013_2]*5.0/0x7fffff);
        float a3 = (ADC[SCT013_3]*5.0/0x7fffff);
        printf("A1=%f\r\n",a1);
        printf("A2=%f\r\n",a2);
        printf("A3=%f\r\n",a3);
        float ax = calcIrms(1480, SCT013_1);
        printf("AX=%f\r\n",ax);
        printf("\r\n\r\n");*/

        //sleep(1);

    }
    return 0;
}

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
    while(1){
        count++;

        ADS1256_GetAll(ADC);
        for(i=0;i<8;i++) {
            printf("%d %f\r\n", i, ADC[i] * 5.0 / 0x7fffff);
        }
        sum +=  ADC[0] * 5.0 / 0x7fffff;

        if(count % 1000 == 0) {
            printf("SUM : %f  \r\n", sum / 1000.0f);
            sum = 0;
        }
        printf("\33[9A");//Move the cursor up 8 lines


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

        // Adapt voltage
        x = (ADC[0] >> 7)*5.0/0xffff;
        DAC8532_Out_Voltage(channel_B, (x));
        DAC8532_Out_Voltage(channel_A, (3.3 - x));

        //sleep(1);

    }
    return 0;
}

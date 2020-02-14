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

UDOUBLE sampleI;
UDOUBLE offsetI;
UDOUBLE filteredI;
UDOUBLE sumI;
UDOUBLE sqI;
UDOUBLE ICAL = 60.0;

void  Handler(int signo)
{
    //System Exit
    printf("\r\nEND                  \r\n");
    DEV_ModuleExit();

    exit(0);
}

double calcIrms(unsigned int Number_of_Samples, UBYTE pin)
{
    for (unsigned int n = 0; n < Number_of_Samples; n++)
    {
        sampleI = ADS1256_GetChannalValue(pin);

        // Digital low pass filter extracts the 2.5 V or 1.65 V dc offset,
        //  then subtract this - signal is now centered on 0 counts.
        offsetI = (offsetI + (sampleI-offsetI)/1024);
        filteredI = sampleI - offsetI;

        // Root-mean-square method current
        // 1) square current values
        sqI = filteredI * filteredI;
        // 2) sum
        sumI += sqI;
    }

    double I_RATIO = ICAL *((5.0/1000.0) / (ADC_COUNTS));
    Irms = I_RATIO * sqrt(sumI / Number_of_Samples);

    //Reset accumulators
    sumI = 0;
    //--------------------------------------------------------------------------------------

    return Irms;
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
    
    
    while(1){

        ADS1256_GetAll(ADC);
        for(i=0;i<8;i++){
            //printf("%d %f\r\n",i,ADC[i]*5.0/0x7fffff);
        }

        float v1 = (ADC[V_1]*5.0/0x7fffff) / (R2/(R1+R2));
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
        printf("\r\n\r\n");

        // Adapt voltage
        x = (ADC[0] >> 7)*5.0/0xffff;
        DAC8532_Out_Voltage(channel_B, (x));
        DAC8532_Out_Voltage(channel_A, (3.3 - x));

        sleep(1);

    }
    return 0;
}

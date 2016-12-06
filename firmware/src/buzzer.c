
/* Buzzer.c */

#include "buzzer.h"
#include "peripheral/oc/plib_oc.h"
#include "peripheral/tmr/plib_tmr.h"
#include "queue.h"

#define QUEUE_SEND_FREQUENCY_MS         ( 50 / portTICK_PERIOD_MS )

/* The queue used by both tasks. */
extern QueueHandle_t xQueue;

BUZZER_DATA buzzerData;

void BUZZER_Initialize ( )
{   
}

void Buzzer_set(float freq)
{
    PLIB_TMR_Stop(TMR_ID_2);
    PLIB_OC_Disable(OC_ID_1);

    vTaskDelay(QUEUE_SEND_FREQUENCY_MS);
    PLIB_TMR_Counter16BitClear(TMR_ID_2);
    PLIB_TMR_PrescaleSelect(TMR_ID_2, TMR_PRESCALE_VALUE_256);
    
    /* TMR_Period set */
    float per = 1/ (float)(0.0000000125 * 256 * freq) ;
    PLIB_TMR_Period16BitSet(TMR_ID_2, per);
    
    PLIB_OC_ModeSelect(OC_ID_1,  OC_DUAL_COMPARE_CONTINUOUS_PULSE_MODE);
    PLIB_OC_BufferSizeSelect(OC_ID_1, OC_BUFFER_SIZE_16BIT);
    PLIB_OC_TimerSelect(OC_ID_1, OC_TIMER_16BIT_TMR2);
    PLIB_OC_FaultInputSelect(OC_ID_1, OC_FAULT_DISABLE);
    PLIB_OC_Buffer16BitSet(OC_ID_1, 0);
    
    /* width set */
//    float wid= per * duty / 100.0 ;
    PLIB_OC_PulseWidth16BitSet(OC_ID_1, 32);

    PLIB_TMR_Start(TMR_ID_2);
    PLIB_OC_Enable(OC_ID_1);

    vTaskDelay(QUEUE_SEND_FREQUENCY_MS);
    
    /* stop buzzer */
    PLIB_TMR_Stop(TMR_ID_2);
    PLIB_OC_Disable(OC_ID_1);
}

float freq=0;
void BUZZER_Tasks ( void )
{   
    unsigned long ulReceivedValue = 0;
    xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );
    
    /* same freq */
//    switch(ulReceivedValue){
//        case 100000UL : Buzzer_set();
//        case 10000UL : Buzzer_set();
//        case 1000UL : Buzzer_set();
//        case 100UL: Buzzer_set(); break;
//    }
            
    /* buzzer by freq */
//    if( ulReceivedValue == 100UL ){ 
//        Buzzer_set(1250);          
//    }
//    if( ulReceivedValue == 1000UL ){
//        int time=0;
//        while(time<2){
//            Buzzer_set(1125);
//            time++;
//        }
//    }
//    if( ulReceivedValue == 10000UL ){
//        int time=0;
//        while(time<3){
//            Buzzer_set(1000);
//            time++;
//        }
//    }
    /* buzzer by time */
//    if( ulReceivedValue == 100UL ){
//        Buzzer_set(1000,25,200);
//        vTaskDelay(QUEUE_SEND_FREQUENCY_MS );
//        Buzzer_set(1000,25,50);        
//    }
//    if( ulReceivedValue == 1000UL ){
//        Buzzer_set(1000,10,50);
//        vTaskDelay(QUEUE_SEND_FREQUENCY_MS );
//         Buzzer_set(1500,10,50);
//        vTaskDelay(QUEUE_SEND_FREQUENCY_MS );
//        Buzzer_set(2000,10,100);
//    }
//    if( ulReceivedValue == 10000UL ){
//        Buzzer_set(750,25,50);
//    }
//    int i; int s=0;
    /* example buzzer by high(freq) */
//    if(ulReceivedValue == 100UL){
//        int num[30]={3,4,2,1,2,3,3,3,4,2,2,2,4,3,3,3,4,3,2,1,2,3,3,3,4,2,2,3,2,1};
//        for(i=0;i<30;i++){
//            s= num[i];
//            switch(s){
//                case 4 : vTaskDelay(100); break;
//                case 3 : Buzzer_set(1250); break;
//                case 2 : Buzzer_set(1125); break;
//                case 1 : Buzzer_set(1000); break;
//            }
//        }
//    }
//    if(ulReceivedValue == 1000UL){      
//        int num[27]={5,5,6,6,5,5,3,7,5,5,3,3,2,7,5,5,6,6,5,5,3,7,5,3,2,3,1};
//        for(i=0;i<27;i++){
//            s=num[i];
//            switch(s){
//                case 7 : vTaskDelay(100); break;
//                case 6 : Buzzer_set(1625); break;
//                case 5 : Buzzer_set(1500); break;
//                case 3 : Buzzer_set(1250); break;
//                case 2 : Buzzer_set(1125); break;
//                case 1 : Buzzer_set(1000); break;
//            }
//        }
//    }
//    if(ulReceivedValue == 10000UL){
//        int num[]={5,7,3,5,7,3,7,6,5,4,3,4,5};
//        for(i=0;i<13;i++){
//            s=num[i];
//            switch(s){
//                case 7 : vTaskDelay(100); break;
//                case 6 : Buzzer_set(1625,25,50); break;
//                case 5 : Buzzer_set(1500,25,50); break;
//                case 4 : Buzzer_set(1375,25,50); break;
//                case 3 : Buzzer_set(1250,25,50); break;
//            }
//        }
//    }  
    /* input freq by keycode */
    if(ulReceivedValue == 1UL) freq = 500;
    if(ulReceivedValue == 10UL) freq = 1000;
    if(ulReceivedValue == 100UL) freq = 2000;
    if(ulReceivedValue == 1000UL) freq = 0;
    if(ulReceivedValue == 10000UL) Buzzer_set(freq);
    
} //buzzer task

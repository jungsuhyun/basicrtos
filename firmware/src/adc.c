
/* Adc.c */

#include "adc.h"
#include "peripheral/oc/plib_oc.h"
#include "queue.h"
#define QUEUE_SEND_FREQUENCY_MS         ( 1000 / portTICK_PERIOD_MS )
#define QUEUE_LENGTH                    ( 1 )

QueueHandle_t xPrintQueue = NULL;
ADC_DATA adcData;
typedef struct {
	void *buf;  //
	size_t sz;  //
} QPacket;



void ADC_Initialize ( void )
{
    
    /* Select Clock Source */
    PLIB_ADC_ConversionClockSourceSelect(ADC_ID_1, ADC_CLOCK_SOURCE_SYSTEM_CLOCK);
    /* Select Clock Prescaler */
    PLIB_ADC_ConversionClockSet(ADC_ID_1, SYS_CLK_FREQ, 320000000);

    /* Select Power Mode */
    PLIB_ADC_StopInIdleDisable(ADC_ID_1);
	
    /* Select Voltage Reference */
    PLIB_ADC_VoltageReferenceSelect(ADC_ID_1, ADC_REFERENCE_VREFPLUS_TO_AVSS);

    /* Sampling Selections */
    /* Select Sampling Mode */
    PLIB_ADC_SamplingModeSelect(ADC_ID_1, ADC_SAMPLING_MODE_MUXA);
    /* Number of Samples Per Interrupt */
    PLIB_ADC_SamplesPerInterruptSelect(ADC_ID_1, ADC_16SAMPLES_PER_INTERRUPT);

    /* Conversion Selections */
    /* Select Trigger Source */
    PLIB_ADC_ConversionTriggerSourceSelect(ADC_ID_1, ADC_CONVERSION_TRIGGER_INTERNAL_COUNT);
    /* Select Result Format */
    PLIB_ADC_ResultFormatSelect(ADC_ID_1, ADC_RESULT_FORMAT_INTEGER_16BIT);
    /* Buffer Mode */
    PLIB_ADC_ResultBufferModeSelect(ADC_ID_1, ADC_BUFFER_MODE_ONE_16WORD_BUFFER);

    /* Channel Selections */
    /* MUX A Negative Input Select */
    PLIB_ADC_MuxChannel0InputNegativeSelect(ADC_ID_1, ADC_MUX_A, ADC_INPUT_NEGATIVE_VREF_MINUS);
 

    /* MUX A Positive Input Select */
    PLIB_ADC_MuxChannel0InputPositiveSelect(ADC_ID_1, ADC_MUX_A, ADC_INPUT_POSITIVE_AN6);
    xPrintQueue = xQueueCreate( 2, sizeof( char* ) );

}

float bett; 
char buffer[100];
void ADC_Tasks ( void )
{
    float bett=0;
    ADC_Initialize();
    
    PLIB_ADC_Enable(ADC_ID_1);
    PLIB_ADC_SamplingStart(ADC_ID_1);
    
    PLIB_ADC_InputScanMaskAdd(ADC_ID_1, ADC_INPUT_POSITIVE_AN6);
    bett= (float) PLIB_ADC_ResultGetByIndex(ADC_ID_1, 0) / 1024.0*10;
    
    /* adc output */
//    printf("%.2f\n",bett);
    
    /* send queue */
//     xQueueSend( xPrintQueue, &bett, 0U );
    
    /* string print */
//    sprintf(beffer, "%.2f", bett);  
//    printf("%s\n",buffer);
    
    /* string queue */
//    char   *ptr_dest = (char *)pvPortMalloc( 50);
//    sprintf(ptr_dest, "%.2f", bett);    
////    printf("%s\n",ptr_dest);
//    xQueueSend( xPrintQueue, &ptr_dest, 0U );
    
    /* string size queue */
    //int size = sizeof(bett);
    //char   *ptr_dest = (char *)pvPortMalloc( size +1 );
    //memset(ptr_dest, 0, size+1);
    //sprintf(ptr_dest, "%.2f", bett);
    //xQueueSend( xPrintQueue, &ptr_dest, 0U );
    //vPortFree(ptr_dest);
    
    /* string struct queue */
    sprintf(buffer, "%.2f", bett);
    long size = strlen(buffer)+1;
                
    QPacket pk; //qpacket declaration 
    pk.sz = size;  //qpacket size
    pk.buf = (void*) pvPortMalloc(size+1);  //memory assignment
    memset(pk.buf, 0, size+1); //reset
    memcpy(pk.buf, buffer, size+1);
    xQueueSend( xPrintQueue, &pk, 0 );

    PLIB_ADC_InputScanMaskRemove(ADC_ID_1, ADC_INPUT_POSITIVE_AN6);
    PLIB_ADC_SamplingStop(ADC_ID_1);
    PLIB_ADC_Disable(ADC_ID_1);
    vTaskDelay(QUEUE_SEND_FREQUENCY_MS );
    
}
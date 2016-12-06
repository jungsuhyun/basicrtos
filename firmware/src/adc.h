
/* Adc.h */

#ifndef _ADC_H
#define _ADC_H


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "peripheral/adc/plib_adc.h"


typedef enum
{
	/* Application's state machine's initial state. */
	ADC_STATE_INIT=0,

	/* TODO: Define states used by the application state machine. */

} ADC_STATES;


typedef struct
{
    /* The application's current state */
    ADC_STATES state;

    /* TODO: Define any additional data used by the application. */


} ADC_DATA;



void ADC_Initialize ( void );


void ADC_Tasks( void );

void DRV_ADC_Initialize(void);

inline void DRV_ADC_DeInitialize(void);

inline void DRV_ADC_Open(void);

inline void DRV_ADC_Close(void);

inline void DRV_ADC_Start(void);

inline void DRV_ADC_Stop(void);

inline void DRV_ADC_NegativeInputSelect(ADC_MUX mux, ADC_INPUTS_NEGATIVE input);

inline void DRV_ADC_PositiveInputSelect(ADC_MUX mux, ADC_INPUTS_POSITIVE input);

inline void DRV_ADC_ChannelScanInputsAdd(ADC_INPUTS_SCAN scanInput);

inline void DRV_ADC_ChannelScanInputsRemove(ADC_INPUTS_SCAN scanInput);

ADC_SAMPLE DRV_ADC_SamplesRead(uint8_t bufIndex);

bool DRV_ADC_SamplesAvailable(void);


#endif /* _ADC_H */

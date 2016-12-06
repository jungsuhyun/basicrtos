
/* Buzzer.h */

#ifndef _BUZZER_H
#define _BUZZER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"


typedef enum
{
	/* Application's state machine's initial state. */
	BUZZER_STATE_INIT=0,

	/* TODO: Define states used by the application state machine. */

} BUZZER_STATES;


typedef struct
{
    /* The application's current state */
    BUZZER_STATES state;

    /* TODO: Define any additional data used by the application. */


} BUZZER_DATA;


void BUZZER_Initialize ( void );


void BUZZER_Tasks( void );


#endif /* _BUZZER_H */

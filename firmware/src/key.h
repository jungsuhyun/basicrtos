 
/* Key.h */ 

#ifndef _KEY_H
#define _KEY_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"


typedef enum
{
	/* Application's state machine's initial state. */
	KEY_STATE_INIT=0,

	/* TODO: Define states used by the application state machine. */

} KEY_STATES;


typedef struct
{
    /* The application's current state */
    KEY_STATES state;

    /* TODO: Define any additional data used by the application. */


} KEY_DATA;



void KEY3_Initialize ( void );



void KEY_Tasks( void );


#endif /* _KEY_H */



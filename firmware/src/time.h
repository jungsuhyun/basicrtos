/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    time.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
//DOM-IGNORE-END

#ifndef _TIME_H
#define _TIME_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "driver/i2c/drv_i2c.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
	/* Application's state machine's initial state. */
	TIME_STATE_INIT=0,

	/* TODO: Define states used by the application state machine. */

} TIME_STATES;




// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */
typedef struct
{
    /* The application's current state */
    TIME_STATES state;

    /* TODO: Define any additional data used by the application. */


} TIME_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

	
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void TIME_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the 
    application in its initial state and prepares it to run so that its 
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    TIME_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void TIME_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks3 ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    TIME_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */
//SemaphoreHandle_t xReadRtcSemaphore;
//QueueHandle_t rtcSetTimeQue;

typedef struct
{
    char yy;
    char mm;
    char dd;
    char h;
    char m;
    int s;
    TickType_t ms;
}DEV_TIME;






void TIME_Tasks( void );



void DRV_I2C0_DeInitialize(void)
{
    /* Disable I2C0 */
    PLIB_I2C_Disable(I2C_ID_2);
}

// *****************************************************************************
// Section: Instance 0 Byte Transfer Functions (Master/Slave)
// *****************************************************************************
bool DRV_I2C0_SetUpByteRead(void)
{
    /* Check for recieve overflow */
    if ( PLIB_I2C_ReceiverOverflowHasOccurred(I2C_ID_2))  
    {
        PLIB_I2C_ReceiverOverflowClear(I2C_ID_2); 
        return false;
    }
	
    /* Initiate clock to receive */
    PLIB_I2C_MasterReceiverClock1Byte(I2C_ID_2);
    return true;
}

bool DRV_I2C0_WaitForReadByteAvailable(void)
{
	/* Wait for Recieve Buffer Full */
    if(PLIB_I2C_ReceivedByteIsAvailable(I2C_ID_2))
       return true;
    else
       return false;
}

uint8_t DRV_I2C0_ByteRead(void)
{	
    /* Return recieved value */
    return (PLIB_I2C_ReceivedByteGet(I2C_ID_2));
}

bool DRV_I2C0_ByteWrite(const uint8_t byte)
{
    /* Check for recieve overflow */
    if ( PLIB_I2C_ReceiverOverflowHasOccurred(I2C_ID_2))  
    {
        PLIB_I2C_ReceivedByteGet(I2C_ID_2);
		PLIB_I2C_ReceiverOverflowClear(I2C_ID_2); 
        return false;
    }
    
    /* Check for transmit overflow */
    if (PLIB_I2C_TransmitterOverflowHasOccurred(I2C_ID_2))
    {
		PLIB_I2C_TransmitterOverflowClear(I2C_ID_2);
        return false;
    }
    
    /* Transmit byte */
    PLIB_I2C_TransmitterByteSend(I2C_ID_2, byte);
    
    return true;
}

bool DRV_I2C0_WaitForByteWriteToComplete(void)
{
    /* Check for transmit busy */
    while(PLIB_I2C_TransmitterIsBusy(I2C_ID_2)); 

    /* Check to see if transmit has completed */
    while(!PLIB_I2C_TransmitterByteHasCompleted(I2C_ID_2));
	
    return true;
}

bool DRV_I2C0_WriteByteAcknowledged(void)
{
    /* Check to see if transmit ACKed = true or NACKed = false */
    if (PLIB_I2C_TransmitterByteWasAcknowledged(I2C_ID_2))
       return true;
    else
       return false;
}

// *****************************************************************************
// Section: Instance 0 I2C Master Mode Functions
// *****************************************************************************
void DRV_I2C0_BaudRateSet(I2C_BAUD_RATE baudRate)
{
    /* Disable I2C0 */
    PLIB_I2C_Disable(I2C_ID_2);
    
    /* Change baud rate */
    PLIB_I2C_BaudRateSet(I2C_ID_2, SYS_CLK_PeripheralFrequencyGet(CLK_BUS_PERIPHERAL_1), baudRate);

    /* Low frequency is enabled (**NOTE** PLIB function revereted) */
    PLIB_I2C_HighFrequencyEnable(I2C_ID_2);
 
    /* Enable I2C0 */
    PLIB_I2C_Enable(I2C_ID_2);       
}

bool DRV_I2C0_MasterBusIdle(void)
{
    if (PLIB_I2C_BusIsIdle(I2C_ID_2))
       return true;
    else
       return false;
}

bool DRV_I2C0_MasterStart(void)
{
    /* Check for recieve overflow */
    if ( PLIB_I2C_ReceiverOverflowHasOccurred(I2C_ID_2))  
    {
        PLIB_I2C_ReceiverOverflowClear(I2C_ID_2); 
        return false;
    }
    
    /* Check for transmit overflow */
    if (PLIB_I2C_TransmitterOverflowHasOccurred(I2C_ID_2))
    {
        PLIB_I2C_TransmitterOverflowClear(I2C_ID_2);
        return false;
    }

    /* Check for bus collision errors */
    if (PLIB_I2C_ArbitrationLossHasOccurred(I2C_ID_2))
    {
        return false;
    }
    
    /* Issue start */
    PLIB_I2C_MasterStart(I2C_ID_2);
       
    return true;
}

bool DRV_I2C0_WaitForStartComplete(void)
{
    /* Wait for start/restart sequence to finish  (hardware clear) */
    while(!PLIB_I2C_StartWasDetected(I2C_ID_2));
	 
    return true;
}

bool DRV_I2C0_MasterRestart(void)
{
    /* Check for recieve overflow */
    if ( PLIB_I2C_ReceiverOverflowHasOccurred(I2C_ID_2))  
    {
        PLIB_I2C_ReceiverOverflowClear(I2C_ID_2); 
        return false;
    }
    
    /* Check for transmit overflow */
    if (PLIB_I2C_TransmitterOverflowHasOccurred(I2C_ID_2))
    {
        PLIB_I2C_TransmitterOverflowClear(I2C_ID_2);
        return false;
    }
    
    /* Issue restart */
    PLIB_I2C_MasterStartRepeat(I2C_ID_2); 
       
    return true;    
}

bool DRV_I2C0_MasterStop(void)
{
    /* Check for transmit overflow */
    if (PLIB_I2C_TransmitterOverflowHasOccurred(I2C_ID_2))
    {
        PLIB_I2C_TransmitterOverflowClear(I2C_ID_2);
        return false;
    }
    
    /* Issue stop */
    PLIB_I2C_MasterStop(I2C_ID_2);  

#if __PIC32MZ
    while (!PLIB_I2C_StopWasDetected(I2C_ID_2));
    PLIB_I2C_Disable(I2C_ID_2);
    PLIB_I2C_Enable(I2C_ID_2);
#endif	
    
    return true;
}

bool DRV_I2C0_WaitForStopComplete(void)
{
    while (!PLIB_I2C_StopWasDetected(I2C_ID_2));
		
    return true;
}

void DRV_I2C0_MasterACKSend(void)
{
    /* Check if receive is ready to ack */
    if ( PLIB_I2C_MasterReceiverReadyToAcknowledge(I2C_ID_2) )
    {
        PLIB_I2C_ReceivedByteAcknowledge (I2C_ID_2, true);
    }    

}

void DRV_I2C0_MasterNACKSend(void)
{
    /* Check if receive is ready to nack */
    if ( PLIB_I2C_MasterReceiverReadyToAcknowledge(I2C_ID_2) )
    {
       PLIB_I2C_ReceivedByteAcknowledge (I2C_ID_2, false);
    }    
}

bool DRV_I2C0_WaitForACKOrNACKComplete(void)
{
    /* Check for ACK/NACK to complete */
    while(PLIB_I2C_ReceiverByteAcknowledgeHasCompleted(I2C_ID_2));
	 
    return true;
}



#endif /* _TIME_H */
/*******************************************************************************
 End of File
 */


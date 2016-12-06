/*******************************************************************************
 System Tasks File

  File Name:
    system_tasks.c

  Summary:
    This file contains source code necessary to maintain system's polled state
    machines.

  Description:
    This file contains source code necessary to maintain system's polled state
    machines.  It implements the "SYS_Tasks" function that calls the individual
    "Tasks" functions for all the MPLAB Harmony modules in the system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    polled in the system.  These handles are passed into the individual module
    "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"
#include "app.h"
#include "app1.h"
#include "app2.h"
#include "app3.h"
#include "key.h"
#include "adc.h"
#include "buzzer.h"
#include "print.h"
// *****************************************************************************
// *****************************************************************************
// Section: Local Prototypes
// *****************************************************************************
// *****************************************************************************

void _SYS_Tasks(void);
void _APP_Tasks(void);
void _APP1_Tasks(void);
void _APP2_Tasks(void);
void _APP3_Tasks(void);
void _KEY_Tasks(void);
void _ADC_Tasks(void);
void _BUZZER_Tasks(void);
void _PRINT_Tasks(void);
void _TIME_Tasks(void);

// *****************************************************************************
// *****************************************************************************
// Section: System "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/

void SYS_Tasks ( void )
{
    /* Maintain the state machines of all library modules executing polled in
    the system. */

    /* create task for Sys Tasks */
    xTaskCreate((TaskFunction_t) _SYS_Tasks,
    "Sys Tasks",
    1024,
    NULL,
    0, 
    NULL);

    /* Maintain the application's state machine. */
    /* create task for App Tasks */
    xTaskCreate((TaskFunction_t) _APP_Tasks,
    "App Tasks",
    1024,
    NULL,
    1, 
    NULL);
    /* create task for App1 Tasks */
    xTaskCreate((TaskFunction_t) _APP1_Tasks,
    "App1 Tasks",
    1024,
    NULL,
    2, 
    NULL);
    
    /* create task for App2 Tasks */
    xTaskCreate((TaskFunction_t) _APP2_Tasks,
    "App2 Tasks",
    1024,
    NULL,
    3, 
    NULL);
    
    /* create task for App3 Tasks */
    xTaskCreate((TaskFunction_t) _APP3_Tasks,
    "App3 Tasks",
    1024,
    NULL,
    4,
    NULL);
    
    /* create task for Key Tasks */
    xTaskCreate((TaskFunction_t) _KEY_Tasks,
    "Key Tasks",
    1024,
    NULL,
   8,
    NULL);
    
    /* create task for Adc Tasks */
    xTaskCreate((TaskFunction_t) _ADC_Tasks,
    "Adc Tasks",
    1024,
    NULL,
   6,
    NULL);
    
    /* create task for Buzzer Tasks */
    xTaskCreate((TaskFunction_t) _BUZZER_Tasks,
    "Buzzer Tasks",
    1024,
    NULL,
    7,
    NULL);
    
    /* create task for Print Tasks */
    xTaskCreate((TaskFunction_t) _PRINT_Tasks,
    "Print Tasks",
    1024,
    NULL,
    3,
    NULL);
    
    /* create task for Time Tasks */
    xTaskCreate((TaskFunction_t) _TIME_Tasks,
    "Time Tasks",
    1024,
    NULL,
    9,
    NULL);
    

    /* Start multitasking (i.e. give control to RTOS). This function never returns */
    vTaskStartScheduler();
    
}

void _SYS_Tasks(void)
{
    while(1)
    {
    /* Maintain system services */
    /* Maintain the DEVCON system state machine. */
    SYS_DEVCON_Tasks(sysObj.sysDevcon);

    /* Maintain Device Drivers */
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void _APP_Tasks(void)
{
    while(1)
    {
        APP_Tasks();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
void _APP1_Tasks(void)
{
    while(1)
    {
        APP1_Tasks();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
void _APP2_Tasks(void)
{
    while(1)
    {
        APP2_Tasks();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
void _APP3_Tasks(void)
{
    while(1)
    {
        APP3_Tasks();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
void _KEY_Tasks(void)
{
    while(1)
    {
        KEY_Tasks();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
void _ADC_Tasks(void)
{
    while(1)
    {
        ADC_Tasks();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
void _BUZZER_Tasks(void)
{
    while(1)
    {
        BUZZER_Tasks();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
void _PRINT_Tasks(void)
{
    while(1)
    {
        PRINT_Tasks();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
void _TIME_Tasks(void)
{
    while(1)
    {
        TIME_Tasks();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}


/*******************************************************************************
 End of File
 */


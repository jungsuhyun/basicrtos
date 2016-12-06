
/* Key.c */

#include "key.h"
#include "system_definitions.h"
#include "queue.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"


#define X1SET  PLIB_PORTS_PinSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_7)
#define X2SET PLIB_PORTS_PinSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_6)
#define X3SET  PLIB_PORTS_PinSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_5)
#define X4SET PLIB_PORTS_PinSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_4)
#define X5SET PLIB_PORTS_PinSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_13)
#define X1CLEAR PLIB_PORTS_PinClear(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_7)
#define X2CLEAR PLIB_PORTS_PinClear(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_6)
#define X3CLEAR PLIB_PORTS_PinClear(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_5)
#define X4CLEAR PLIB_PORTS_PinClear(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_4)
#define X5CLEAR PLIB_PORTS_PinClear(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_13)
#define Y1GET PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_12)
#define Y2GET PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_3)
#define Y3GET PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_2)
#define Y4GET PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_1)
#define Y5GET PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_C,PORTS_BIT_POS_14)

#define X1KEY 0
#define X2KEY 4
#define X3KEY 8
#define X4KEY 12
#define X5KEY 16
#define Y1KEY 0
#define Y2KEY 1
#define Y3KEY 2
#define Y4KEY 3

#define QUEUE_SEND_FREQUENCY_MS         ( 200 / portTICK_PERIOD_MS )

#define QUEUE_LENGTH                    ( 1 )


KEY_DATA keyData;
/* The queue used by both tasks. */
QueueHandle_t xQueue = NULL;
QueueHandle_t xPrintQueue;

typedef struct {
	void *buf;  //pointer 
	size_t sz;  //size
} QPacket;


const unsigned long ulValueToSend1 = 1UL;
const unsigned long ulValueToSend2 = 10UL;
const unsigned long ulValueToSend3 = 100UL;
const unsigned long ulValueToSend4 = 1000UL;
const unsigned long ulValueToSend5 = 10000UL;





void KEY_Initialize ( void )
{
    /* X1 ~ X5 OutputSet */
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_7);   
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_6);    
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_5);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_4);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_13);
    
    /* Y1 ~ Y5 InputSet */
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_12);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_3);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_2);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_1);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0,PORT_CHANNEL_C,PORTS_BIT_POS_14);
    /* Create the queue. */
    xQueue = xQueueCreate( 1, sizeof( unsigned long ) );
    
}

//unsigned int keycode=0;
int keycode=0;
int step=0;
int i; int a; int num[20];
char buffer[100];
void KEY_Tasks ( void )
{
    
    switch(step){
        case 0 : 
            /* F1 ~ F4 set */
            if(Y1GET) keycode +=1 << (X1KEY+Y1KEY); 
            if(Y2GET) keycode +=1 << (X1KEY+Y2KEY); 
            if(Y3GET) keycode +=1 << (X1KEY+Y3KEY); 
            if(Y4GET) keycode +=1 << (X1KEY+Y4KEY);
            X2SET;X1CLEAR;X3CLEAR;X4CLEAR;X5CLEAR;
            break;
        case 1 : 
            /* user ~ hold */
            if(Y1GET) keycode +=1 << (X2KEY+Y1KEY);
            if(Y2GET) keycode +=1 << (X2KEY+Y2KEY);
            if(Y3GET) keycode +=1 << (X2KEY+Y3KEY);
            if(Y4GET) keycode +=1 << (X2KEY+Y4KEY);
            X3SET;X1CLEAR;X2CLEAR;X4CLEAR;X5CLEAR;
            break;
        case 2 : 
            /* up ~ right */
            if(Y1GET) keycode +=1 << (X3KEY+Y1KEY);
            if(Y2GET) keycode +=1 << (X3KEY+Y2KEY);
            if(Y3GET) keycode +=1 << (X3KEY+Y3KEY);
            if(Y4GET) keycode +=1 << (X3KEY+Y4KEY);
            X4SET;X1CLEAR;X2CLEAR;X3CLEAR;X5CLEAR;
            break;
        case 3 : 
            /* power ~ currunt */
            if(Y1GET) keycode +=1 << (X4KEY+Y1KEY);
            if(Y2GET) keycode +=1 << (X4KEY+Y2KEY);
            if(Y3GET) keycode +=1 << (X4KEY+Y3KEY); 
            if(Y4GET) keycode +=1 << (X4KEY+Y4KEY); 
            X5SET;X1CLEAR;X2CLEAR;X3CLEAR;X4CLEAR;
            break;
        case 4 : 
            /* wave ~ trend */
            if(Y1GET) keycode +=1 << (X5KEY+Y1KEY); 
            if(Y2GET) keycode +=1 << (X5KEY+Y2KEY); 
            if(Y3GET) keycode +=1 << (X5KEY+Y3KEY); 
            if(Y4GET) keycode +=1 << (X5KEY+Y4KEY); 
            X1SET;X2CLEAR;X3CLEAR;X4CLEAR;X5CLEAR;
            if(keycode != 0 ) {
                /* keycode output */
//                printf("KEYCODE = %d\r\n",keycode);
                
                /* queue send */
//                xQueueSend( xPrintQueue, &keycode, 0U );
                
                /* string print */
//                itoa(buffer, keycode, 10);
//                printf("%s\n",buffer);
                
                /* string queue */
//                char   *ptr_dest = (char *)pvPortMalloc( 50);
//                 itoa(ptr_dest, keycode, 10);                
////                 printf("s %s\n",ptr_dest);
//                xQueueSend( xPrintQueue, &ptr_dest, 0U );
                
//                 char   *ptr_dest ;
//                ptr_dest= pvPortMalloc( 50);
//                 itoa(ptr_dest, keycode, 10);                
//                xQueueSend( xPrintQueue, &ptr_dest, 0U );
                
                 /* string size queue */
//               itoa(buffer, keycode, 10);
//                int size = strlen(buffer)+1;
//                char   *ptr_dest = (char *)pvPortMalloc(size);
//                memset(ptr_dest, 0, size+1);  //reset 
//                itoa(ptr_dest, keycode, 10);         //memory assignment      
//                xQueueSend( xPrintQueue, &ptr_dest, 0 );
                
                /* string struct queue */
                itoa(buffer, keycode, 10);
                long size = strlen(buffer)+1;
//                printf("%d\n",size);
                QPacket pk; //qpacket declaration 
                pk.sz = size;  //qpacket size
                pk.buf = (void*) pvPortMalloc(size+1);  //memory assignment
                memset(pk.buf, 0, size+1); //reset
                memcpy(pk.buf, buffer, size+1);
//                printf("%s\n",pk);
                xQueueSend( xPrintQueue, &pk, 0 );
                
                /* buzzer by keycode */
//                if(keycode <15 ) xQueueSend( xQueue, &ulValueToSend1, 0U );
//                else if(keycode < 512) xQueueSend( xQueue, &ulValueToSend2, 0U );
//                else if(keycode < 4096)xQueueSend( xQueue, &ulValueToSend3, 0U );
//                else xQueueSend( xQueue, &ulValueToSend4, 0U );
                
                /* buzzer by Multiplier of 2 */
//                a=0;
//                for(i=0;i<20;i++){
//                    num[i]=pow(2,i);
//                    if(num[i] == keycode) {
//                        a=1; break;
//                    }
//                }
//                if(a) xQueueSend( xQueue, &ulValueToSend1, 0U );
//                else xQueueSend( xQueue, &ulValueToSend2, 0U );
                if(keycode == 1) xQueueSend( xQueue, &ulValueToSend1, 0U );
                else if(keycode == 2) xQueueSend( xQueue, &ulValueToSend2, 0U );
                else if(keycode == 4) xQueueSend( xQueue, &ulValueToSend3, 0U );
                else if(keycode == 8) xQueueSend( xQueue, &ulValueToSend4, 0U );
                else xQueueSend( xQueue, &ulValueToSend5, 0U );
                vTaskDelay(QUEUE_SEND_FREQUENCY_MS );
            }
            keycode=0;
            break;
    }
    step++;  
    if(step==5) step=0;
}

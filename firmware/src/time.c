/* time */

#include "time.h"
#include "driver/i2c/drv_i2c.h"
/* Standard includes. */
#include <stdio.h>
#include <xc.h>


/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stdint.h"


#define MAX_NUM_OF_BYTES            8
#define MAX_NUM_OF_BYTES_IN_BUF (MAX_NUM_OF_BYTES + 4)


#define RTC_I2C_SLAVE_ADDRESS 0b11011110

#define  ADDR_SEC          0x00       //  address of SECONDS      register
#define  ADDR_MIN          0x01       //  address of MINUTES      register
#define  ADDR_HOUR         0x02       //  address of HOURS        register
#define  ADDR_DAY          0x03       //  address of DAY OF WK    register
#define  ADDR_STAT         0x03       //  address of STATUS       register
#define  ADDR_DATE         0x04       //  address of DATE         register
#define  ADDR_MNTH         0x05       //  address of MONTH        register
#define  ADDR_YEAR         0x06       //  address of YEAR         register
////////////////////////////////////////////////////////////////////////////////

#define  START_32KHZ       0x80 
#define SEND_FREQUENCY_MS         ( 900 / portTICK_PERIOD_MS )

uint8_t drvI2CTXbuffer[MAX_NUM_OF_BYTES_IN_BUF];
uint8_t drvI2CRXbuffer[MAX_NUM_OF_BYTES_IN_BUF];
/* address conversion*/
static char bcd_hex(char bcd)
{
	char i;

	i = (bcd >> 4);
	i = (i * 10);
	i = (i + (bcd & 0x0f));
	return(i);
} 

static char hex_bcd(char hex)
{	
	return (hex / 10) << 4 | (hex % 10);
}

typedef enum {
	/* Master initiates a stop condition */
	I2C_STATE_IDLE,

	/* Master initiates a start condition */
	I2C_STATE_SEND_START_CONDITION,

	/* Master sends out an address byte */
	I2C_STATE_SEND_ADDRESS_BYTE,

	/* Master sends data to a slave */
	I2C_STATE_SEND_DATA,

	/* Master Send Start condition again to Read */
	I2C_SEND_PAGE_MARKER_START_CONDITON,

	/* Master Send Start Address */
	I2C_SEND_PAGE_MARKER_DEVICE_ADDRESS,

	/* Send Page address to mark for read */
	I2C_SEND_PAGE_ADDRESS,

	/* Master Send Start condition again to Read */
	I2C_READ_SEND_START_CONDITON,

	/*Master sends Read address byte to slave */
	I2C_STATE_SEND_READ_ADDRESS_BYTE,

	/*Master Reads Data from Slave*/
	I2C_STATE_READ_DATA,

	/* Stop condition for Read */
	I2C_STATE_READ_STOP_CONDITION,

	/* Master initiates a stop condition */
	I2C_STATE_SEND_STOP_CONDITION,

	I2C_OPERATION_COMPLETE

} I2C_STATES;

static I2C_STATES state;

static unsigned char byteCounter = 0;

static void RtcI2cWrite(uint8_t* txbuffer, uint32_t num_of_bytes)
{
	state = I2C_STATE_IDLE;
	while (1) {
        switch (state) {
        case I2C_STATE_IDLE:
            state = I2C_STATE_SEND_START_CONDITION;
            break;
        case I2C_STATE_SEND_START_CONDITION:
            DRV_I2C0_MasterStart();
            DRV_I2C0_WaitForStartComplete();
            state = I2C_STATE_SEND_ADDRESS_BYTE;

            break;

        case I2C_STATE_SEND_ADDRESS_BYTE:
            if (DRV_I2C0_ByteWrite(RTC_I2C_SLAVE_ADDRESS)) {
                DRV_I2C0_WaitForByteWriteToComplete();
                if (DRV_I2C0_WriteByteAcknowledged())
                    state = I2C_STATE_SEND_DATA;
                else
                    state = I2C_OPERATION_COMPLETE;
            } else
                state = I2C_STATE_SEND_ADDRESS_BYTE;
            break;

        case I2C_STATE_SEND_DATA:
            /* Total bytes send = page address + data */
            for (byteCounter = 0; byteCounter < num_of_bytes; byteCounter++) {   
                DRV_I2C0_ByteWrite(*txbuffer);  //data send
                vTaskDelay(1);                 
                txbuffer++;   
            }
            
            DRV_I2C0_WaitForByteWriteToComplete();
            state = I2C_STATE_SEND_STOP_CONDITION;
            break;

        case I2C_STATE_SEND_STOP_CONDITION:
            DRV_I2C0_MasterStop();
            DRV_I2C0_WaitForStopComplete();
            state = I2C_OPERATION_COMPLETE;
            break;
        case I2C_OPERATION_COMPLETE:
//            DRV_I2C0_DeInitialize();
            return;
            break;
        default:
            while (1);
			
		}
	}
}

static void RtcI2cRead(unsigned char addr, uint8_t *rxbuffer, uint32_t num_of_bytes)
{
	state = I2C_STATE_IDLE;
	while (1) {
        switch (state) {  //true -> master stste 
        case I2C_STATE_IDLE:
            state = I2C_SEND_PAGE_MARKER_START_CONDITON;
            break;
        case I2C_SEND_PAGE_MARKER_START_CONDITON:
            DRV_I2C0_MasterStart();   //master state start
            DRV_I2C0_WaitForStartComplete();
            state = I2C_SEND_PAGE_MARKER_DEVICE_ADDRESS;
            break;

        case I2C_SEND_PAGE_MARKER_DEVICE_ADDRESS:
            vTaskDelay(1);  //delay
            if (DRV_I2C0_ByteWrite(RTC_I2C_SLAVE_ADDRESS)) {  //0b11011110 address write
                DRV_I2C0_WaitForByteWriteToComplete();
                if (DRV_I2C0_WriteByteAcknowledged())  //byte transmitter
                    state = I2C_SEND_PAGE_ADDRESS;
                else
                    state = I2C_OPERATION_COMPLETE;
            } else
                state = I2C_SEND_PAGE_MARKER_DEVICE_ADDRESS;
            break;

        case I2C_SEND_PAGE_ADDRESS:
            DRV_I2C0_ByteWrite(addr);   //address transmitter
            DRV_I2C0_WaitForByteWriteToComplete();
            while (!DRV_I2C0_WriteByteAcknowledged);
            state = I2C_READ_SEND_START_CONDITON;
            break;

        case I2C_READ_SEND_START_CONDITON: 
            DRV_I2C0_MasterRestart();
            DRV_I2C0_WaitForStartComplete();
            state = I2C_STATE_SEND_READ_ADDRESS_BYTE;
            break;

        case I2C_STATE_SEND_READ_ADDRESS_BYTE:
            vTaskDelay(1);
            if (DRV_I2C0_ByteWrite((RTC_I2C_SLAVE_ADDRESS | 0x01))) { 
                DRV_I2C0_WaitForByteWriteToComplete();
                if (DRV_I2C0_WriteByteAcknowledged())
                    state = I2C_STATE_SEND_DATA;
                else
                    state = I2C_OPERATION_COMPLETE;
            } else
                state = I2C_STATE_SEND_READ_ADDRESS_BYTE;
            state = I2C_STATE_READ_DATA;
            break;
        case I2C_STATE_READ_DATA:
            while (num_of_bytes > 0) {
                DRV_I2C0_SetUpByteRead(); //Set Rx enable in MSTR which causes SLAVE to send data
                while (!DRV_I2C0_WaitForReadByteAvailable()); // Wait till RBF = 1; Which means data is available in I2C2RCV reg
                *rxbuffer = DRV_I2C0_ByteRead(); //Read from I2CxRCV
                if (num_of_bytes > 1) {
                    DRV_I2C0_MasterACKSend(); //Send ACK to Slave
                }
                while (!DRV_I2C0_WaitForACKOrNACKComplete());
                num_of_bytes--;
                rxbuffer++;
            }
            DRV_I2C0_MasterNACKSend(); //last byte; send NACK to Slave, no more data needed
            while (!DRV_I2C0_WaitForACKOrNACKComplete()); // wait till NACK sequence is complete i.e ACKEN = 0
            state = I2C_STATE_READ_STOP_CONDITION;
            break;
        case I2C_STATE_READ_STOP_CONDITION:
            DRV_I2C0_MasterStop();
            DRV_I2C0_WaitForStopComplete();
            state = I2C_OPERATION_COMPLETE;
            break;
        case I2C_OPERATION_COMPLETE:
            //            DRV_I2C0_DeInitialize();
            return;
            break;
        
				while (1);
        }
		}
}


static char RtcRead(unsigned char rtcc_reg)
{
	uint32_t num_of_bytes;
	/* Add the buffer to get the data from EEPROM*/
	num_of_bytes = 1;
	RtcI2cRead(rtcc_reg, &drvI2CRXbuffer[0], num_of_bytes);
	return drvI2CRXbuffer[0];
}

static void RtcWrite(unsigned char rtcc_reg, unsigned char time_var)
{
	uint32_t num_of_bytes;

	drvI2CTXbuffer[0] = rtcc_reg;
	drvI2CTXbuffer[1] = time_var;

	/* Number of bytes to transfer */
	num_of_bytes = 2;

	/* Add to the write buffer to transmit*/
	RtcI2cWrite(&drvI2CTXbuffer[0], num_of_bytes);
}
////////////////////////////////////////////////////////////////////////////////

void TIME_Initialize ( void )
{
   
    PLIB_I2C_BaudRateSet(I2C_ID_2, SYS_CLK_PeripheralFrequencyGet(CLK_BUS_PERIPHERAL_1), 400000);
    PLIB_I2C_StopInIdleDisable(I2C_ID_2);

    /* Low frequency is enabled (**NOTE** PLIB function revereted) */
    PLIB_I2C_HighFrequencyEnable(I2C_ID_2);

    /* Enable I2C0 */
    PLIB_I2C_Enable(I2C_ID_2);
}


/******************************************************************************
  Function:
    void TIME_Tasks ( void )

  Remarks:
    See prototype in time.h.
 */


void TIME_Tasks (void)
{

    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_4);

    DEV_TIME dt;
    char yy, mm, dd, h, m, s;
	unsigned char preSec = 61;
    
    TickType_t xLastWakeTime;
	TickType_t xLastWakeTime1m;

	xLastWakeTime = xTaskGetTickCount();
	xLastWakeTime1m = xTaskGetTickCount();
    
    /* initialize yy,mm,dd,h,m,s */
//    RtcWrite(ADDR_YEAR, hex_bcd(16)); 
//    RtcWrite(ADDR_MNTH, hex_bcd(12)); 
//    RtcWrite(ADDR_DATE, hex_bcd(06));
//    RtcWrite(ADDR_HOUR, hex_bcd(14));
//    RtcWrite(ADDR_MIN, hex_bcd(17));
//    RtcWrite(ADDR_SEC, START_32KHZ);
    
    s = bcd_hex(RtcRead(ADDR_SEC)&0x7F);
	m = bcd_hex(RtcRead(ADDR_MIN));
	h = bcd_hex(RtcRead(ADDR_HOUR));
	yy = bcd_hex(RtcRead(ADDR_YEAR));
	mm = bcd_hex(RtcRead(ADDR_MNTH)&0x1F);
	dd = bcd_hex(RtcRead(ADDR_DATE));

	dt.yy = yy;
	dt.mm = mm;
	dt.dd = dd;
	dt.h = h;
	dt.m = m;
	dt.s = s;
    dt.ms = xLastWakeTime;
    preSec = s;
    
    while(1){
    
        s = bcd_hex(RtcRead(ADDR_SEC)&0x7F);
            if (s == 0) {
                m = bcd_hex(RtcRead(ADDR_MIN));
                if (m == 0) {
                    h = bcd_hex(RtcRead(ADDR_HOUR));
                    if (h == 0) {
                        yy = bcd_hex(RtcRead(ADDR_YEAR));
                        mm = bcd_hex(RtcRead(ADDR_MNTH)&0x1F);
                        dd = bcd_hex(RtcRead(ADDR_DATE));
                    }
                }
            }

        dt.yy = yy;
        dt.mm = mm;
        dt.dd = dd;
        dt.h = h;
        dt.m = m;
        dt.s = s;
        dt.ms = xLastWakeTime;
        preSec = s;
        
        printf("\n20%d - %d - %d  %d : %d : %d\n",dt.yy,dt.mm,dt.dd,dt.h,dt.m,dt.s);
//        printf("\n%d\n",dt);
        vTaskDelay(SEND_FREQUENCY_MS);
        }

 

 
}
/*******************************************************************************
 End of File
 */

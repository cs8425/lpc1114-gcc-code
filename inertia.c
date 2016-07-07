// accel.c
// API implementation for the MPU6050 accelerometer/gyro on the I2C bus
#include <stdint.h>
#include "inertia.h"
#include "lpc111x.h"
#include "serial.h"
#define MAX_TRACE 20
int debug_trace_data[MAX_TRACE];
volatile int debug_count=0;
void debug_trace(int value)
{	
	if (debug_count < MAX_TRACE)
		debug_trace_data[debug_count++]=value;
}
void dump_debug_trace(void)
{
	int i=0;
	while (i < MAX_TRACE) {
		printByte(debug_trace_data[i++]);
		printString(" ");	
	}
}
void reset_debug_trace(void)
{
	int i=0;
	while(i < MAX_TRACE) {
		debug_trace_data[i++]=0;
	}
	debug_count = 0;
}
#define MAX_I2CDATA 4
volatile typedef struct 
{
	int RegisterNumber;
	char Mode; // 'r' = read, 'w' = write
	int Count;  // how many bytes are involved
	int Index;  // Index of next byte
	int Status; // Current status of I2C peripheral
	int Busy;
	uint8_t Data[MAX_I2CDATA]; // Data transfer area

} I2CDCB_t;
static I2CDCB_t I2CDCB;
void initI2C(void)
{

	// Allocate the I2C pins
	// PIO0_5 : SDA
	// PIO0_4 : SCL
	// set up the I2C bus 
	SYSAHBCLKCTRL |= BIT16; // turn on IOCON clock
	SYSAHBCLKCTRL |= BIT6;  // turn on GPIO	
	SYSAHBCLKCTRL |= BIT5;  // turn on I2C clock
	PRESETCTRL &= ~BIT1; 	// put I2C into reset
	IOCON_PIO0_5 = BIT0;	// Make pin behave as SDA (open drain)
	IOCON_PIO0_4 = BIT0; 	// Make pin behave as SCL (open drain)
	PRESETCTRL |= BIT1; 	// take I2C out of reset
	// Assuming PCLK is 48MHz
	// 48MHz/(60+60) = 400kHz
	I2C0SCLL = 60;
	I2C0SCLH = 60; 
	I2C0CONSET = BIT6;		// enable I2C
	// enable I2C interrupt handling
	ISER |= BIT15;
}
void I2C_isr(void)
{ // only processing master mode states
// See page 271 of reference manual

	I2CDCB.Status = I2C0STAT;
	
	debug_trace(I2CDCB.Status);
	
	switch (I2CDCB.Status) {
		case 0x08 :
		{
			//"Start tx" send;
			// This is a read operation , need to send the
			// register number with r/w set to 0 (write)
			I2C0DAT = MPU_6050_ADDRESS + 0;
			// set the AA bit
			I2C0CONSET = BIT2;
			// clear SI bit
			I2C0CONCLR = BIT3;			
			break;
		}
		case 0x10 : 
		{
			// "Rep start tx" sent
			// Write slave address with read flag to data register
			I2C0DAT = MPU_6050_ADDRESS + 1;	
			// set the AA bit
			I2C0CONSET = BIT2;
			// clear SI bit
			I2C0CONCLR = BIT3+BIT5;
			break;
		}
		case 0x18 :
		{

			// "SLA+W tx ACK rx"
			// Now need to write the register number we are interested in
			I2C0DAT = I2CDCB.RegisterNumber;
			// set the AA bit
			I2C0CONSET = BIT2;
			// clear SI bit
			I2C0CONCLR = BIT3+BIT5;
			break;
		}
		case 0x20 :
		{
			// "SLA+W tx NAK rx"
			I2C0CONSET = BIT4+BIT2; // set STOP and AA bits
			// clear SI bit
			I2C0CONCLR = BIT3;
			I2CDCB.Busy = 0;
			break;
		}
		case 0x28 :
		{

 			if (I2CDCB.Mode == 'r')
            {
				// "Data tx ACK rx"
				I2C0CONSET = BIT5+BIT2; // set START and AA bits (repeated start)
				I2C0CONCLR = BIT3;	    // clear SI flag
				break;
			}
			else
			{
				if (I2CDCB.Count)
				{
					I2C0DAT = I2CDCB.Data[0];
					I2CDCB.Count--;
				}
				else
				{
					I2C0CONSET = BIT4; // send STOP					
					I2CDCB.Busy = 0;
				}
				I2C0CONCLR = BIT3; // Clear SI Flag
				break;
			}
		}
		case 0x30 :
		{
			// "Data tx NAK rx"
			I2C0CONSET = BIT4+BIT2; // set STOP and AA bits
			I2C0CONCLR = BIT3;
			I2CDCB.Busy = 0;
			break;
		}
		case 0x38 :
		{
			// "Arb lost"
			//dump_debug_trace();
			//printString("!\r\n");
			I2C0CONSET = BIT5+BIT2; // set STA and AA bits						
			I2C0CONCLR = BIT3;
			break;
		}
		case 0x40 :
		{
			// "SLA+R tx, ACK rx"
			I2C0CONSET = BIT2; // set AA
			I2C0CONCLR = BIT3; // clear SI flag
			break;
		}
		case 0x48 :
		{
			// "SLA+R tx, NAK rx"
			I2C0CONSET = BIT4+BIT2; // set STOP and AA bits
			// clear SI bit
			I2C0CONCLR = BIT3;
			I2CDCB.Busy = 0;
			break;
		}
		case 0x50 :
		{
			// "Data rx, ACK tx"
			I2CDCB.Data[I2CDCB.Index++]=I2C0DAT;
			I2C0CONCLR = BIT3+BIT2; // clear SI and send NAK to terminate transaction			
			break;
		}
		case 0x58 :
		{
			// "Data rx, NAK tx"			
			I2CDCB.Data[I2CDCB.Index++]=I2C0DAT;
			// set STO bit	(end the transaction)
			I2C0CONSET = BIT4+BIT2; // set STOP and AA bits
			// clear SI bit
			I2C0CONCLR = BIT3;
			I2CDCB.Busy = 0;
			break;
		}
		default : 
		{
			// error: shouldn't get here!
			// TODO: Add retry count and reset I2C bus
			while (1) {
				dump_debug_trace();
				printString("\r\n");
				printString("I2C Master mode error! ");
				//delay(100000);
				printInteger(I2CDCB.Status);
				//delay(100000);
			}
		}
	}
}
int readRegister(int RegNum)
{
	unsigned Timeout=0xffff;
	I2CDCB.Mode='r';
	I2CDCB.RegisterNumber = RegNum;
	I2CDCB.Busy = 1;
	I2CDCB.Data[0] = -1;
	I2CDCB.Count = 1;
	I2CDCB.Index = 0;
	reset_debug_trace();
	// set STA bit
	I2C0CONSET = BIT5;
	while ( (I2CDCB.Busy != 0) && (Timeout--));
	if (I2CDCB.Busy == 1) {
		// something has gone wrong (read timeout)
		//dump_debug_trace();
		//printString("\r\n");
		//initI2C();
		//enable_interrupts();
		//powerUpAccel();
		return -1;
	} else {
		return I2CDCB.Data[0];
	}

}
int writeRegister(int RegNum, int Data)
{
	unsigned Timeout=0xffff;
	I2CDCB.Mode='w';
	I2CDCB.RegisterNumber = RegNum;
	I2CDCB.Busy = 1;
	I2CDCB.Count = 1;
	I2CDCB.Index = 0;
    I2CDCB.Data[0] = Data;
    reset_debug_trace();
    // set STA bit
	I2C0CONSET = BIT5;
	while ( (I2CDCB.Busy != 0) && (Timeout--));
	if (I2CDCB.Busy == 1) {// timeout occurred
		dump_debug_trace();
		printString(":(\r\n");
		return -1;
	} else {			
		return 0;
	}
}
int initAccel(void)
{
	initI2C();
	enable_interrupts();
	// Make sure the MPU6050 is awake by reading its WHO_AM_I register	
	if (readRegister(WHO_AM_I) == 0x68) {
		powerUpAccel();
		return 0; 	// ok
	} else {
		return -1; 	// device not found
	}
}
int getMotion(motion *m)
{	
	m->x_a = (readRegister(ACCEL_XOUT_H) << 8) + readRegister(ACCEL_XOUT_L);	
	m->y_a = (readRegister(ACCEL_YOUT_H) << 8) + readRegister(ACCEL_YOUT_L);
	m->z_a = (readRegister(ACCEL_ZOUT_H) << 8) + readRegister(ACCEL_ZOUT_L);
	m->x_g = (readRegister(GYRO_XOUT_H) << 8) + readRegister(GYRO_XOUT_L);
	m->y_g = (readRegister(GYRO_YOUT_H) << 8) + readRegister(GYRO_YOUT_L);
	m->z_g = (readRegister(GYRO_ZOUT_H) << 8) + readRegister(GYRO_ZOUT_L);			
	//delay(600); // wait for I2C bus traffic to finish	

	return 0; // ok
}
void powerUpAccel(void)
{
	writeRegister(PWR_MGMT_1,1);
	//delay(600);
}
int getTemperature(void)
{
	int16_t Temperature;					
	Temperature = (readRegister(TEMP_OUT_H)<<8)+readRegister(TEMP_OUT_L);			
	Temperature = Temperature / (int16_t)340;
	Temperature = Temperature + (int16_t)37;
	
	//delay(600); // wait for I2C bus traffic to finish
	
	return (int)Temperature;
}

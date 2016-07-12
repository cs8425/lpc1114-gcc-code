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

/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define I2C_CONSET       (I2C0CONSET)
#define I2C_CONCLR       (I2C0CONCLR)
#define I2C_STAT         (I2C0STAT)
#define I2C_DAT          (I2C0DAT)
#define I2C_SCLL(val)    (I2C0SCLL = val)
#define I2C_SCLH(val)    (I2C0SCLH = val)
/*
static inline void i2c_conclr(int start, int stop, int interrupt, int acknowledge) {
    I2C_CONCLR = (start << 5)
                    | (stop << 4)
                    | (interrupt << 3)
                    | (acknowledge << 2);
}

static inline void i2c_conset(int start, int stop, int interrupt, int acknowledge) {
    I2C_CONSET = (start << 5)
                    | (stop << 4)
                    | (interrupt << 3)
                    | (acknowledge << 2);
}

// Clear the Serial Interrupt (SI)
static inline void i2c_clear_SI() {
    i2c_conclr(obj, 0, 0, 1, 0);
}

static inline int i2c_status() {
    return I2C_STAT;
}

// Wait until the Serial Interrupt (SI) is set
static int i2c_wait_SI() {
    int timeout = 0;
    while (!(I2C_CONSET & (1 << 3))) {
        timeout++;
        if (timeout > 100000) return -1;
    }
    return 0;
}

static inline void i2c_interface_enable() {
    I2C_CONSET = 0x40;
}

static inline void i2c_power_enable() {
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 5);
    LPC_SYSCON->PRESETCTRL |= 1 << 1;
}

void i2c_init() {
    // enable power
    i2c_power_enable(obj);
    
    // set default frequency at 400k
    i2c_frequency(400000);
    i2c_conclr(1, 1, 1, 1);
    i2c_interface_enable();
    
    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);
}

inline int i2c_start() {
    int status = 0;
    int isInterrupted = I2C_CONSET & (1 << 3);

    // 8.1 Before master mode can be entered, I2CON must be initialised to:
    //  - I2EN STA STO SI AA - -
    //  -  1    0   0   x  x - -
    // if AA = 0, it can't enter slave mode
    i2c_conclr(1, 1, 0, 1);

    // The master mode may now be entered by setting the STA bit
    // this will generate a start condition when the bus becomes free
    i2c_conset(, 1, 0, 0, 1);
    // Clearing SI bit when it wasn't set on entry can jump past state
    // 0x10 or 0x08 and erroneously send uninitialized slave address.
    if (isInterrupted)
        i2c_clear_SI();

    i2c_wait_SI();
    status = i2c_status();

    // Clear start bit now that it's transmitted
    i2c_conclr(1, 0, 0, 0);
    return status;
}

inline int i2c_stop(i2c_t *obj) {
    int timeout = 0;

    // write the stop bit
    i2c_conset(obj, 0, 1, 0, 0);
    i2c_clear_SI(obj);
    
    // wait for STO bit to reset
    while(I2C_CONSET(obj) & (1 << 4)) {
        timeout ++;
        if (timeout > 100000) return 1;
    }

    return 0;
}


static inline int i2c_do_write(i2c_t *obj, int value, uint8_t addr) {
    // write the data
    I2C_DAT(obj) = value;
    
    // clear SI to init a send
    i2c_clear_SI(obj);
    
    // wait and return status
    i2c_wait_SI(obj);
    return i2c_status(obj);
}

static inline int i2c_do_read(i2c_t *obj, int last) {
    // we are in state 0x40 (SLA+R tx'd) or 0x50 (data rx'd and ack)
    if (last) {
        i2c_conclr(obj, 0, 0, 0, 1); // send a NOT ACK
    } else {
        i2c_conset(obj, 0, 0, 0, 1); // send a ACK
    }
    
    // accept byte
    i2c_clear_SI(obj);
    
    // wait for it to arrive
    i2c_wait_SI(obj);
    
    // return the data
    return (I2C_DAT(obj) & 0xFF);
}

void i2c_frequency(i2c_t *obj, int hz) {
    // No peripheral clock divider on the M0
    uint32_t PCLK = SystemCoreClock;
    
    uint32_t pulse = PCLK / (hz * 2);
    
    // I2C Rate
    I2C_SCLL(obj, pulse);
    I2C_SCLH(obj, pulse);
}

// The I2C does a read or a write as a whole operation
// There are two types of error conditions it can encounter
//  1) it can not obtain the bus
//  2) it gets error responses at part of the transmission
//
// We tackle them as follows:
//  1) we retry until we get the bus. we could have a "timeout" if we can not get it
//      which basically turns it in to a 2)
//  2) on error, we use the standard error mechanisms to report/debug
//
// Therefore an I2C transaction should always complete. If it doesn't it is usually
// because something is setup wrong (e.g. wiring), and we don't need to programatically
// check for that

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {
    int count, status;
    
    status = i2c_start(obj);
    
    if ((status != 0x10) && (status != 0x08)) {
        i2c_stop(obj);
        return I2C_ERROR_BUS_BUSY;
    }
    
    status = i2c_do_write(obj, (address | 0x01), 1);
    if (status != 0x40) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }

    // Read in all except last byte
    for (count = 0; count < (length - 1); count++) {
        int value = i2c_do_read(obj, 0);
        status = i2c_status(obj);
        if (status != 0x50) {
            i2c_stop(obj);
            return count;
        }
        data[count] = (char) value;
    }

    // read in last byte
    int value = i2c_do_read(obj, 1);
    status = i2c_status(obj);
    if (status != 0x58) {
        i2c_stop(obj);
        return length - 1;
    }
    
    data[count] = (char) value;
    
    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
    }
    
    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {
    int i, status;
    
    status = i2c_start(obj);
    
    if ((status != 0x10) && (status != 0x08)) {
        i2c_stop(obj);
        return I2C_ERROR_BUS_BUSY;
    }
    
    status = i2c_do_write(obj, (address & 0xFE), 1);
    if (status != 0x18) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }
    
    for (i=0; i<length; i++) {
        status = i2c_do_write(obj, data[i], 0);
        if(status != 0x28) {
            i2c_stop(obj);
            return i;
        }
    }
    
    // clearing the serial interrupt here might cause an unintended rewrite of the last byte
    // see also issue report https://mbed.org/users/mbed_official/code/mbed/issues/1
    // i2c_clear_SI(obj);
    
    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
    }
    
    return length;
}

void i2c_reset(i2c_t *obj) {
    i2c_stop(obj);
}

int i2c_byte_read(i2c_t *obj, int last) {
    return (i2c_do_read(obj, last) & 0xFF);
}

int i2c_byte_write(i2c_t *obj, int data) {
    int ack;
    int status = i2c_do_write(obj, (data & 0xFF), 0);
    
    switch(status) {
        case 0x18: case 0x28:       // Master transmit ACKs
            ack = 1;
            break;
        case 0x40:                  // Master receive address transmitted ACK
            ack = 1;
            break;
        case 0xB8:                  // Slave transmit ACK
            ack = 1;
            break;
        default:
            ack = 0;
            break;
    }

    return ack;
}
*/

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

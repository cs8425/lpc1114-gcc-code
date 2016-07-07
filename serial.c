// Serial I/O routines
#include <stdint.h>
#include "serial.h"
#include "lpc111x.h"
SerialBuffer TXBuffer, RXBuffer;
void UART_isr(void);
void initUART()
{
	RXBuffer.count = RXBuffer.head = RXBuffer.tail = 0;
	TXBuffer.count = TXBuffer.head = TXBuffer.tail = 0;

	SYSAHBCLKCTRL |= BIT6 + BIT16; // Turn on clock for GPIO and IOCON
	// Enable UART RX function on PIO1_6
	IOCON_PIO1_6 |= BIT0; 
	IOCON_PIO1_6 &= ~(BIT1+BIT2);
	// Enable UART TX function on PIO1_7
	IOCON_PIO1_7 |= BIT0;
	IOCON_PIO1_7 &= ~(BIT1+BIT2);
	// Turn on clock for UART
	SYSAHBCLKCTRL |= BIT12;
	UARTCLKDIV = 1;
/*
	// PCLK = 48Mhz. Desired Baud rate = 9600
	// See table 199
	// 9600=48MHz/(16* (256*U0DLM + U0DLL)*(1+DivAddVal/MulVal))
	// 312.5 = (256*U0DLM+U0DLL)*(1+DivAddVal/MulVal)
	// let U0DLM=1, DivAddVal=0,MulVal =1
	// 312.5=256+U0DLL
	// U0DLL=56.5.  
	// Choose U0DLL=56.
	// Actual baud rate achieved = 9615 - close enough.
	U0LCR |= BIT7; // Enable divisor latch access
	U0FDR = (1<<4)+0; // Set DivAddVal = 0; MulVal = 1
	U0DLL = 56;
	U0DLM = 1;
	U0LCR &= ~BIT7; // Disable divisor latch access
*/
	UART_baud(115200);
	U0LCR |= (BIT1+BIT0); // set word length to 8 bits.
	U0IER = BIT0+BIT1+BIT2;	// Enable UART TX,RX Data  interrupts
	ISER |= BIT21; 			// enable UART IRQ's in NVIC was 13
}
// copy from mbed
// https://github.com/mbedmicro/mbed/blob/master/hal/targets/hal/TARGET_NXP/TARGET_LPC11XX_11CXX/serial_api.c
void UART_baud(int baudrate) {
    UARTCLKDIV = 0x1;
    uint32_t PCLK = 48000000;
    // First we check to see if the basic divide with no DivAddVal/MulVal
    // ratio gives us an integer result. If it does, we set DivAddVal = 0,
    // MulVal = 1. Otherwise, we search the valid ratio value range to find
    // the closest match. This could be more elegant, using search methods
    // and/or lookup tables, but the brute force method is not that much
    // slower, and is more maintainable.
    uint16_t DL = PCLK / (16 * baudrate);
    
    uint8_t DivAddVal = 0;
    uint8_t MulVal = 1;
    int hit = 0;
    uint16_t dlv;
    uint8_t mv, dav;
    if ((PCLK % (16 * baudrate)) != 0) {     // Checking for zero remainder
        int err_best = baudrate, b;
        for (mv = 1; mv < 16 && !hit; mv++)
        {
            for (dav = 0; dav < mv; dav++)
            {
                // baudrate = PCLK / (16 * dlv * (1 + (DivAdd / Mul))
                // solving for dlv, we get dlv = mul * PCLK / (16 * baudrate * (divadd + mul))
                // mul has 4 bits, PCLK has 27 so we have 1 bit headroom which can be used for rounding
                // for many values of mul and PCLK we have 2 or more bits of headroom which can be used to improve precision
                // note: X / 32 doesn't round correctly. Instead, we use ((X / 16) + 1) / 2 for correct rounding

                if ((mv * PCLK * 2) & 0x80000000) // 1 bit headroom
                    dlv = ((((2 * mv * PCLK) / (baudrate * (dav + mv))) / 16) + 1) / 2;
                else // 2 bits headroom, use more precision
                    dlv = ((((4 * mv * PCLK) / (baudrate * (dav + mv))) / 32) + 1) / 2;

                // datasheet says if DLL==DLM==0, then 1 is used instead since divide by zero is ungood
                if (dlv == 0)
                    dlv = 1;

                // datasheet says if dav > 0 then DL must be >= 2
                if ((dav > 0) && (dlv < 2))
                    dlv = 2;

                // integer rearrangement of the baudrate equation (with rounding)
                b = ((PCLK * mv / (dlv * (dav + mv) * 8)) + 1) / 2;

                // check to see how we went
				b = (b - baudrate);
                b = (b < 0)? -b : b;
                if (b < err_best)
                {
                    err_best  = b;

                    DL        = dlv;
                    MulVal    = mv;
                    DivAddVal = dav;

                    if (b == baudrate)
                    {
                        hit = 1;
                        break;
                    }
                }
            }
        }
    }
    
    // set LCR[DLAB] to enable writing to divider registers
    U0LCR |= (1 << 7);
    
    // set divider values
    U0DLM = (DL >> 8) & 0xFF;
    U0DLL = (DL >> 0) & 0xFF;
    U0FDR = (uint32_t) DivAddVal << 0
                   | (uint32_t) MulVal    << 4;
    
    // clear LCR[DLAB]
    U0LCR &= ~(1 << 7);
}
void UART_isr(void)
{
	int Source = U0IIR; // have to cache the interrupt id register
					  // as the 'if' clause below is reading it
					  // and hence changing its contents.
	if (Source & BIT2) { // RX Interrupt
		putBuf(&RXBuffer, U0RBR);
	}
	if (Source & BIT1) { // TX Interrupt
		if (TXBuffer.count > 0)
		{			
			U0THR = getBuf(&TXBuffer);
		}		
	}
}
void putBuf(SerialBuffer *sbuf, char c)
{
	if (sbuf->count < SBUFSIZE) {
		disable_interrupts();
		sbuf->count++;
		sbuf->buffer[sbuf->head] = c;
		sbuf->head=(sbuf->head+1) % SBUFSIZE;
		enable_interrupts();
	}
}
char getBuf(SerialBuffer *sbuf)
{
	char c=0;
	if (sbuf->count >0 ) {
		disable_interrupts();
		sbuf->count--;
		c=sbuf->buffer[sbuf->tail];
		sbuf->tail = (sbuf->tail+1) % SBUFSIZE;
		enable_interrupts();
	}
	return c;
}
int rx_count()
{
	return RXBuffer.count;
}
int tx_count()
{
	return TXBuffer.count;
}
void eputcOld(char c)
{
	if (U0LSR & BIT6) {
		U0THR = c; // Transmitter idle, so just write out directly
	}else {
		putBuf(&TXBuffer,c);
	}
}
void eputc(char c)
{
	putBuf(&TXBuffer,c);	
	if (U0LSR & BIT6) // If Transmitter idle, kickstart first write
		U0THR = getBuf(&TXBuffer); 		
}

char egetc()
{
	return getBuf(&RXBuffer);
}
void printString(char *String)
{
	int timeout=0x8000;
	while( (tx_count()>(SBUFSIZE/2)) && (timeout--));
	if (!timeout) {
		U0THR = 't';
		return;
	}
	while(*String) {
		eputc(*String);
		String++;
	}
	return;
}
char HexDigit(int Value)
{
	if ((Value >=0) && (Value < 10))
		return Value+'0';
	else if ((Value >9) && (Value < 16))
		return Value-10 + 'A';
	else
		return 'z';
}
void printInteger(unsigned int Number)
{
	// Output the number over the serial port as
	// as hexadecimal string.
	char TxString[9];
	int Index = 8;
	TxString[Index] = 0; // terminate the string
	Index--;
	while (Index >= 0) {
		TxString[Index] = HexDigit(Number & 0x0f);
		Number = Number >> 4;
		Index--;
	}
	printString(TxString);
}
void printShort(unsigned int Number)
{
	// Output the number over the serial port as
	// as hexadecimal string.
	char TxString[5];
	int Index = 4;
	TxString[Index] = 0; // terminate the string
	Index--;
	while(Index >= 0)
	{
		TxString[Index]=HexDigit(Number & 0x0f);
		Number = Number >> 4;
		Index--;
	}
	printString(TxString);
}
void printByte(unsigned int Number)
{
	// Output the number over the serial port as
	// as hexadecimal string.
	char TxString[3];
	int Index = 2;
	TxString[Index] = 0; // terminate the string
	Index--;
	while(Index >= 0)
	{
		TxString[Index] = HexDigit(Number & 0x0f);
		Number = Number >> 4;
		Index--;
	}
	printString(TxString);
}


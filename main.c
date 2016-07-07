#include "lpc111x.h"
#include "serial.h"
#include "inertia.h"
void SysTick(void)
{
	static uint32_t TickCounter=0;
	TickCounter++;
	if (TickCounter >= 1000) {
		TickCounter = 0;
	}
}
void initSysTick(void)
{
	
	// The systick timer is driven by a 48MHz clock
	// Divide this down to achieve a 1ms timebase
	// Divisor = 48MHz/1000Hz
	// Reload value = 48000-1
	// enable systick and its interrupts
	SYST_CSR |=(BIT0+BIT1+BIT2); 
	SYST_RVR=48000-1; // generate 1 millisecond time base
	SYST_CVR=5;
	enable_interrupts();
}
void delay(unsigned amount)
{
	while (amount--);
}
void ConfigPins(void)
{
	SYSAHBCLKCTRL |= BIT6 + BIT16; 	// Turn on clock for GPIO and IOCON 
	IOCON_PIO0_2 &= ~(BIT1+BIT0);  	// ensure Pin 25 behaves as GPIO
	GPIO0DIR |= BIT2;       		// Make Pin 25 an output	
	GPIO0DATA = 0;          		// 0 output initially
}
int RValue;
int Temperature;
motion m;
int main(void)
{
	
	ConfigPins();
	initUART();
	initSysTick();	
	RValue = initAccel();	
	printInteger(RValue);
	if (RValue == 0) {
		printString("Boldly going :)\r\n");
	} else {
		printString("Inertia sensors offline :(\r\n");
	}
	while (1) { 		
		Temperature = getTemperature();		
		printShort(Temperature);		
		printString(" ");					
		getMotion(&m);
		printShort(m.x_a);		
		printString(" ");
		printShort(m.y_a);		
		printString(" ");
		printShort(m.z_a);		
		printString(" ");		
		printString("\r\n");				
		GPIO0DATA ^= BIT2;				
		delay(10000);
	}

	return 0;
}

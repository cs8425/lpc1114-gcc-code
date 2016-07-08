#include "lpc111x.h"

#include "serial.h"
#include "inertia.h"

#include "config.h"

#include "hal.h"

#include "EventDriven.h"
#include "TimerEv.h"

uint8_t get_dip(void);
void set_led(uint8_t i);

void capture(void);
void dump(void);

#define CAM0 px0

#define CAM_value_MAX 65535
#define CAM_value_MIN 0

uint16_t px0[128];

void SysTick(void)
{
	static uint32_t TickCounter = 0;
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
	SYST_RVR = 48000-1; // generate 1 millisecond time base
	SYST_CVR = 5;
	enable_interrupts();
}
void delay(uint32_t amount)
{
	while (amount--) asm("nop");
}
void ConfigPins(void)
{
	SYSAHBCLKCTRL |= BIT6 + BIT16; 	// Turn on clock for GPIO, ADC, IOCON 
	IOCON_PIO0_2 &= ~(BIT1+BIT0);  	// ensure Pin 25 behaves as GPIO
	GPIO0DIR |= BIT2;       		// Make Pin 25 an output	
	GPIO0DATA = 0;          		// 0 output initially
}
int ReadADC(void)
{
	// Start a conversion
	AD0CR |= BIT1 + BIT24;
	// Wait for conversion to complete
	while((AD0GDR&BIT31)==0);
	// return the result
	return ((AD0GDR>>6)&0x3ff);
}
int RValue;
int Temperature;
motion m;

#define CAM          1
#define LED          2
#define DEBUG        3
#define Real_start   4
EventDriven eventloop;
TimerEv timerev;

int main(void){
	initUART();

	ADC_init();
	GPIO_init();
	PWM_init();

	initSysTick();

	us_ticker_init();

	EventDriven_init(&eventloop);
	TimerEv_init(&timerev, &eventloop);

	TimerEv_add(&timerev, LED, 500*1000, 1, 1);
	TimerEv_add(&timerev, CAM, 2.5*1000, 1, 1);
	delay(1000);
	TimerEv_add(&timerev, DEBUG, 125*1000, 1, 1);


	while (1) {
		//printShort(ADC_read());
		//printString("\r\n");
		TimerEv_tick(&timerev); // 週期性的事件推送
		switch(EventDriven_get(&eventloop)){ // 事件處理
			case CAM:
				capture();
				
			break;

			case DEBUG:
				dump();
			break;

			case LED:
				GPIO0DATA ^= BIT2;
			break;

		}



//		delay(6000000);
	}
/*	delay(100000);
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
		delay(6000000);
	}*/

	return 0;
}
/*uint8_t get_dip(){
    int n = (GET_DIP2) ? 1 : 0;
    n <<= 1;
    n |= (GET_DIP1) ? 1 : 0;
    n <<= 1;
    n |= (GET_DIP0) ? 1 : 0;
    return n;
}

void set_led(uint8_t i){
    if(i & 0x01){
        SET_LED0_HIGH;
    }else{
        SET_LED0_LOW;
    }
    if(i & 0x02){
        SET_LED1_HIGH;
    }else{
        SET_LED1_LOW;
    }
    if(i & 0x04){
        SET_LED2_HIGH;
    }else{
        SET_LED2_LOW;
    }
    if(i & 0x08){
        SET_LED3_HIGH;
    }else{
        SET_LED3_LOW;
    }
}
*/
void capture(){
	uint8_t i;

	__disable_irq();

	TAOS_SI_HIGH;
	TAOS_CLK_HIGH;
//	wait_us(1);
//	asm("nop;nop;nop;nop;");
	TAOS_SI_LOW;

	for(i = 0; i < 128; i++){
		CAM0[i] = ADC_read();
		TAOS_CLK_LOW;
//		wait_us(1);
//		asm("nop;nop;");
		TAOS_CLK_HIGH;
	}
	TAOS_CLK_LOW;

	__enable_irq();
}

inline void _p(const int data){
	eputc( ((data >> 12) & 0xF) + '0');
	eputc( ((data >> 8) & 0xF) + '0');
	eputc( ((data >> 4) & 0xF) + '0');
	eputc( (data & 0xF) + '0');
}
void dump(void){
	unsigned char i;

	eputc('!');
	eputc('c');
	for(i = 0; i < 128; i ++){
//		_p(MA_out[i]);
		_p(CAM0[i]);
	}
	eputc('\n');
}


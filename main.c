#include "lpc111x.h"

#include "serial.h"
#include "inertia.h"

#include "config.h"

#include "hal.h"

#include "fuzzysetupIntC.h"

#include "EventDriven.h"
#include "TimerEv.h"

#include "filter.h"
#include "ctrl.h"

uint8_t get_dip(void);
void set_led(uint8_t i);

void capture(void);
void dump(void);
void dump2(void);
void dump_edge(char h, edge *_E);


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

	//initSysTick();

	us_ticker_init();

	EventDriven_init(&eventloop);
	TimerEv_init(&timerev, &eventloop);

	TimerEv_add(&timerev, LED, 500*1000, 1, 1);
	TimerEv_add(&timerev, CAM, 2.5*1000, 1, 1);
	delay(1000);
	TimerEv_add(&timerev, DEBUG, 125*1000, 1, 1);

	edge_init(&edgeL2H);
	edge_init(&edgeH2L);

	FuzzySetup();

//	TFC_SetServo(2890);
//	while(1);

	while (1) {
		if(GET_SW1 == 0){
			TimerEv_add(&timerev, Real_start, 3000*1000, 0, 1);
			SET_LED3_HIGH;
		}

		TimerEv_tick(&timerev); // 週期性的事件推送
		switch(EventDriven_get(&eventloop)){ // 事件處理
			case CAM:

				capture();
				SET_LED1_HIGH;
				filt();
				MA();
				scale();
				cut();
				SET_LED1_LOW;

				M_FB = get_dip() * 500;
				SET_LED2_HIGH;
				toCtrl();
				SET_LED2_LOW;
			break;

			case Real_start:
				SET_LED3_LOW;
				isStart = 1;
			break;

			case DEBUG:
				//dump2();
			break;

			case LED:
				SET_LED0_T;
				//dump_edge('L', &edgeL2H);
				printShort(get_dip());
				eputc(',');
				eputc('0' + GET_SW0);
				eputc(',');
				eputc('0' + GET_SW1);
				eputc(',');
				printShort(debug_int1);
				eputc(',');
				printShort(debug_int2);
				eputc(',');
				printShort(debug_int3);
				printString("\n");

				//pc.printf("%d,%d:%f:%d[%d,%d]\n", debug_int, debug_int2, debug_f, get_dip(), GET_SW0, GET_SW1);
			break;

		}



//		delay(6000000);
	}
/*	delay(100000);
	int RValue;
	int Temperature;
	motion m;
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
		delay(6000000);
	}*/

	return 0;
}

uint8_t get_dip(){
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

inline void _p(const int16_t data){
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
//		_p(filt_out[i]);
		_p(CAM0[i]);
	}
	eputc('\n');
}
void dump2(void){
	unsigned char i;

	eputc('!');
	eputc('d');
	for(i = 0; i < 106; i ++){
		_p(bin_out[i]);
//		_p(MA_out[i]);
//		_p(MA_out_ABS[i]);
//		_p(filt_out[i]);
	}
	eputc('\n');
}
void dump_edge(char h, edge *_E){
	uint8_t i;
	uint8_t count = edge_get_count(_E);
	int16_t tmp;

	eputc('!');
	eputc(h);
	for(i = 0; i < count; i ++){
//		tmp = _E.get_R2L(i);
		tmp = edge_get_L2R(_E, i);
		eputc( ((tmp >> 4) & 0xF) + '0');
		eputc( (tmp & 0xF) + '0');
	}
	eputc('\n');
}



#ifndef __HAL_H
#define __HAL_H

#include "lpc111x.h"

// SI	P1_2
// CLK	P1_1
// Ao	P1_0

// SERVO	P0_11	CT32B0_MAT3

// A1	P1_10	CT16B1_MAT1
// A2	P0_10	CT16B0_MAT2
// B1	P0_9	CT16B0_MAT1
// B2	P0_8	CT16B0_MAT0

// SW0	P0_1
// SW1	P0_3

// DIP0	P0_2
// DIP1	P2_7
// DIP2	P2_8

// LED0	P2_4
// LED1	P2_5
// LED2	P0_6
// LED3	P0_7


// CLK P1_1
#define TAOS_CLK_HIGH  GPIO1DATA |= (1<<1)
#define TAOS_CLK_LOW   GPIO1DATA &= ~(1<<1)

// SI P1_2
#define TAOS_SI_HIGH   GPIO1DATA |= (1<<2)
#define TAOS_SI_LOW    GPIO1DATA &= ~(1<<2)


#define GET_DIP0    (GPIO0DATA & (1<<2))
#define GET_DIP1    (GPIO2DATA & (1<<7))
#define GET_DIP2    (GPIO2DATA & (1<<8))

#define GET_SW0    ((GPIO0DATA & (1<<1)) ? 1 : 0)
#define GET_SW1    ((GPIO0DATA & (1<<3)) ? 1 : 0)

#define SET_LED0_HIGH   GPIO2DATA |= (1<<4)
#define SET_LED0_LOW    GPIO2DATA &= ~(1<<4)
#define SET_LED0_T      GPIO2DATA ^= (1<<4)

#define SET_LED1_HIGH   GPIO2DATA |= (1<<5)
#define SET_LED1_LOW    GPIO2DATA &= ~(1<<5)
#define SET_LED1_T      GPIO2DATA ^= (1<<5)

#define SET_LED2_HIGH   GPIO0DATA |= (1<<6)
#define SET_LED2_LOW    GPIO0DATA &= ~(1<<6)
#define SET_LED2_T      GPIO0DATA ^= (1<<6)

#define SET_LED3_HIGH   GPIO0DATA |= (1<<7)
#define SET_LED3_LOW    GPIO0DATA &= ~(1<<7)
#define SET_LED3_T      GPIO0DATA ^= (1<<7)


// Core clock     48 MHz
// System clock   48 MHz

// init
void ADC_init(void){

	SYSAHBCLKCTRL |= BIT13; // Turn on clock for ADC
	PDRUNCFG &= ~BIT4; // Power up the ADC

	// select analog mode for PIO1_0
	IOCON_R_PIO1_0 = 2;

	// set ADC clock rate and select channel 1
	AD0CR = (11<<8) + BIT1;
}


// blocking ADC
uint16_t ADC_read(void){
//uint16_t ADC_read(uint8_t ch){
//	AD0CR = (11<<8) + (1 << (ch & 0x07));

	AD0CR |= BIT1 + BIT24; // Start a conversion
	while((AD0GDR&BIT31)==0); // Wait for conversion to complete

	// return the result
	return ((AD0GDR>>6) & 0x3ff);
}


// 48MHz
void PWM_init(void){

// SERVO	P0_11	CT32B0_MAT3

// A1	P1_10	CT16B1_MAT1
// A2	P0_10	CT16B0_MAT2
// B1	P0_9	CT16B0_MAT1
// B2	P0_8	CT16B0_MAT0

	// Turn on CT32B0 CT16B0 CT16B1
	SYSAHBCLKCTRL |= BIT9 | BIT7 | BIT8;

	// Servo @ 50Hz
// 48Mhz
// div 24
// 1s >> 2000000
// 1ms >> 2000
// 1.5ms >> 3000
// 20ms >> 40000
	TMR32B0TCR = 0x2;  // reset
	TMR32B0PR = 24 - 1;
	TMR32B0MR3 = 40000 - 3000; // 1.5ms
	TMR32B0MR0 = 40000; // 20ms
	TMR32B0MCR = BIT1; // Reset TC on match with MR0
	TMR32B0TC = 0;
	TMR32B0PWMC = BIT3; // Enable PWM on channel 3
	TMR32B0TCR = 0x1; // enable = 1, reset = 0

// MAX = 2ms >> 24000 * 2
// MIN = 1ms >> 24000 * 1
//	LPC_TMR32B0->PR = 15;          // div 16
//	LPC_TMR32B0->MR2 = 60000;          // MR2 = Period
//	LPC_TMR32B0->MR3 = 60000 - 4500;            // MR3 = 1.5ms

	// Servo	P0_11	CT32B0_MAT3
	IOCON_R_PIO0_11 = 0xC0 | 0x03; //0x03; // CT32B0_MAT3, no pull


	// Motor PWM (@ 10kHz)
	TMR16B0TCR = 0x2;  // reset
	TMR16B0PR = 0;
	TMR16B0MR3 = 4800;
	TMR16B0MR2 = 4801; // Zero output to begin with
	TMR16B0MR1 = 4801; // Zero output to begin with
	TMR16B0MR0 = 4801; // Zero output to begin with
	TMR16B0MCR = BIT10; // Reset TC on match with MR3
	TMR16B0TC = 0 ; // Zero the counter to begin with
	TMR16B0PWMC = BIT2 | BIT1 | BIT0; // Enable PWM on channel 0, 1, 2
	TMR16B0TCR = 1; // Enable the timer

	TMR16B1TCR = 0x2;  // reset
	TMR16B1PR = 0;
	TMR16B1MR3 = 4800;
	TMR16B1MR1 = 4801; // Zero output to begin with
	TMR16B1MCR = BIT10; // Reset TC on match with MR3
	TMR16B1TC = 0 ; // Zero the counter to begin with
	TMR16B1PWMC = BIT1; // Enable PWM on channel 1
	TMR16B1TCR = 1; // Enable the timer

	// A1	P1_10	CT16B1_MAT1
	IOCON_PIO1_10 = 0xC0 | 0x2; // CT16B1_MAT1, no pull
//	GPIO1DATA &= ~(BIT10);

	// A2	P0_10	CT16B0_MAT2
	IOCON_SWCLK_PIO0_10  = 0xC0 | 0x03; // CT16B0_MAT2, no pull
	// B1	P0_9	CT16B0_MAT1
	IOCON_PIO0_9 = 0xC0 | 0x2; // CT16B0_MAT1, no pull
	// B2	P0_8	CT16B0_MAT0
	IOCON_PIO0_8  = 0xC0 | 0x02; // CT16B0_MAT0, no pull
//	GPIO0DATA &= ~(BIT10 | BIT9 | BIT8);
}

// center 3000
// min 2000
// max 4000
void TFC_SetServo(uint16_t S0){
	TMR32B0MR3 = 40000 - S0;
}

// center 0
// min -4800
// max 4800
void TFC_SetMotorPWM(int16_t MotorA , int16_t MotorB){
	if(MotorA == 0){
			TMR16B1MR1 = 4801;
			TMR16B0MR2 = 4801;
	}else{
		if(MotorA > 0){
			// A1
			TMR16B1MR1 = 4800 - MotorA;
			// A2
			TMR16B0MR2 = 4801;
		}else{
			TMR16B1MR1 = 4801;
			TMR16B0MR2 = 4800 + MotorA;
		}
	}
	if(MotorB == 0){
			TMR16B0MR1 = 4801;
			TMR16B0MR0 = 4801;
	}else{
		if(MotorB > 0){
			// B1
			TMR16B0MR1 = 4800 - MotorB;
			// B2
			TMR16B0MR0 = 4801;
		}else{
			TMR16B0MR1 = 4801;
			TMR16B0MR0 = 4800 + MotorB;
		}
	}
}

void GPIO_init(void){
	SYSAHBCLKCTRL |= BIT6 + BIT16; 	// Turn on clock for GPIO, IOCON

	// GPIO P1_1 (CLK)
	IOCON_R_PIO1_1 &= ~(BIT4+BIT3+BIT2+BIT1+BIT0);  	// ensure Pin P1_1 behaves as GPIO
	IOCON_R_PIO1_1 |= BIT3 + BIT0;  	// ensure Pin P1_1 behaves as GPIO
	GPIO1DIR |= BIT1;

	// GPIO P1_2 (SI)
	IOCON_R_PIO1_2 &= ~(BIT4+BIT3+BIT2+BIT1+BIT0);  	// ensure Pin P1_2 behaves as GPIO
	IOCON_R_PIO1_2 |= BIT3 + BIT0;  	// ensure Pin P1_2 behaves as GPIO
	GPIO1DIR |= BIT2;


	// SW0	P0_1
	// SW1	P0_3
	IOCON_PIO0_1 = 0xC0 | (0x02 << 3) | (0x00); // pull up
	IOCON_PIO0_3 = 0xC0 | (0x02 << 3) | (0x00); // pull up
	GPIO0DIR &= ~(BIT3 | BIT1);


	// DIP0	P0_2
	// DIP1	P2_7
	// DIP2	P2_8
	IOCON_PIO0_2 = 0xC0 | (0x01 << 3) | (0x00); // pull down
	IOCON_PIO2_7 = 0xC0 | (0x01 << 3) | (0x00); // pull down
	IOCON_PIO2_8 = 0xC0 | (0x01 << 3) | (0x00); // pull down
	GPIO0DIR &= ~(BIT3);
	GPIO2DIR &= ~(BIT8 | BIT7);

	// LED0	P2_4
	// LED1	P2_5
	// LED2	P0_6
	// LED3	P0_7
	IOCON_PIO2_4 = 0xC0 | (0x00 << 3) | (0x00);
	IOCON_PIO2_5 = 0xC0 | (0x00 << 3) | (0x00);
	IOCON_PIO0_6 = 0xC0 | (0x00 << 3) | (0x00);
	IOCON_PIO0_7 = 0xC0 | (0x00 << 3) | (0x00);
	GPIO2DIR |= BIT5 | BIT4;
	GPIO0DIR |= BIT7 | BIT6;

	GPIO2DATA &= ~(BIT5 | BIT4);
	GPIO0DATA &= ~(BIT7 | BIT6);

}

#endif

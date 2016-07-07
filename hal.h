#ifndef __HAL_H
#define __HAL_H

#include "lpc111x.h"


// CLK P1_1
#define TAOS_CLK_HIGH  GPIO1DATA |= (1<<1)
#define TAOS_CLK_LOW   GPIO1DATA &= ~(1<<1)

// SI P1_2
#define TAOS_SI_HIGH   GPIO1DATA |= (1<<2)
#define TAOS_SI_LOW    GPIO1DATA &= ~(1<<2)

//#define TFC_HBRIDGE_ENABLE          GPIOB_PSOR = (1 << 20)
//#define TFC_HBRIDGE_DISABLE         GPIOB_PCOR = (1 << 20)

/*
#define GET_DIP0    (GPIO1DATA & (1<<1))
#define GET_DIP1    (GPIO1DATA & (1<<19))
#define GET_DIP2    (GPIO1DATA & (1<<18))

#define GET_SW0    ((GPIO1DATA & (1<<4)) ? 1 : 0)
#define GET_SW1    ((GPIO1DATA & (1<<5)) ? 1 : 0)

#define SET_LED0_HIGH   GPIOD_PSOR = (1<<1)
#define SET_LED0_LOW    GPIOD_PCOR = (1<<1)
#define SET_LED0_T      GPIOD_PTOR = (1<<1)

#define SET_LED1_HIGH   GPIOD_PSOR = (1<<3)
#define SET_LED1_LOW    GPIOD_PCOR = (1<<3)
#define SET_LED1_T      GPIOD_PTOR = (1<<3)

#define SET_LED2_HIGH   GPIOD_PSOR = (1<<2)
#define SET_LED2_LOW    GPIOD_PCOR = (1<<2)
#define SET_LED2_T      GPIOD_PTOR = (1<<2)

#define SET_LED3_HIGH   GPIOD_PSOR = (1<<0)
#define SET_LED3_LOW    GPIOD_PCOR = (1<<0)
#define SET_LED3_T      GPIOD_PTOR = (1<<0)
*/

// Core clock     48 MHz
// System clock   48 MHz
// Bus clock       24 MHz


// ADC clock = Bus clock
// FlexTimers = Bus clock
// PIT = Bus clock
// GPIO = System clock

// I2C = Bus clock
// UART0-1 = System clock
// UART2-5 = Bus clock

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

	// Turn on CT32B1
	SYSAHBCLKCTRL |= BIT10;

	// Servo @ 50Hz
	// GPIO PTC10 (S0)

	// GPIO PTC11 (S1)


// 60000000
// div 32
// 1875 >> 1ms
// 37500 >> 20ms
/*	SIM_SCGC3 |= (1 << 25); // Enable FTM3 (Motor)
	FTM3_SC |= (1 << 3) | (5 << 0); // CLK = System clock / 32
	FTM3_CNT = 0;
	FTM3_MOD = 37500; // 10kHz

	FTM3_C6SC |= (2 << 4) | (2 << 2);
	FTM3_C6V = 2812;

	FTM3_C7SC |= (2 << 4) | (2 << 2);
	FTM3_C7V = 2812;
*/

	// Motor PWM (max 11kHz)
	// GPIO PTC2 (A1)

	// GPIO PTC3 (A2)

	// GPIO PTA1 (B1)

	// GPIO PTA2 (B2)



	// FTM0
/*	SIM_SCGC6 |= (1 << 24); // Enable FTM0 (servo)
	FTM0_SC |= (1 << 3) | (0 << 0); // CLK = System clock / 32
	FTM0_CNT = 0;
	FTM0_MOD = 6000;
*/
	// A1

	// A2

	// B1

	// B2


}

// center 2812
// min 1875
// max 3750
void TFC_SetServo(uint16_t S0 , uint16_t S1){

}

// center 3000
// min 0
// max 6000
void TFC_SetMotorPWM(uint16_t MotorA , uint16_t MotorB){

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


	// GPIO P (LED_0)

	// GPIO P (LED_1)

	// GPIO P (LED_2)

	// GPIO P (LED_3)

	// GPIO PTB20 (H_EN)


	// GPIO PTC1 (DIP0)

	// GPIO PTB19 (DIP1)

	// GPIO PTB18 (DIP2)

	// GPIO PTC4 (SW0)

	// GPIO PTC5 (Sw1)

}

#endif

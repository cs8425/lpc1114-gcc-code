// lpc111x.h
// LPC low level all-in-one header file for lpc111x devices
// principally targeted at lpc1114fn28 (DIP28 package)
// Written by Frank Duignan
// Latest version available at http://eleceng.dit.ie/frank/arm/cortex/lpc111x.h
// Derived from UM10398 user manual from NXP semiconductors
// Naming convention: Register names are as described in UM10398
// No claims are made for the suitability, accuracy or otherwise of this file
// for any application
// Define some bitmasks
#define BIT0 (1 << 0)
#define BIT1 (1 << 1)
#define BIT2 (1 << 2)
#define BIT3 (1 << 3)
#define BIT4 (1 << 4)
#define BIT5 (1 << 5)
#define BIT6 (1 << 6)
#define BIT7 (1 << 7)
#define BIT8 (1 << 8)
#define BIT9 (1 << 9)
#define BIT10 (1 << 10)
#define BIT11 (1 << 11)
#define BIT12 (1 << 12)
#define BIT13 (1 << 13)
#define BIT14 (1 << 14)
#define BIT15 (1 << 15)
#define BIT16 (1 << 16)
#define BIT17 (1 << 17)
#define BIT18 (1 << 18)
#define BIT19 (1 << 19)
#define BIT20 (1 << 20)
#define BIT21 (1 << 21)
#define BIT22 (1 << 22)
#define BIT23 (1 << 23)
#define BIT24 (1 << 24)
#define BIT25 (1 << 25)
#define BIT26 (1 << 26)
#define BIT27 (1 << 27)
#define BIT28 (1 << 28)
#define BIT29 (1 << 29)
#define BIT30 (1 << 30)
#define BIT31 (1 << 31)

// Macros to reduce typing later on
#define  REGISTER_32(ADDRESS) (*((volatile unsigned int *)(ADDRESS)))
#define  REGISTER_16(ADDRESS) (*((volatile unsigned int *)(ADDRESS)))
// Macros to enable/disable global interrupts
#define enable_interrupts() asm(" cpsie i ")
#define disable_interrupts() asm(" cpsid i ")


// APB Peripherals
#define I2C_BASE 		0x40000000
#define WDT_BASE 		0x40004000
#define UART_BASE 		0x40008000
#define TMR16B0_BASE	0x4000c000
#define TMR16B1_BASE	0x40010000
#define TMR32B0_BASE	0x40014000
#define TMR32B1_BASE	0x40018000
#define ADC_BASE		0x4001c000
#define PMU_BASE		0x40038000
#define FLASH_CTRL_BASE	0x4003c000
#define SSP0_BASE		0x40040000
#define IOCONFIG_BASE 	0x40044000
#define SYSCON_BASE 	0x40048000
#define C_CAN_BASE		0x40050000
#define SSP1_BASE		0x40058000

// I2C
#define I2C0CONSET		REGISTER_32(I2C_BASE + 0x000)
#define I2C0STAT		REGISTER_32(I2C_BASE + 0x004)
#define I2C0DAT			REGISTER_32(I2C_BASE + 0x008)
#define I2C0ADR0		REGISTER_32(I2C_BASE + 0x00c)
#define I2C0SCLH		REGISTER_32(I2C_BASE + 0x010)
#define I2C0SCLL		REGISTER_32(I2C_BASE + 0x014)
#define I2C0CONCLR		REGISTER_32(I2C_BASE + 0x018)
#define I2C0COMMCTRL	REGISTER_32(I2C_BASE + 0x01c)
#define I2C0ADR1		REGISTER_32(I2C_BASE + 0x020)
#define I2C0ADR2		REGISTER_32(I2C_BASE + 0x024)
#define I2C0ADR3		REGISTER_32(I2C_BASE + 0x028)
#define I2C0DATA_BUFFER	REGISTER_32(I2C_BASE + 0x02c)
#define I2C0MASK0		REGISTER_32(I2C_BASE + 0x030)
#define I2C0MASK1		REGISTER_32(I2C_BASE + 0x034)
#define I2C0MASK2		REGISTER_32(I2C_BASE + 0x038)
#define I2C0MASK3		REGISTER_32(I2C_BASE + 0x03c)
// WDT
#define WDMOD			REGISTER_32(WDT_BASE + 0x000)
#define WDTC			REGISTER_32(WDT_BASE + 0x004)
#define WDFEED			REGISTER_32(WDT_BASE + 0x008)
#define WDTV			REGISTER_32(WDT_BASE + 0x00c)
#define WDWARNINT		REGISTER_32(WDT_BASE + 0x014)
#define WDWINDOW		REGISTER_32(WDT_BASE + 0x018)

// UART
// registers appear to share addresses here.  Depending 
// on the state of DLAB different registers are active.
// Also some registers are read-only, some write-only
#define U0RBR			REGISTER_32(UART_BASE + 0x000) 
#define U0THR			REGISTER_32(UART_BASE + 0x000)
#define U0DLL			REGISTER_32(UART_BASE + 0x000)
#define U0DLM			REGISTER_32(UART_BASE + 0x004)
#define U0IER			REGISTER_32(UART_BASE + 0x004)
#define U0IIR			REGISTER_32(UART_BASE + 0x008)
#define U0FCR			REGISTER_32(UART_BASE + 0x008)
#define U0LCR			REGISTER_32(UART_BASE + 0x00c)
#define U0MCR			REGISTER_32(UART_BASE + 0x010)
#define U0LSR			REGISTER_32(UART_BASE + 0x014)
#define U0MSR			REGISTER_32(UART_BASE + 0x018)
#define U0SCR			REGISTER_32(UART_BASE + 0x01c)
#define U0ACR			REGISTER_32(UART_BASE + 0x020)
#define U0FDR			REGISTER_32(UART_BASE + 0x028)
#define U0TER			REGISTER_32(UART_BASE + 0x030)
#define U0RS485CTRL		REGISTER_32(UART_BASE + 0x04c)
#define U0RS485ADRMATCH		REGISTER_32(UART_BASE + 0x050)
#define U0RS485DLY		REGISTER_32(UART_BASE + 0x054)



// TMR16B0
#define TMR16B0IR		REGISTER_32(TMR16B0_BASE + 0x000)
#define TMR16B0TCR		REGISTER_32(TMR16B0_BASE + 0x004)
#define TMR16B0TC		REGISTER_32(TMR16B0_BASE + 0x008)
#define TMR16B0PR		REGISTER_32(TMR16B0_BASE + 0x00c)
#define TMR16B0PC		REGISTER_32(TMR16B0_BASE + 0x010)
#define TMR16B0MCR		REGISTER_32(TMR16B0_BASE + 0x014)
#define TMR16B0MR0		REGISTER_32(TMR16B0_BASE + 0x018)
#define TMR16B0MR1		REGISTER_32(TMR16B0_BASE + 0x01c)
#define TMR16B0MR2		REGISTER_32(TMR16B0_BASE + 0x020)
#define TMR16B0MR3		REGISTER_32(TMR16B0_BASE + 0x024)
#define TMR16B0CCR		REGISTER_32(TMR16B0_BASE + 0x028)
#define TMR16B0CR0		REGISTER_32(TMR16B0_BASE + 0x02c)
#define TMR16B0CR1		REGISTER_32(TMR16B0_BASE + 0x030)
#define TMR16B0EMR		REGISTER_32(TMR16B0_BASE + 0x03c)
#define TMR16B0CTCR		REGISTER_32(TMR16B0_BASE + 0x070)
#define TMR16B0PWMC		REGISTER_32(TMR16B0_BASE + 0x074)
// TMR16B1
#define TMR16B1IR		REGISTER_32(TMR16B1_BASE + 0x000)
#define TMR16B1TCR		REGISTER_32(TMR16B1_BASE + 0x004)
#define TMR16B1TC		REGISTER_32(TMR16B1_BASE + 0x008)
#define TMR16B1PR		REGISTER_32(TMR16B1_BASE + 0x00c)
#define TMR16B1PC		REGISTER_32(TMR16B1_BASE + 0x010)
#define TMR16B1MCR		REGISTER_32(TMR16B1_BASE + 0x014)
#define TMR16B1MR0		REGISTER_32(TMR16B1_BASE + 0x018)
#define TMR16B1MR1		REGISTER_32(TMR16B1_BASE + 0x01c)
#define TMR16B1MR2		REGISTER_32(TMR16B1_BASE + 0x020)
#define TMR16B1MR3		REGISTER_32(TMR16B1_BASE + 0x024)
#define TMR16B1CCR		REGISTER_32(TMR16B1_BASE + 0x028)
#define TMR16B1CR0		REGISTER_32(TMR16B1_BASE + 0x02c)
#define TMR16B1CR1		REGISTER_32(TMR16B1_BASE + 0x030)
#define TMR16B1EMR		REGISTER_32(TMR16B1_BASE + 0x03c)
#define TMR16B1CTCR		REGISTER_32(TMR16B1_BASE + 0x070)
#define TMR16B1PWMC		REGISTER_32(TMR16B1_BASE + 0x074)



// TMR32B0
#define TMR32B0IR		REGISTER_32(TMR32B0_BASE + 0x000)
#define TMR32B0TCR		REGISTER_32(TMR32B0_BASE + 0x004)
#define TMR32B0TC		REGISTER_32(TMR32B0_BASE + 0x008)
#define TMR32B0PR		REGISTER_32(TMR32B0_BASE + 0x00c)
#define TMR32B0PC		REGISTER_32(TMR32B0_BASE + 0x010)
#define TMR32B0MCR		REGISTER_32(TMR32B0_BASE + 0x014)
#define TMR32B0MR0		REGISTER_32(TMR32B0_BASE + 0x018)
#define TMR32B0MR1		REGISTER_32(TMR32B0_BASE + 0x01c)
#define TMR32B0MR2		REGISTER_32(TMR32B0_BASE + 0x020)
#define TMR32B0MR3		REGISTER_32(TMR32B0_BASE + 0x024)
#define TMR32B0CCR		REGISTER_32(TMR32B0_BASE + 0x028)
#define TMR32B0CR0		REGISTER_32(TMR32B0_BASE + 0x02c)
#define TMR32B0CR1		REGISTER_32(TMR32B0_BASE + 0x030)
#define TMR32B0EMR		REGISTER_32(TMR32B0_BASE + 0x03c)
#define TMR32B0CTCR		REGISTER_32(TMR32B0_BASE + 0x070)
#define TMR32B0PWMC		REGISTER_32(TMR32B0_BASE + 0x074)
// TMR32B1
#define TMR32B1IR		REGISTER_32(TMR32B1_BASE + 0x000)
#define TMR32B1TCR		REGISTER_32(TMR32B1_BASE + 0x004)
#define TMR32B1TC		REGISTER_32(TMR32B1_BASE + 0x008)
#define TMR32B1PR		REGISTER_32(TMR32B1_BASE + 0x00c)
#define TMR32B1PC		REGISTER_32(TMR32B1_BASE + 0x010)
#define TMR32B1MCR		REGISTER_32(TMR32B1_BASE + 0x014)
#define TMR32B1MR0		REGISTER_32(TMR32B1_BASE + 0x018)
#define TMR32B1MR1		REGISTER_32(TMR32B1_BASE + 0x01c)
#define TMR32B1MR2		REGISTER_32(TMR32B1_BASE + 0x020)
#define TMR32B1MR3		REGISTER_32(TMR32B1_BASE + 0x024)
#define TMR32B1CCR		REGISTER_32(TMR32B1_BASE + 0x028)
#define TMR32B1CR0		REGISTER_32(TMR32B1_BASE + 0x02c)
#define TMR32B1CR1		REGISTER_32(TMR32B1_BASE + 0x030)
#define TMR32B1EMR		REGISTER_32(TMR32B1_BASE + 0x03c)
#define TMR32B1CTCR		REGISTER_32(TMR32B1_BASE + 0x070)
#define TMR32B1PWMC		REGISTER_32(TMR32B1_BASE + 0x074)



// ADC 
#define AD0CR		REGISTER_32(ADC_BASE + 0x000)
#define AD0GDR		REGISTER_32(ADC_BASE + 0x004)
#define AD0INTEN	REGISTER_32(ADC_BASE + 0x00c)
#define AD0DR0		REGISTER_32(ADC_BASE + 0x010)
#define AD0DR1		REGISTER_32(ADC_BASE + 0x014)
#define AD0DR2		REGISTER_32(ADC_BASE + 0x018)
#define AD0DR3		REGISTER_32(ADC_BASE + 0x01c)
#define AD0DR4		REGISTER_32(ADC_BASE + 0x020)
#define AD0DR5		REGISTER_32(ADC_BASE + 0x024)
#define AD0DR6		REGISTER_32(ADC_BASE + 0x028)
#define AD0DR7		REGISTER_32(ADC_BASE + 0x02c)
#define AD0STAT		REGISTER_32(ADC_BASE + 0x030)

// PMU
#define PCON		REGISTER_32(PMU_BASE + 0x000)
#define GPREG0		REGISTER_32(PMU_BASE + 0x004)
#define GPREG1		REGISTER_32(PMU_BASE + 0x008)
#define GPREG2		REGISTER_32(PMU_BASE + 0x00c)
#define GPREG3		REGISTER_32(PMU_BASE + 0x010)
#define GPREG4		REGISTER_32(PMU_BASE + 0x014)

// FLASH CONTROLLER
#define FLASHCFG	REGISTER_32(FLASH_CTRL_BASE + 0x010)
#define FMSSTART	REGISTER_32(FLASH_CTRL_BASE + 0x020)
#define FMSSTOP		REGISTER_32(FLASH_CTRL_BASE + 0x024)
#define FMSW0		REGISTER_32(FLASH_CTRL_BASE + 0x02c)
#define FMSW1		REGISTER_32(FLASH_CTRL_BASE + 0x030)
#define FMSW2		REGISTER_32(FLASH_CTRL_BASE + 0x034)
#define FMSW3		REGISTER_32(FLASH_CTRL_BASE + 0x038)
#define FMSTAT		REGISTER_32(FLASH_CTRL_BASE + 0xfe0)
#define FMSTATCLR	REGISTER_32(FLASH_CTRL_BASE + 0xfe8)
// IOCONFIG
#define IOCON_PIO2_6		REGISTER_32(IOCONFIG_BASE + 0x000)
#define IOCON_PIO2_0		REGISTER_32(IOCONFIG_BASE + 0x008)
#define IOCON_RESET_PIO0_0	REGISTER_32(IOCONFIG_BASE + 0x00c)
#define IOCON_PIO0_1		REGISTER_32(IOCONFIG_BASE + 0x010)
#define IOCON_PIO1_8		REGISTER_32(IOCONFIG_BASE + 0x014)
#define IOCON_SSEL1_LOC		REGISTER_32(IOCONFIG_BASE + 0x018)
#define IOCON_PIO0_2		REGISTER_32(IOCONFIG_BASE + 0x01c)
#define IOCON_PIO2_7		REGISTER_32(IOCONFIG_BASE + 0x020)
#define IOCON_PIO2_8		REGISTER_32(IOCONFIG_BASE + 0x024)
#define IOCON_PIO2_1		REGISTER_32(IOCONFIG_BASE + 0x028)
#define IOCON_PIO0_3		REGISTER_32(IOCONFIG_BASE + 0x02c)
#define IOCON_PIO0_4		REGISTER_32(IOCONFIG_BASE + 0x030)
#define IOCON_PIO0_5		REGISTER_32(IOCONFIG_BASE + 0x034)
#define IOCON_PIO1_9		REGISTER_32(IOCONFIG_BASE + 0x038)
#define IOCON_PIO3_4		REGISTER_32(IOCONFIG_BASE + 0x03c)
#define IOCON_PIO2_4		REGISTER_32(IOCONFIG_BASE + 0x040)
#define IOCON_PIO2_5		REGISTER_32(IOCONFIG_BASE + 0x044)
#define IOCON_PIO3_5		REGISTER_32(IOCONFIG_BASE + 0x048)
#define IOCON_PIO0_6		REGISTER_32(IOCONFIG_BASE + 0x04c)
#define IOCON_PIO0_7		REGISTER_32(IOCONFIG_BASE + 0x050)
#define IOCON_PIO2_9		REGISTER_32(IOCONFIG_BASE + 0x054)
#define IOCON_PIO2_10		REGISTER_32(IOCONFIG_BASE + 0x058)
#define IOCON_PIO2_2		REGISTER_32(IOCONFIG_BASE + 0x05c)
#define IOCON_PIO0_8		REGISTER_32(IOCONFIG_BASE + 0x060)
#define IOCON_PIO0_9		REGISTER_32(IOCONFIG_BASE + 0x064)
#define IOCON_SWCLK_PIO0_10 	REGISTER_32(IOCONFIG_BASE + 0x068)
#define IOCON_PIO1_10		REGISTER_32(IOCONFIG_BASE + 0x06c)
#define IOCON_PIO2_11		REGISTER_32(IOCONFIG_BASE + 0x070)
#define IOCON_R_PIO0_11		REGISTER_32(IOCONFIG_BASE + 0x074)
#define IOCON_R_PIO1_0		REGISTER_32(IOCONFIG_BASE + 0x078)
#define IOCON_R_PIO1_1		REGISTER_32(IOCONFIG_BASE + 0x07c)
#define IOCON_R_PIO1_2		REGISTER_32(IOCONFIG_BASE + 0x080)
#define IOCON_PIO3_0		REGISTER_32(IOCONFIG_BASE + 0x084)
#define IOCON_PIO3_1		REGISTER_32(IOCONFIG_BASE + 0x088)
#define IOCON_PIO2_3		REGISTER_32(IOCONFIG_BASE + 0x08c)
#define IOCON_SWDIO_PIO1_3	REGISTER_32(IOCONFIG_BASE + 0x090)
#define IOCON_PIO1_4		REGISTER_32(IOCONFIG_BASE + 0x094)
#define IOCON_PIO1_11		REGISTER_32(IOCONFIG_BASE + 0x098)
#define IOCON_PIO3_2		REGISTER_32(IOCONFIG_BASE + 0x09c)
#define IOCON_PIO1_5		REGISTER_32(IOCONFIG_BASE + 0x0a0)
#define IOCON_PIO1_6		REGISTER_32(IOCONFIG_BASE + 0x0a4)
#define IOCON_PIO1_7		REGISTER_32(IOCONFIG_BASE + 0x0a8)
#define IOCON_PIO3_3		REGISTER_32(IOCONFIG_BASE + 0x0ac)
#define IOCON_SCK0_LOC		REGISTER_32(IOCONFIG_BASE + 0x0b0)
#define IOCON_DSR_LOC		REGISTER_32(IOCONFIG_BASE + 0x0b4)
#define IOCON_DCD_LOC		REGISTER_32(IOCONFIG_BASE + 0x0b8)
#define IOCON_RI_LOC		REGISTER_32(IOCONFIG_BASE + 0x0bc)
#define IOCON_CT16B0_CAP0_LOC 		REGISTER_32(IOCONFIG_BASE + 0x0c0)
#define IOCON_SCK1_LOC		REGISTER_32(IOCONFIG_BASE + 0x0c4)
#define IOCON_MISO1_LOC		REGISTER_32(IOCONFIG_BASE + 0x0c8)
#define IOCON_MOSI1_LOC		REGISTER_32(IOCONFIG_BASE + 0x0cc)
#define IOCON_CT32B0_CAP0_LOC		REGISTER_32(IOCONFIG_BASE + 0x0d0)
#define IOCON_RXD_LOC		REGISTER_32(IOCONFIG_BASE + 0x0d4)





// SYSCON
#define SYSMEMREMAP		REGISTER_32(SYSCON_BASE + 0x000)
#define PRESETCTRL		REGISTER_32(SYSCON_BASE + 0x004)
#define SYSPLLCTRL		REGISTER_32(SYSCON_BASE + 0x008)
#define SYSPLLSTAT		REGISTER_32(SYSCON_BASE + 0x00c)
#define SYSOSCCTRL		REGISTER_32(SYSCON_BASE + 0x020)
#define WDTOSCCTRL		REGISTER_32(SYSCON_BASE + 0x024)
#define IRCCTL			REGISTER_32(SYSCON_BASE + 0x028)
#define SYSRSTSTAT		REGISTER_32(SYSCON_BASE + 0x030)
#define SYSPLLCLKSEL	REGISTER_32(SYSCON_BASE + 0x040)
#define SYSPLLCLKUEN	REGISTER_32(SYSCON_BASE + 0x044)
#define MAINCLKSEL		REGISTER_32(SYSCON_BASE + 0x070)
#define MAINCLKUEN		REGISTER_32(SYSCON_BASE + 0x074)
#define SYSAHBCLKDIV	REGISTER_32(SYSCON_BASE + 0x078)
#define SYSAHBCLKCTRL	REGISTER_32(SYSCON_BASE + 0x080)
#define SSP0CLKDIV		REGISTER_32(SYSCON_BASE + 0x094)
#define UARTCLKDIV		REGISTER_32(SYSCON_BASE + 0x098)
#define SSP1CLKDIV		REGISTER_32(SYSCON_BASE + 0x09c)
#define WDTCLKSEL		REGISTER_32(SYSCON_BASE + 0x0d0)
#define WDTCLKUEN		REGISTER_32(SYSCON_BASE + 0x0d4)
#define WDTCLKDIV		REGISTER_32(SYSCON_BASE + 0x0d8)
#define CLKOUTCLKSEL	REGISTER_32(SYSCON_BASE + 0x0e0)
#define CLKOUTUEN		REGISTER_32(SYSCON_BASE + 0x0e4)
#define CLKOUTCLKDIV	REGISTER_32(SYSCON_BASE + 0x0e8)
#define PIOPORCAP0		REGISTER_32(SYSCON_BASE + 0x100)
#define PIOPORCAP1		REGISTER_32(SYSCON_BASE + 0x104)
#define BODCTRL			REGISTER_32(SYSCON_BASE + 0x150)
#define SYSTCKCAL		REGISTER_32(SYSCON_BASE + 0x154)
#define NMISRC			REGISTER_32(SYSCON_BASE + 0x174)
#define STARTAPRP0		REGISTER_32(SYSCON_BASE + 0x200)
#define STARTERP0		REGISTER_32(SYSCON_BASE + 0x204)
#define STARTRSRP0CLR	REGISTER_32(SYSCON_BASE + 0x208)
#define STARTSRP0		REGISTER_32(SYSCON_BASE + 0x20c)
#define PDSLEEPCFG		REGISTER_32(SYSCON_BASE + 0x230)
#define PDAWAKECFG		REGISTER_32(SYSCON_BASE + 0x234)
#define PDRUNCFG		REGISTER_32(SYSCON_BASE + 0x238)
#define DEVICE_ID		REGISTER_32(SYSCON_BASE + 0x3f4)


// CAN
#define CANCNTL		REGISTER_32(C_CAN_BASE + 0x000)
#define CANSTAT		REGISTER_32(C_CAN_BASE + 0x004)
#define CANEC		REGISTER_32(C_CAN_BASE + 0x008)
#define CANBT		REGISTER_32(C_CAN_BASE + 0x00c)
#define CANINT		REGISTER_32(C_CAN_BASE + 0x010)
#define CANTEST		REGISTER_32(C_CAN_BASE + 0x014)
#define CANBRPE		REGISTER_32(C_CAN_BASE + 0x018)
#define CANIF1_CMDREQ	REGISTER_32(C_CAN_BASE + 0x020)
#define CANIF1_CMDMSK_W	REGISTER_32(C_CAN_BASE + 0x024)
#define CANIF1_CMDMSK_R	REGISTER_32(C_CAN_BASE + 0x024)
#define CANIF1_MSK1		REGISTER_32(C_CAN_BASE + 0x028)
#define CANIF1_MSK2		REGISTER_32(C_CAN_BASE + 0x02c)
#define CANIF1_ARB1		REGISTER_32(C_CAN_BASE + 0x030)
#define CANIF1_ARB2		REGISTER_32(C_CAN_BASE + 0x034)
#define CANIF1_MCTRL	REGISTER_32(C_CAN_BASE + 0x038)
#define CANIF1_DA1		REGISTER_32(C_CAN_BASE + 0x03c)
#define CANIF1_DA2		REGISTER_32(C_CAN_BASE + 0x040)
#define CANIF1_DB1		REGISTER_32(C_CAN_BASE + 0x044)
#define CANIF1_DB2		REGISTER_32(C_CAN_BASE + 0x048)
#define CANIF2_CMDREQ	REGISTER_32(C_CAN_BASE + 0x080)
#define CANIF2_CMDMSK_W	REGISTER_32(C_CAN_BASE + 0x084)
#define CANIF2_CMDMSK_R	REGISTER_32(C_CAN_BASE + 0x084)
#define CANIF2_MSK1		REGISTER_32(C_CAN_BASE + 0x088)
#define CANIF2_MSK2		REGISTER_32(C_CAN_BASE + 0x08c)
#define CANIF2_ARB1		REGISTER_32(C_CAN_BASE + 0x090)
#define CANIF2_ARB2		REGISTER_32(C_CAN_BASE + 0x094)
#define CANIF2_MCTRL	REGISTER_32(C_CAN_BASE + 0x098)
#define CANIF2_DA1		REGISTER_32(C_CAN_BASE + 0x09c)
#define CANIF2_DA2		REGISTER_32(C_CAN_BASE + 0x0a0)
#define CANIF2_DB1		REGISTER_32(C_CAN_BASE + 0x0a4)
#define CANIF2_DB2		REGISTER_32(C_CAN_BASE + 0x0a8)
#define CANTXREQ1		REGISTER_32(C_CAN_BASE + 0x100)
#define CANTXREQ2		REGISTER_32(C_CAN_BASE + 0x104)
#define CANND1			REGISTER_32(C_CAN_BASE + 0x120)
#define CANND2			REGISTER_32(C_CAN_BASE + 0x124)
#define CANIR1			REGISTER_32(C_CAN_BASE + 0x140)
#define CANIR2			REGISTER_32(C_CAN_BASE + 0x144)
#define CANMSGV1		REGISTER_32(C_CAN_BASE + 0x160)
#define CANNSGV2		REGISTER_32(C_CAN_BASE + 0x164)
#define CANCLKDIV		REGISTER_32(C_CAN_BASE + 0x180)


// SSP0
#define SSP0CR0		REGISTER_32(SSP0_BASE + 0x000)
#define SSP0CR1		REGISTER_32(SSP0_BASE + 0x004)
#define SSP0DR		REGISTER_32(SSP0_BASE + 0x008)
#define SSP0SR		REGISTER_32(SSP0_BASE + 0x00c)
#define SSP0CPSR	REGISTER_32(SSP0_BASE + 0x010)
#define SSP0IMSC	REGISTER_32(SSP0_BASE + 0x014)
#define SSP0RIS		REGISTER_32(SSP0_BASE + 0x018)
#define SSP0MIS		REGISTER_32(SSP0_BASE + 0x01c)
#define SSP0ICR		REGISTER_32(SSP0_BASE + 0x020)

// SSP1
#define SSP1CR0		REGISTER_32(SSP1_BASE + 0x000)
#define SSP1CR1		REGISTER_32(SSP1_BASE + 0x004)
#define SSP1DR		REGISTER_32(SSP1_BASE + 0x008)
#define SSP1SR		REGISTER_32(SSP1_BASE + 0x00c)
#define SSP1CPSR	REGISTER_32(SSP1_BASE + 0x010)
#define SSP1IMSC	REGISTER_32(SSP1_BASE + 0x014)
#define SSP1RIS		REGISTER_32(SSP1_BASE + 0x018)
#define SSP1MIS		REGISTER_32(SSP1_BASE + 0x01c)
#define SSP1ICR		REGISTER_32(SSP1_BASE + 0x020)

// AHB Peripherals
#define GPIO0_BASE		0x50000000
#define GPIO1_BASE		0x50010000
#define GPIO2_BASE		0x50020000
#define GPIO3_BASE		0x50030000

// not dealing with mask registers here
#define GPIO0DATA		REGISTER_32(GPIO0_BASE + 0x3ffc)
#define GPIO0DIR		REGISTER_32(GPIO0_BASE + 0x8000)
#define GPIO0IS			REGISTER_32(GPIO0_BASE + 0x8004)
#define GPIO0IBE		REGISTER_32(GPIO0_BASE + 0x8008)
#define GPIO0IEV		REGISTER_32(GPIO0_BASE + 0x800c)
#define GPIO0IE			REGISTER_32(GPIO0_BASE + 0x8010)
#define GPIO0RIS		REGISTER_32(GPIO0_BASE + 0x8014)
#define GPIO0MIS		REGISTER_32(GPIO0_BASE + 0x8018)
#define GPIO0IC			REGISTER_32(GPIO0_BASE + 0x801c)

// not dealing with mask registers here
#define GPIO1DATA		REGISTER_32(GPIO1_BASE + 0x3ffc)
#define GPIO1DIR		REGISTER_32(GPIO1_BASE + 0x8000)
#define GPIO1IS			REGISTER_32(GPIO1_BASE + 0x8004)
#define GPIO1IBE		REGISTER_32(GPIO1_BASE + 0x8008)
#define GPIO1IEV		REGISTER_32(GPIO1_BASE + 0x800c)
#define GPIO1IE			REGISTER_32(GPIO1_BASE + 0x8010)
#define GPIO1RIS		REGISTER_32(GPIO1_BASE + 0x8014)
#define GPIO1MIS		REGISTER_32(GPIO1_BASE + 0x8018)
#define GPIO1IC			REGISTER_32(GPIO1_BASE + 0x801c)

// not dealing with mask registers here
#define GPIO2DATA		REGISTER_32(GPIO2_BASE + 0x3ffc)
#define GPIO2DIR		REGISTER_32(GPIO2_BASE + 0x8000)
#define GPIO2IS			REGISTER_32(GPIO2_BASE + 0x8004)
#define GPIO2IBE		REGISTER_32(GPIO2_BASE + 0x8008)
#define GPIO2IEV		REGISTER_32(GPIO2_BASE + 0x800c)
#define GPIO2IE			REGISTER_32(GPIO2_BASE + 0x8010)
#define GPIO2RIS		REGISTER_32(GPIO2_BASE + 0x8014)
#define GPIO2MIS		REGISTER_32(GPIO2_BASE + 0x8018)
#define GPIO2IC			REGISTER_32(GPIO2_BASE + 0x801c)

// Core peripherals
#define STK_BASE	0xe000e010
#define SCB_BASE	0xe000ed00
#define NVIC_BASE 	0xe000e100
// Seems base addresses are split for some core peripherals
#define SCB_BASE2	0xe000e008
#define NVIC_BASE2	0xe000ef00 

// NVIC
#define ISER		REGISTER_32(NVIC_BASE + 0)
#define ICER		REGISTER_32(NVIC_BASE + 0x80)
#define ISPR		REGISTER_32(NVIC_BASE + 0x100)
#define ICPR		REGISTER_32(NVIC_BASE + 0x180)
#define IPR0		REGISTER_32(NVIC_BASE + 0x300)
#define IPR1		REGISTER_32(NVIC_BASE + 0x304)
#define IPR2		REGISTER_32(NVIC_BASE + 0x308)
#define IPR3		REGISTER_32(NVIC_BASE + 0x30c)
#define IPR4		REGISTER_32(NVIC_BASE + 0x310)
#define IPR5		REGISTER_32(NVIC_BASE + 0x314)
#define IPR6		REGISTER_32(NVIC_BASE + 0x318)
#define IPR7		REGISTER_32(NVIC_BASE + 0x31c)

// STK
#define SYST_CSR	REGISTER_32(STK_BASE + 0)
#define SYST_RVR	REGISTER_32(STK_BASE + 4)
#define SYST_CVR	REGISTER_32(STK_BASE + 8)
#define SYST_CALIB	REGISTER_32(STK_BASE + 0x0c)

// SCB_BASE
#define CPUID		REGISTER_32(SCB_BASE + 0)
#define ICSR		REGISTER_32(SCB_BASE + 4)
#define AIRCR		REGISTER_32(SCB_BASE + 0x0c)
#define SCR			REGISTER_32(SCB_BASE + 0x10)
#define CCR			REGISTER_32(SCB_BASE + 0x14)
#define SHPR2		REGISTER_32(SCB_BASE + 0x1c)
#define SHPR3		REGISTER_32(SCB_BASE + 0x20)


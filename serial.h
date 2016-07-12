#include <stdint.h>

#define RX_SBUFSIZE 256
#define TX_SBUFSIZE 768
typedef struct {
	volatile unsigned head;
	volatile unsigned tail;
	volatile int count;
	char buffer[RX_SBUFSIZE];
} SerialBuffer_RX;

typedef struct {
	volatile unsigned head;
	volatile unsigned tail;
	volatile int count;
	char buffer[TX_SBUFSIZE];
} SerialBuffer_TX;

void initUART(void);
void UART_baud(int baudrate);
void putBuf_RX(SerialBuffer_RX *sbuf, char c);
char getBuf_RX(SerialBuffer_RX *sbuf);
void putBuf_TX(SerialBuffer_TX *sbuf, char c);
char getBuf_TX(SerialBuffer_TX *sbuf);
int rx_count(void);
int tx_count(void);
char egetc(void);
void eputc(char c);
void printString(char *s);
void printInteger(unsigned int Number);
void printShort(unsigned int Number);
void printByte(unsigned int Number);


//uart_defines.h
#define TXD0 0x00000001
#define RXD0 0x00000004

#define TXD1 0x00010000
#define RXD1 0x00040000

#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define BAUD 9600
#define DIVISOR (PCLK/(16*BAUD))

//DEFINES FORE LCR
#define WORDLEN 3
#define DLAB_BIT 7				

//DEFINES FOR LSR
#define DR_BIT 0
#define TEMT_BIT 6



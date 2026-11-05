#include <lpc21xx.h>
#include "delay.h"
#include "interrupt.h"
#include"types.h"
#define EINT0_CH 14 //einto channel_no
volatile u32 flag;
int cnt;
void eint0_isr(void)__irq
{
	flag=1;
	//end of interrupt
	VICVectAddr=0;
	//clear EINT2 flag
	EXTINT=1<<0;
}
void enable_eint0(void)
{
	//cfg p0.16 as eint0 pin
		PINSEL1 &= ~(3 << 0);
    PINSEL1 |=  (1 << 0);
	//select eint2 as irq
	VICIntSelect &= ~(1 << EINT0_CH);
	//enable the eint0 source
		VICIntEnable=1<<EINT0_CH;
	//selct slot0 for eint0
		VICVectCntl0=(1<<5)|EINT0_CH;
	//load ISR addr into addr reg
		VICVectAddr0=(unsigned int)eint0_isr;
	//eint0 trigger at Edge
		EXTMODE |= 1<<0;
	//raising Edge
		EXTPOLAR &= ~(1<<0);
}

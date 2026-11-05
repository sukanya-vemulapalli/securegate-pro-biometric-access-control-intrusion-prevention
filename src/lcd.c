#include <LPC21xx.h>
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "delay.h"

void WriteLCD(u8 byte)
{
	//write to data pins
	IOCLR1=0XFFU<<LCD_DATA;
	IOSET1=byte<<LCD_DATA;
	//provide high to low enable pulse
	IOSET0=1<<LCD_EN;
	delay_us(1);
	IOCLR0=1<<LCD_EN;
	delay_ms(2);
}

void CmdLCD(u8 opcode)
{
	//clr rs pin for cmd register select
  IOCLR0=1<<LCD_RS;
  //write to cmd register via d0 to d7
  WriteLCD(opcode);	
}

void InitLCD(void)
{
	//cfg p0.17-p0.24,rs,en as gpio out pins
	IODIR1|=(0xFFU<<LCD_DATA);
	IODIR0|=((1<<LCD_RS)|(1<<LCD_EN));
	delay_ms(15);
	CmdLCD(0x30);
	delay_ms(4);
	delay_us(100);
	CmdLCD(0x30);
	delay_us(100);
	CmdLCD(0x30);
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DSP_ON_CUR_OFF);
	CmdLCD(CLEAR_LCD);
	CmdLCD(SHIFT_CUR_RIGHT);
}
void CharLCD(u8 asciiVal)
{
	//set rs pin for data register select
	IOSET0=1<<LCD_RS;
	//write to ddram via data reg via data pins
	WriteLCD(asciiVal);
}

void StrLCD(s8 *str)
{
	while(*str)
		CharLCD(*str++);
}

void U32LCD(u32 num)
{
  u8 a[10];
  s32 i=0;
  if(num==0)
  {
		CharLCD('0');
  }		
	else
	{
		while(num>0)
		{
			a[i++]=(num%10)+48;
			num/=10;
		}
		for(--i;i>=0;i--)
		 CharLCD(a[i]);
	}
}

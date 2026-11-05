#include<lpc21xx.h>
#include "main_defines.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "keypad.h"
#include "delay.h"
#include "types.h"
#include "interrupt.h"
extern volatile u32 flag;
u32 key,c;
// FOR ENTERING USER ID
int enter_user_id(void)
{
	u32 id=0;
	c=0;
	
	key=keyscan();
	delay_ms(100);
	while(1)
	{
		/* Exit immediately if interrupt occurred */
        if(flag == 1)
        {
            return -1;
        }
		if(key=='D')
		{
		return -2;
		}
		if(key=='*')
		{
			if(c>0 && c<5)
			{
			backspace();
			id/=10;
			c--;
			}
		}
		else if(key == '#')
    {
        if(c == 4)
            break;     
    }
		else if(c< 4 && key <= 9)
		{
    id=(id*10)+key;
    //U32LCD(key);
			CharLCD('*');
		c++;
		}
		key=keyscan();
		delay_ms(100);
}
	return id;
}
int enter_password(void)
{
	u32 password=0;
	c=0;
	
	key=keyscan();
	delay_ms(100);
	while(1)
	{
		/* Exit immediately if interrupt occurred */
        if(flag == 1)
        {
            return -1;
        }
		if(key=='D')
		{
		return -2;
		}
		if(key=='*')
		{
			if(c>0 && c<=5)
			{
			backspace();
			password/=10;
			c--;
			}
		}
		else if(key == '#')
    {
        if(c == 4)
            break;      // Exit only after 4 digits + '#'
    }
		else if(c<4 && key <= 9)
		{
    password=(password*10)+key;
    //U32LCD(key);
			CharLCD('*');
		c++;
		}
		key=keyscan();
		delay_ms(100);
	}
	return password;
}
//MENU OPTIONS

////FOR ENTERING USER ID IN MENU
int enter_user_id_menu(void)
{
	u32 id=0;
	c=0;
	
	key=keyscan();
	delay_ms(100);
	while(1)
	{
	if(key=='D')
		{
		return -2;
		}
		if(key=='*')
		{
			if(c>0 && c<=5)
			{
			backspace();
			id/=10;
			c--;
			}
		}
		else if(key == '#')
    {
        if(c == 4)
            break;      // Exit only after 4 digits + '#'
    }
		else if(c<4 && key <= 9)
		{
    id=(id*10)+key;
    //U32LCD(key);
	CharLCD('*');
		c++;
		}
		key=keyscan();
		delay_ms(100);
}
	return id;
}

//FOR ENTERING ADMIN PASSWORD & PASSWORD OF USERS
int enter_menu_password(void)
{
	u32 pass=0;
	c=0;
	key=keyscan();
	delay_ms(100);
	while(1)
	{
	if(key=='D')
		{
		return -2;
		}
		if(key=='*')
		{
			if(c>0 && c<=5)
			{
			backspace();
			pass/=10;
			c--;
			}
		}
		else if(key == '#')
    {
        if(c == 4)
            break;      // Exit only after 4 digits + '#'
    }
		else if(c<4  && key <= 9)
		{
    pass=(pass*10)+key;
    //U32LCD(key);
	CharLCD('*');
		c++;
		}
		key=keyscan();
		delay_ms(100);
	}
	return pass;
}



   
//FOR BACKSPACE
void backspace(void)
{
		    CmdLCD(MOV_CUR_LEFT);
        CharLCD(' ');
        CmdLCD(MOV_CUR_LEFT);
}

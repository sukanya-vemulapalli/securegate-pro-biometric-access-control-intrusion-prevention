#include <lpc21xx.h>
#include "types.h"
#include "keypad_defines.h"
u32 KPMLUT[4][4]={{1,2,3,'A'},{4,5,6,'B'},{7,8,9,'C'},{'*',0,'#','D'}};
void Init_KPM(void)
{
	IODIR1|=15<<ROW0;
}
u32 Colscan(void)
{
	if(((IOPIN1>>COL0)&15)<15)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
u32 Rowcheck(void)
{
	u32 rno;
	for(rno=0;rno<4;rno++)
	{
		IOPIN1=(IOPIN1&~(15<<ROW0))|((~(1<<rno))<<ROW0);
		if(Colscan()==0)
		{
			break;}
		}
	IOCLR1=15<<ROW0;
	return rno;
	}
u32 Colcheck(void)
{
	u32 cno;
	for(cno=0;cno<4;cno++)
	{
		if(((IOPIN1>>(COL0+cno))&1)==0)
		{
			break;
		}
	}
	return cno;
	}
u32 keyscan(void)
{
	u32 rno,cno,key;
	while(Colscan());
	rno=Rowcheck();
	cno=Colcheck();
	key=KPMLUT[rno][cno];
	while(!Colscan());
	return key;
}

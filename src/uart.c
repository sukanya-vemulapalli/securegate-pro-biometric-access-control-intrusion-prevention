#include <lpc21xx.h>
#include "types.h"
#include "uart_defines.h"
extern unsigned char buf[25]; 

void Uart_Init(void)
{

        //cfg p0.0,p0.1 as tx and rx
        //cfg p0.8,p0.9 as tx and rx
PINSEL0|=TXD0;
PINSEL0|=RXD0;
PINSEL0|=TXD1;
PINSEL0|=RXD1;

  	//CFG U0LCR FOR 8N1,DLAB ENABLE
	U0LCR = (1<<DLAB_BIT)|WORDLEN;
	//CFG U1LCR FOR 8N1,DLAB ENABLE
	U1LCR = (1<<DLAB_BIT)|WORDLEN;

	//cfg FOR SPPED/BAURD RATE
	U0DLL = DIVISOR;
	U0DLM = DIVISOR>>8;
	U1DLL = DIVISOR;
	U1DLM = DIVISOR>>8;
	//RESET DLAB BIT
	U0LCR &=~(1<<DLAB_BIT);
	U1LCR &=~(1<<DLAB_BIT);

}
//-----------------UART0-----------------------------
void U0_Tx(u8 byte)
{
        U0THR = byte;
        while(!((U0LSR>>TEMT_BIT)&1));
}

u8 U0_Rx(void)
{
        while(!((U0LSR>>DR_BIT)&1));
        return U0RBR;
}

void U0_Str(s8 *p)
{
        while(*p)
        U0_Tx(*p++);
}
void U0_TxU32(u32 num)
{
        u8 a[10];
        s32 i=0;
        if(num==0)
         U0_Tx('0');
         else
         {
                while(num>0)
                {
                        a[i++]=(num%10)+48;
                        num/=10;
                }
                for(i--;i>=0;i--)
                {
                        U0_Tx(a[i]);
                }
        }
}
void U0_TxS32(s32 num)
{
        if(num<0)
        {
                U0_Tx('-');
                num=-num;
        }
        U0_TxU32(num);
}

void U0_TxF32(f32 fnum,u32 ndp)
{
        u32 num;
        s32 i;
        if(fnum<0.0)
        {
                U0_Tx('-');
                fnum=-fnum;
        }
        num=fnum;
        U0_TxU32(num);
        U0_Tx('.');
for(i=0;i<ndp;i++)
        {
                fnum=(fnum-num)*10;
                num = fnum;
                U0_Tx(num+48);
        }
}

s8 *U0_RxStr(void)
{
        static s8 rbuf[100];
        s32 i=0;
        while(1)
        {
                rbuf[i]=U0_Rx();
                if((rbuf[i]=='\n')||(rbuf[i]=='\r'))
                {
                rbuf[i]='\0';
                break;
                }
                i++;
        }
        return rbuf;
}
//-------------------UART1-------------------------
void U1_Tx(u8 byte)
{
	U1THR = byte;
	while(!((U1LSR>>TEMT_BIT)&1));
}

u8 U1_Rx(void)
{
	while(!((U1LSR>>DR_BIT)&1));
	return U1RBR;
}

void U1_Str(s8 *p)
{
	while(*p)
	U1_Tx(*p++);
}
void U1_Str1(unsigned char n)
{
 	unsigned char j;   
    for(j=0;j<n;j++)
    {
        U1_Tx(buf[j]);
    }   
}

void U1_TxU32(u32 num)
{
	u8 a[10];
	s32 i=0;
	if(num==0)
	 U1_Tx('0');
	 else
	 {
	 	while(num>0)
		{
			a[i++]=(num%10)+48;
			num/=10;
		}
		for(i--;i>=0;i--)
		{
			U1_Tx(a[i]);
		}
	}
}

void U1_TxS32(s32 num)
{
	if(num<0)
	{
		U1_Tx('-');
		num=-num;
	}
	U1_TxU32(num);
}

void U1_TxF32(f32 fnum,u32 ndp)
{
	u32 num;
	s32 i;
	if(fnum<0.0)
	{
		U1_Tx('-');
		fnum=-fnum;
	}
	num=fnum;
	U1_TxU32(num);
	U1_Tx('.');
	for(i=0;i<ndp;i++)
	{
		fnum=(fnum-num)*10;
		num = fnum;
		U1_Tx(num+48);
	}
}

s8 *U1_RxStr(void)
{
	static s8 rbuf[100];
	s32 i=0;
	while(1)
	{
		rbuf[i]=U1_Rx();
		if((rbuf[i]=='\n')||(rbuf[i]=='\r'))
		{
		rbuf[i]='\0';
		break;
		}
		i++;
	}
	return rbuf;
}
void U1_ReceivePacket(unsigned char *p)
{
    unsigned char i;
    unsigned int packet_length;

    /* Receive first 9 bytes:
       Header(2) + Address(4) + Packet ID(1) + Length(2)
    */
    for(i = 0; i < 9; i++)
    {
        p[i] = U1_Rx();
    }

    /* Packet length is stored at byte 7 and byte 8 */
    packet_length = ((unsigned int)p[7] << 8) | p[8];

    /* Remaining bytes:
       instruction/data + checksum
    */
    for(i = 9; i < (9 + packet_length); i++)
    {
        p[i] = U1_Rx();
    }
}

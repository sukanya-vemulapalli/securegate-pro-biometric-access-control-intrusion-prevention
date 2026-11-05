//uart.h

#include "types.h"

void Uart_Init(void);
//--------uart0---------------
void U0_Tx(u8 byte);
u8 U0_Rx(void);
void U0_Str(s8 *p);
void U0_TxU32(u32 num);
void U0_TxS32(s32 num);
void U0_TxF32(f32 fnum,u32 ndp);
s8 *U0_RxStr(void);

//----------uart1------------
void U1_Tx(u8 byte);
u8 U1_Rx(void);
void U1_Str(s8 *p);
void U1_Str1(unsigned char n);
void U1_TxU32(u32 num);
void U1_TxS32(s32 num);
void U1_TxF32(f32 fnum,u32 ndp);
s8 *U1_RxStr(void);
void U1_ReceivePacket(unsigned char *p);

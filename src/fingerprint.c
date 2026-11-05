#include "uart.h"     
#include "lcd.h"   
#include "delay.h"  
#include "lcd_defines.h"

unsigned char ret,j,buf[25];
unsigned char rec_buff[25];

unsigned char collect(void)
{
    for(j=0;j<25;j++)
        buf[j]=0x00;

    buf[0]=0xEF;
    buf[1]=0x01;

    buf[2]=0xFF;
    buf[3]=0xFF;
    buf[4]=0xFF;
    buf[5]=0xFF;

    buf[6]=0x01;

    buf[7]=0x00;
    buf[8]=0x03;

    buf[9]=0x01;

    buf[10]=0x00;
    buf[11]=0x05;

    U1_Str1(12);

    U1_ReceivePacket(rec_buff);

    if(rec_buff[9]==0x00)
        return 0;
    else
        return 1;
}

unsigned char store(unsigned char id)
{
    for(j=0;j<25;j++)
        buf[j]=0x00;

    buf[0]=0xEF;
    buf[1]=0x01;

    buf[2]=0xFF;
    buf[3]=0xFF;
    buf[4]=0xFF;
    buf[5]=0xFF;

    buf[6]=0x01;

    buf[7]=0x00;
    buf[8]=0x04;

    buf[9]=0x02;

    buf[10]=id;

    buf[11]=0x00;
    buf[12]=0x07+id;

    U1_Str1(13);

    U1_ReceivePacket(rec_buff);

    if(rec_buff[9]==0x00)
        return 0;
    else
        return 1;
}

unsigned char gen_template(unsigned int pageid)
{
    unsigned int checksum;

    /* REGMODEL COMMAND */
    for(j=0; j<12; j++)
        buf[j]=0x00;

    buf[0]=0xEF;
    buf[1]=0x01;

    buf[2]=0xFF;
    buf[3]=0xFF;
    buf[4]=0xFF;
    buf[5]=0xFF;

    buf[6]=0x01;
    buf[7]=0x00;
    buf[8]=0x03;
    buf[9]=0x05;

    checksum=0x09;

    buf[10]=(checksum>>8)&0xFF;
    buf[11]=checksum&0xFF;

    U1_Str1(12);
    U1_ReceivePacket(rec_buff);

    if(rec_buff[9]!=0x00)
        return 1;


    /* STORE COMMAND */
    for(j=0; j<15; j++)
        buf[j]=0x00;

    buf[0]=0xEF;
    buf[1]=0x01;

    buf[2]=0xFF;
    buf[3]=0xFF;
    buf[4]=0xFF;
    buf[5]=0xFF;

    buf[6]=0x01;
    buf[7]=0x00;
    buf[8]=0x06;

    buf[9]=0x06;
    buf[10]=0x01;

    buf[11]=(pageid>>8)&0xFF;
    buf[12]=pageid&0xFF;

    checksum=0x000E+buf[11]+buf[12];

    buf[13]=(checksum>>8)&0xFF;
    buf[14]=checksum&0xFF;

    U1_Str1(15);
    U1_ReceivePacket(rec_buff);

    if(rec_buff[9]==0x00)
        return 0;
    else
        return 1;
}

unsigned char enroll(unsigned int pageid)
{
    CmdLCD(0x01);
    CmdLCD(0x80);
    StrLCD("PLACE FINGER  ");
    delay_ms(2000);

    ret = collect();
    delay_ms(1000);

    CmdLCD(0x80);
    StrLCD("REMOVE FINGER ");
    delay_ms(1500);

    CmdLCD(0x01);
    CmdLCD(0x80);

    if(ret == 0)
    {
        ret = store(1);

        if(ret == 0)
        {
            StrLCD("REPLACE FINGER  ");
            delay_ms(2000);

            ret = collect();
            delay_ms(1000);

            CmdLCD(0x80);
            StrLCD("REMOVE FINGER ");
            delay_ms(1500);

            CmdLCD(0x80);

            if(ret == 0)
            {
                ret = store(2);

                if(ret == 0)
                {
                    ret = gen_template(pageid);

                    if(ret == 0)
                        return 0;
                    else
                    {
                        StrLCD("FAILED IN SAVING");
                       U0_Str("FAILED IN SAVING\r\n");
						delay_ms(2000);
                        return 1;
                    }
                }
                else
                {
                    StrLCD("FAILED IN STORE2");
					U0_Str("FAILED IN STORE2\r\n");
                    delay_ms(2000);
                    return 1;
                }
            }
            else
            {
                StrLCD("FAILED IN COLLECT2");
				U0_Str("FAILED IN COLLECT2\r\n");
                delay_ms(2000);
                return 1;
            }
        }
        else
        {
            StrLCD("FAILED IN STORE1");
			U0_Str("FAILED IN STORE1\r\n");
            delay_ms(2000);
            return 1;
        }
    }
    else
    {
        StrLCD("FAILED IN COLLECT1");
		U0_Str("FAILED IN COLLECT1\r\n");
        delay_ms(2000);
        return 1;
    }
}

unsigned char delete_fp(unsigned int pageid)
{
    for(j=0;j<16;j++)
        buf[j]=0x00;    
    
    buf[0]=0xEF; //MSB
    buf[1]=0x01;//LSB Header---2 BYTES
    
    buf[2]=0xff;
    buf[3]=0xff;
    buf[4]=0xff;
    buf[5]=0xff; //LSB DEVICE ADRESS----4 BYTES
    
     
    buf[6]=0x01;//Package identifier--1 BYTE
    
    
    buf[8]=0x07;//LSB Package length--2 BYTES
  
  
    buf[9]=0x0C; //Instruction code--1 BYTE  
    
    buf[10]=pageid>>8;//pageID MSB    
    
    buf[11]=pageid;//PAGEID---2 BYTES   
    
    buf[13]=1; //No.OF ID's to be deleted
    
    
    j=0x15+pageid;
    buf[14]=j>>8;
    buf[15]=j;//LSBChecksum--2 BYTES  
    U1_Str1(16);

		U1_ReceivePacket(rec_buff);

		if(rec_buff[9]==0x00)
        return 0;  //SUCCESSFULLY DELETED
    else 
        return 1;  //ERROR IN DELETION

} 


unsigned char deleteall_fp(void)
{
    for(j=0;j<12;j++)
        buf[j]=0x00;    
    
    buf[0]=0xEF; //MSB
    buf[1]=0x01;//LSB Header---2 BYTES
    
    buf[2]=0xff;
    buf[3]=0xff;
    buf[4]=0xff;
    buf[5]=0xff; //LSB DEVICE ADRESS----4 BYTES
    
     
    buf[6]=0x01;//Package identifier--1 BYTE
    
    
    buf[8]=0x03;//LSB Package length--2 BYTES
  
  
    buf[9]=0x0D; //Instruction code--1 BYTE  
    
       
    buf[11]=0x11;//LSBC hecksum--2 BYTES  
   U1_Str1(12);

U1_ReceivePacket(rec_buff);

if(rec_buff[9]==0x00)
    return 0;
else
    return 1;
} 


unsigned int identify_fp(void)       
{    
    for(j=0;j<12;j++)
        buf[j]=0x00;    
    
    buf[0]=0xEF; //MSB
    buf[1]=0x01;//LSB Header---2 BYTES    
    buf[2]=0xff;
    buf[3]=0xff;
    buf[4]=0xff;
    buf[5]=0xff; //LSB DEVICE ADRESS----4 BYTES      
    buf[6]=0x01;//Package identifier--1 BYTE     
    buf[8]=0x08;//LSB Package length--2 BYTES  
    buf[9]=0x04; //Instruction code--1 BYTE     
    buf[10]=0x01;//BUFFER ID    
    buf[12]=0x00;//START PAGE ADDRESS--2 BYTES    
   buf[13]=0x00;
buf[14]=0x0A;

buf[15]=0x00;
buf[16]=0x18;
		
    U1_Str1(17);   
 
    U1_ReceivePacket(rec_buff);
    if(rec_buff[9]==0x00)
    {
        j=rec_buff[10];
        j=(j<<8)|rec_buff[11];
        return (j);  //SUCCESSFULLY IDENTIFIED FP
    }
    else 
        return 0;  //ERROR IN DELETION
}


unsigned int search_fp()  
{   
    ret=collect();     
    delay_ms(1000);
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("                    ");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("REMOVE FINGER"); 
    delay_ms(1500);  
    CmdLCD(0x01);
    CmdLCD(0x80);
    if(ret==0)
    {
        ret=store(1);     
        if(ret==0)
        {
            ret=identify_fp();  
            return(ret);
        }                  
        else
        {
            StrLCD("FAILED IN STORING");
			U0_Str("FAILED IN STORING\r\n");
            delay_ms(2000); 
            return 0;    
        } 
    }   
    else
    {
        StrLCD("FAILED IN IDENTIFING");
		U0_Str("FAILED IN IDENTIFING\r\n");
        delay_ms(2000); 
        return 0; 
    } 
}

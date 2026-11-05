//i2c_eeprom.c
#include "types.h"
#include "i2c.h"
#include "delay.h"
void i2c_eeprom_bytewrite(u8 slaveAddr,
                          u16 memAddr,
                          u8 dat)
{
    i2c_start();

    i2c_write(slaveAddr << 1);

    // Send 16-bit memory address
    i2c_write((memAddr >> 8) & 0xFF);
    i2c_write(memAddr & 0xFF);

    // Send data
    i2c_write(dat);

    i2c_stop();

    delay_ms(10);
}

u8 i2c_eeprom_randomread(u8 slaveAddr,
                         u16 memAddr)
{
    u8 dat;

    i2c_start();

    i2c_write(slaveAddr << 1);

    // Send 16-bit memory address
    i2c_write((memAddr >> 8) & 0xFF);
    i2c_write(memAddr & 0xFF);

    // Repeated start
    i2c_restart();

    i2c_write((slaveAddr << 1) | 1);

    dat = i2c_nack();

    i2c_stop();

    return dat;
}
void i2c_eeprom_pagewrite(u8 slaveAddr,
	                        u8 wBuffStartAddr,
                          u8 *p,
                          u8 nBytes)
{
  u8 i;
  i2c_start();	
  i2c_write(slaveAddr<<1);
	i2c_write(wBuffStartAddr);
	for(i=0;i<nBytes;i++)
	{
	   i2c_write(p[i]);       
	}
	i2c_stop();
	delay_ms(10);
}			 

void i2c_eeprom_seqread(u8 slaveAddr,
	                      u8 rBuffStartAddr,
                        u8 *p,
                        u8 nBytes)
{
	u8 i;
	i2c_start();	
  i2c_write(slaveAddr<<1);
	i2c_write(rBuffStartAddr); 
	i2c_restart();	
	i2c_write(slaveAddr<<1|1); 
	for(i=0;i<nBytes-1;i++)
	{
    p[i]=i2c_mack();	
	}
	p[i]=i2c_nack();
	i2c_stop();
}

//i2c_eeprom.h
#include "types.h"

void i2c_eeprom_bytewrite(u8 slaveAddr,
                          u16 memAddr,
                          u8 dat);

u8 i2c_eeprom_randomread(u8 slaveAddr,
                         u16 memAddr);

void i2c_eeprom_pagewrite(u8 slaveAddr,
	                         u8 wBuffStartAddr,
                           u8 *p,
                           u8 nBytes);

void i2c_eeprom_seqread(u8 slaveAddr,
	                      u8 rBuffStartAddr,
                        u8 *p,
                        u8 nBytes);

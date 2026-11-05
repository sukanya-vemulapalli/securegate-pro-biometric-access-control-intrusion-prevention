#include "database.h"
#include "i2c_eeprom.h"
#include <string.h>

void SaveUser(u8 index,USER *u)
{
    u8 addr;
    u8 *p;
    u8 i;

    addr=USER_START_ADDR+(index*RECORD_SIZE);

    p=(u8 *)u;

    for(i=0;i<RECORD_SIZE;i++)
    {
        i2c_eeprom_bytewrite(0x50,
                             addr+i,
                             p[i]);
    }
}

void ReadUser(u8 index,USER *u)
{
    u8 addr;
    u8 *p;
    u8 i;

    addr=USER_START_ADDR+(index*RECORD_SIZE);

    p=(u8 *)u;

    for(i=0;i<RECORD_SIZE;i++)
    {
        p[i]=i2c_eeprom_randomread(0x50,
                                   addr+i);
    }
}

u8 SearchUser(u32 id, USER *u)
{
    u8 index;

    for(index = 0; index < MAX_USERS; index++)
    {
        ReadUser(index, u);

        if(u->status == ACTIVE_RECORD)
        {
            if(u->user_id == id)
            {
                return index;
            }
        }
    }

    return 0xFF;
}

u8 GetFreeIndex(void)
{
    USER u;
    u8 index;

    for(index = 0; index < MAX_USERS; index++)
    {
        ReadUser(index, &u);

        if(u.status != ACTIVE_RECORD)
        {
            return index;
        }
    }

    return 0xFF;
}

u8 GetFreeFingerID(void)
{
    USER u;
    u8 finger;
    u8 index;
    u8 found;

    for(finger = 1; finger <= MAX_USERS; finger++)
    {
        found = 0;

        for(index = 0; index < MAX_USERS; index++)
        {
            ReadUser(index, &u);

            if((u.status == ACTIVE_RECORD) &&
               (u.finger_id == finger))
            {
                found = 1;
                break;
            }
        }

        if(found == 0)
            return finger;
    }

    return 0xFF;
}

u8 AddUser(USER *u)
{
    USER temp;
    u8 index;

    /* Check duplicate user ID */
    if(SearchUser(u->user_id, &temp) != 0xFF)
        return 0;

    /* Find free EEPROM record */
    index = GetFreeIndex();

    if(index == 0xFF)
        return 0;

    /* Finger ID should already be assigned */
    if(u->finger_id == 0xFF)
        return 0;

    u->status = ACTIVE_RECORD;

    SaveUser(index, u);

    return 1;
}
u8 DeleteUser(u32 id)
{
    USER u;
    u8 index;

    index = SearchUser(id, &u);

    if(index == 0xFF)
        return 0;

    u.status = DELETE_RECORD;

    SaveUser(index, &u);

    return 1;
}

void SaveAdminPassword(u32 pass)
{
    u8 *p;
    u8 i;

    p = (u8 *)&pass;

    for(i = 0; i < 4; i++)
    {
        i2c_eeprom_bytewrite(0x50, ADMIN_ADDR+i, p[i]);
    }
}

u32 ReadAdminPassword(void)
{
    u32 pass;
    u8 *p;
    u8 i;

    p = (u8 *)&pass;

    for(i = 0; i < 4; i++)
    {
        p[i] = i2c_eeprom_randomread(0x50, ADMIN_ADDR+i);
    }

    return pass;
}

u8 UpdatePassword(u32 id,u32 newpass)
{
    USER u;

    u8 index;

    index=SearchUser(id,&u);

    if(index==0xFF)
        return 0;

    u.password=newpass;

    SaveUser(index,&u);

    return 1;
}


u8 VerifyAdminPassword(u32 pass)
{
    if(pass == ReadAdminPassword())
        return 1;

    return 0;
}
void ClearDatabase(void)
{
    u8 i;

    // Clear user records only
    for(i = USER_START_ADDR;
        i < USER_START_ADDR + (MAX_USERS * RECORD_SIZE);
        i++)
    {
        i2c_eeprom_bytewrite(0x50, i, 0xFF);
    }

    // Reset admin password
    SaveAdminPassword(DEFAULT_ADMIN_PASSWORD);
}


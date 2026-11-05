
#include "types.h"

#define MAX_USERS      9
#define RECORD_SIZE    10

#define EMPTY_RECORD   0xFF
#define ACTIVE_RECORD  0x01
#define DELETE_RECORD  0x00

#define ADMIN_ADDR     0

#define USER_START_ADDR 10

#define DEFAULT_ADMIN_PASSWORD 1234
typedef struct
{
    u32 user_id;
    u32 password;
    u8 finger_id;
    u8 status;

}USER;

/* User Functions */

void SaveUser(u8 index,USER *u);

void ReadUser(u8 index,USER *u);

u8 SearchUser(u32 id,USER *u);

u8 AddUser(USER *u);

u8 DeleteUser(u32 id);

u8 UpdatePassword(u32 id,u32 newpass);

u8 GetFreeIndex(void);

u8 GetFreeFingerID(void);

/* Admin Functions */

void SaveAdminPassword(u32 pass);

u32 ReadAdminPassword(void);

u8 VerifyAdminPassword(u32 pass);

void ClearDatabase(void);


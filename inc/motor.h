#ifndef __MOTOR_H
#define __MOTOR_H

#include "types.h"

void Motor_Init(void);   /* pins as outputs, motor stopped (call once at start-up) */
void Door_Open(void);    /* forward : access granted               */
u8   Door_Close(void);   /* reverse : 1 = door closed, 0 = limit switch never closed (time-out) */

#define MOTOR_IN1          (1UL << 4)
#define MOTOR_IN2          (1UL << 5)
#define MOTOR_EN_ENABLED   1
#define MOTOR_EN           (1UL << 7)

#define DOOR_SWITCH_ENABLED 0
#define DOOR_SW             (1UL << 6)

#define DOOR_OPEN_MS     3000       /* forward run time                         */
#define DOOR_HOLD_MS     2000       /* door stays open                          */
#define DOOR_CLOSE_MS    3000       /* reverse run time (or time-out with switch) */
#endif /* __MOTOR_H */

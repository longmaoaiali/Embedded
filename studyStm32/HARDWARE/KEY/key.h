#ifndef __KEY_H
#define __KEY_H

#include "sys.h"
#include "delay.h"

#define KEY_UP PAin(0)
#define KEY1 PEin(3)
#define KEY0 PEin(4)

#define KEY0_PRES 1 
#define KEY1_PRES 2 
#define KEYUP_PRES 3 

void KEY_Init(void);
u8 KEY_Scan(void);
#endif

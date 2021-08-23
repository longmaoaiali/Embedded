#ifndef __PWM_H
#define __PWM_H

#include "sys.h"

#define LED0_PWM_VAL TIM3->CCR2

void Timer3_PWM_Init(u16 arr,u16 psc);


#endif

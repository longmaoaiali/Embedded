#include "beep.h"

void BEEP_Init(void)
{
	RCC->APB2ENR|=1<<3; //enable APB2ENR clock
	GPIOB->CRH&=0XFFFFFFF0; 
	GPIOB->CRH|=0X00000003; //PB8 output
	BEEP=0; 
}

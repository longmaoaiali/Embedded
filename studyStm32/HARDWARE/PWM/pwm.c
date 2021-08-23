#include "pwm.h"

void Timer3_PWM_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR |= 1<<1;
	RCC->APB2ENR |= 1<<3;//enable timer clock and IO clock
	
	GPIOB->CRL &= 0XFF0FFFFF;
	GPIOB->CRL |= 0X00B00000;//PB5 output
	
	RCC->APB2ENR |= 1<<0;
	AFIO->MAPR &= 0XFFFFF3FF;
	AFIO->MAPR |= 1<<11;//part remap
	
	TIM3->ARR=arr; //set arr
	TIM3->PSC=psc; //set psc
	TIM3->CCMR1|=7<<12; //CH2 PWM2 mode
	TIM3->CCMR1|=1<<11; //CH2 enable
	TIM3->CCER|=1<<4; //OC2 output enable 
	TIM3->CR1=0x0080; //ARPE enable
	TIM3->CR1|=0x01; //enable timer3
	
	
}

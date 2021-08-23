#include "timer.h"
#include "led.h"

void TIM3_IRQHandler(void)
{
	if(TIM3->SR&0X0001){
		LED0 = !LED0;
	}
	
	TIM3->SR&=~(1<<0); //clean interrupt flag
}

void Timer_Init(u16 arr,u16 psc){
	RCC->APB1ENR |= 1<<1;
	TIM3->ARR = arr;
	TIM3->PSC = psc;
	TIM3->DIER |= 1<<0;
	TIM3->CR1 |= 0x01;
	MY_NVIC_Init(1,3,TIM3_IRQn,2);
}

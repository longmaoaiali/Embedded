#include "led.h"

void LED_Init(void)
{
	RCC->APB2ENR|=1<<3; 
	RCC->APB2ENR|=1<<6; 
	GPIOB->CRL&=0XFF0FFFFF; 
	GPIOB->CRL|=0X00300000;
	GPIOB->ODR|=1<<5; //PB5 output high Level 
	GPIOE->CRL&=0XFF0FFFFF;
	GPIOE->CRL|=0X00300000;
	GPIOE->ODR|=1<<5; 
}
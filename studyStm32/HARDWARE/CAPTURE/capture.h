#include "capture.h"

void TIM5_Cap_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<3; //enable timer5 clock
	RCC->APB2ENR|=1<<2; //enable IO clock
	GPIOA->CRL&=0XFFFFFFF0; 
	GPIOA->CRL|=0X00000008; //PA0 input
	GPIOA->ODR|=0<<0; 
	TIM5->ARR=arr; //reload
	TIM5->PSC=psc; //pre freq
	TIM5->CCMR1|=1<<0; 
	TIM5->CCMR1|=0<<4; 
	TIM5->CCMR1|=0<<10; 
	TIM5->CCER|=0<<1; 
	TIM5->CCER|=1<<0; 
	TIM5->DIER|=1<<1; 
	TIM5->DIER|=1<<0; 
	TIM5->CR1|=0x01; 
	MY_NVIC_Init(2,0,TIM5_IRQn,2);
}


void TIM5_IRQHandler(void)
{ 
	u16 tsr;
	tsr=TIM5->SR;
	if((TIM5CH1_CAPTURE_STA&0X80)==0)
	{
		if(tsr&0X01)//timer out
		{ 
			if(TIM5CH1_CAPTURE_STA&0X40)//get high level 
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)
				{
					TIM5CH1_CAPTURE_STA|=0X80;
					TIM5CH1_CAPTURE_VAL=0XFFFF;
				}else TIM5CH1_CAPTURE_STA++;//timer out count increase
			} 
		}
		
		if(tsr&0x02)//capture event
		{
			if(TIM5CH1_CAPTURE_STA&0X40) //get high level 
			{
				TIM5CH1_CAPTURE_STA|=0X80; 
				TIM5CH1_CAPTURE_VAL=TIM5->CCR1;
				TIM5->CCER&=~(1<<1); 
			}else //get low level 
			{
				TIM5CH1_CAPTURE_STA=0; 
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;
				TIM5->CNT=0; 
				TIM5->CCER|=1<<1; 
			} 
		} 
	}

	TIM5->SR=0;
}


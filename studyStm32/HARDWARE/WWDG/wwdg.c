#include "wwdg.h"
#include "led.h"

u8 WWDG_CNT=0x7f;
void WWDG_Init(u8 tr,u8 wr,u8 fprer) 
{ 
	RCC->APB1ENR|=1<<11; //enable wwdg clock
	WWDG_CNT=tr&WWDG_CNT; 
	WWDG->CFR|=fprer<<7; 
	WWDG->CFR&=0XFF80; 
	WWDG->CFR|=wr; //init win value
	WWDG->CR|=WWDG_CNT; //init count value 
	WWDG->CR|=1<<7; 
	
	MY_NVIC_Init(2,3,WWDG_IRQn,2);
	WWDG->SR=0X00; 
	WWDG->CFR|=1<<9; //enable interrupt
}

//reload WWDG
void WWDG_Set_Counter(u8 cnt) 
{ 
	WWDG->CR =(cnt&0x7F);
} 

//interrupt handler
void WWDG_IRQHandler(void) 
{ 
	WWDG_Set_Counter(WWDG_CNT);
	WWDG->SR=0X00;//clean interrupt flag
	LED1=!LED1; 
}

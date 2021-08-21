#include "exti.h"
#include "key.h"
#include "delay.h"
#include "beep.h"
#include "led.h"

void EXTI_Init(void){
	//init PA0 PE3 PE4 input pin
	KEY_Init();
	
	//参数:
	//GPIOx:0~6,代表GPIOA~G
	//BITx:需要使能的位;
	//TRIM:触发模式,1,下升沿;2,上降沿;3，任意电平触发
	//该函数一次只能配置1个IO口,多个IO口,需多次调用
	//该函数会自动开启对应中断,以及屏蔽线 
	Ex_NVIC_Config(GPIO_A,0,RTIR);
	Ex_NVIC_Config(GPIO_E,3,FTIR);
	Ex_NVIC_Config(GPIO_E,4,FTIR);
	
	//设置NVIC 
	//NVIC_PreemptionPriority:抢占优先级
	//NVIC_SubPriority       :响应优先级
	//NVIC_Channel           :中断编号
	//NVIC_Group             :中断分组 0~4
	MY_NVIC_Init(2,3,EXTI0_IRQn,2);
	MY_NVIC_Init(2,1,EXTI3_IRQn,2);
	MY_NVIC_Init(2,0,EXTI4_IRQn,2);
	
	
}

void EXTI0_IRQHandler(void){
	delay_ms(10);
	if(KEY_UP==1){
		BEEP = !BEEP;
	}
	
	EXTI->PR = 1<<0;//clean LINE0 interrupt flag
}

void EXTI3_IRQHandler(void){
	delay_ms(10);
	if(KEY1==0){
		LED0=!LED0; LED1=!LED1;
	}
	
	EXTI->PR = 1<<3;//clean LINE3 interrupt flag
}

void EXTI4_IRQHandler(void){
	delay_ms(10);
	if(KEY0==0){
		LED0=!LED0; 
	}
	
	EXTI->PR = 1<<4;//clean LINE3 interrupt flag
}




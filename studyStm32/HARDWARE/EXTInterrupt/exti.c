#include "exti.h"
#include "key.h"
#include "delay.h"
#include "beep.h"
#include "led.h"

void EXTI_Init(void){
	//init PA0 PE3 PE4 input pin
	KEY_Init();
	
	//����:
	//GPIOx:0~6,����GPIOA~G
	//BITx:��Ҫʹ�ܵ�λ;
	//TRIM:����ģʽ,1,������;2,�Ͻ���;3�������ƽ����
	//�ú���һ��ֻ������1��IO��,���IO��,���ε���
	//�ú������Զ�������Ӧ�ж�,�Լ������� 
	Ex_NVIC_Config(GPIO_A,0,RTIR);
	Ex_NVIC_Config(GPIO_E,3,FTIR);
	Ex_NVIC_Config(GPIO_E,4,FTIR);
	
	//����NVIC 
	//NVIC_PreemptionPriority:��ռ���ȼ�
	//NVIC_SubPriority       :��Ӧ���ȼ�
	//NVIC_Channel           :�жϱ��
	//NVIC_Group             :�жϷ��� 0~4
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




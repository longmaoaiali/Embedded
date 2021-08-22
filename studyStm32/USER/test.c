#include "delay.h"
#include "sys.h"
#include "led.h"
#include "usart.h"
#include "exti.h"
#include "iwdg.h"
#include "key.h"

int main()
{
	Stm32_Clock_Init(9);
	delay_init(72);
	LED_Init();
	uart_init(72,115200);
	EXTI_Init();
	KEY_Init();
	delay_ms(1000);
	IWDG_Init(4,625);
	LED0=0;
	
	while(1){
		if(KEY_Scan()!=0){
			IWDG_Feed();
		}
		
		delay_ms(10);
	
	}
}


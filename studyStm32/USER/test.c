#include "delay.h"
#include "sys.h"
#include "led.h"
#include "usart.h"
#include "exti.h"
#include "beep.h"

int main()
{
	Stm32_Clock_Init(9);
	delay_init(72);
	LED_Init();
	uart_init(72,115200);
	EXTI_Init();
	BEEP_Init();
	
	while(1){
		printf("OK Liuyu\r\n");
		delay_ms(1000);
	}
	
}


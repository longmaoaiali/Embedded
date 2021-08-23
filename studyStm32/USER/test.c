#include "delay.h"
#include "sys.h"
#include "led.h"
#include "timer.h"


int main()
{
	Stm32_Clock_Init(9);
	delay_init(72);
	LED_Init();
	
	Timer_Init(4999,7199); //500ms
	while(1){
		LED1=!LED1;
		delay_ms(200);
	}
}


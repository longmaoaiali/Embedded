#include "delay.h"
#include "sys.h"
#include "led.h"
#include "beep.h"

int main()
{
	Stm32_Clock_Init(9);
	delay_init(72);
	LED_Init();
	BEEP_Init();
	
	while(1){	
		LED0 = 0;
		LED1 = 1;
		BEEP = 0;
		delay_ms(500);
		LED0 = 1;
		LED1 = 0;
		BEEP = 1;
		delay_ms(500);
	}
}


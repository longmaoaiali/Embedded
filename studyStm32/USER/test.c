#include "delay.h"
#include "sys.h"
#include "led.h"
#include "beep.h"
#include "key.h"

int main()
{
	Stm32_Clock_Init(9);
	delay_init(72);
	LED_Init();
	BEEP_Init();
	KEY_Init();
	
	while(1){	
		int key = KEY_Scan();
		switch(key){
			case 1:
				LED0 = !LED0;
				break;
			case 2:
				LED1= !LED1;
				break;
			case 3:
				BEEP = !BEEP;
				break;
			default:
				break;
		}
		
		delay_ms(10);
	}
}


#include "delay.h"
#include "sys.h"
#include "led.h"
#include "wwdg.h"


int main()
{
	Stm32_Clock_Init(9);
	delay_init(72);
	LED_Init();
	LED0=0;//open led

	delay_ms(1000);
	//count value , left win value, pfreq=8
	WWDG_Init(0X7F,0X5F,3);
	
	
	while(1){
		LED0=1;//close led
	}
}


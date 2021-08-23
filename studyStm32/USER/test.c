#include "delay.h"
#include "sys.h"
#include "led.h"
#include "pwm.h"


int main()
{
	u16 led0pwmval=0; u8 dir=1;
	Stm32_Clock_Init(9);
	delay_init(72);
	LED_Init();
	
	Timer3_PWM_Init(899,0);
	while(1){
		delay_ms(10);
		if(dir) led0pwmval++;
		else led0pwmval--;
		
		if(led0pwmval>300) dir=0;
		if(led0pwmval==0) dir=1;
		LED0_PWM_VAL = led0pwmval;
	}
}


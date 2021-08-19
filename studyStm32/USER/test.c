#include "delay.h"
#include "sys.h"
#include "led.h"
#include "usart.h"

int main()
{
	Stm32_Clock_Init(9);
	delay_init(72);
	LED_Init();
	uart_init(72,115200);
	int len,i,times=0;
	
	while(1){
		if(USART_RX_STA & 0x8000){
				len = USART_RX_STA & 0x3FFF;
				printf("you send message is \r\n\r\n");
				for(i=0;i<len;i++){
					USART1->DR = USART_RX_BUF[i];
					while((USART1->SR&0x40)==0);//wait until send over
				}
				printf("\r\n");
				USART_RX_STA=0;
		} else {
			times++;
			if((times%30)==0){
				LED0 = !LED0;
			}
			delay_ms(30);
		}
	}
	
}


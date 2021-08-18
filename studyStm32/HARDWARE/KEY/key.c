#include "key.h"

void KEY_Init(void)
{
	//PA0 PE3 PE4
	//enable IO clock RCC_APB2ENR
	RCC->APB2ENR |= 1<<2;
	RCC->APB2ENR |= 1<<6;
	
	//configure IO input  PA0 PE3 PE4
	GPIOA->CRL &= 0xFFFFFFF0;
	GPIOA->CRL |= 1<<3;
	
	GPIOE->CRL &= 0xFFF00FFF;
	GPIOE->CRL |= 0x00088000;
	
	GPIOE->ODR |= 3<<3;
	
	
	//0x4001 0800 - 0x4001 0BFF GPIOA
	
}

//#define KEY_UP PAin(0)
//#define KEY1 PEin(3)
//#define KEY0 PEin(4)

u8 KEY_Scan(){
	if(KEY_UP==1||KEY1==0||KEY0==0){
		delay_ms(100);
		if(KEY_UP==1){
			return 1;
		} else if(KEY1==0) {
			return 2;
		} else if(KEY0==0) {
			return 3;
		}
	}
	
	return 0;
	
}

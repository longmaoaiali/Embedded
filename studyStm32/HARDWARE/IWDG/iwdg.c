#include "iwdg.h"

void IWDG_Init(u8 pere,u16 rlr){
	//enable write
	IWDG->KR=0X5555;
	
	IWDG->PR=pere;
	IWDG->RLR=rlr;
	
	//reload RLR and enable IWDG
	IWDG->KR=0XAAAA;
	IWDG->KR=0XCCCC;
}

void IWDG_Feed(void){
	//reload
	IWDG->KR=0XAAAA;
}


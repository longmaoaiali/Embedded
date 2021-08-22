#ifndef __WWDG_H
#define __WWDG_H

#include "sys.h"

//tr :count value
//wr :windows value
//fprer :div freq
//Fwwdg=PCLK1/(4096*2^fprer)
void WWDG_Init(u8 tr,u8 wr,u8 fprer);



#endif

#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

//位定义
#define LED0 PBout(5)	// PA8
#define LED1 PEout(5)	// PD2	

void LED_Init(void);//初始化

		 				    
#endif


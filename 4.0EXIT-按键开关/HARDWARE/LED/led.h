#ifndef __LED_H__
#define __LED_H__

#include "sys.h"    //位带操作函数库

#define LED0 PBout(5)  //led0接PB5  位带操作
#define LED1 PCout(5)  //led1接PE5

void LED_Init(void);  //LED初始化


#endif

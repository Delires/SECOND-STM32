#ifndef __LED_H__
#define __LED_H__

#include "sys.h"    //λ������������

#define LED0 PBout(5)  //led0��PB5  λ������
#define LED1 PCout(5)  //led1��PE5

void LED_Init(void);  //LED��ʼ��


#endif

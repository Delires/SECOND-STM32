#ifndef DelayPriacy__h__
#define DelayPriacy__h__

#include "stm32f10x.h"
#include "sys.h"

void delay_init(void); //初始化函数声明
void delay_ms(u16 nms); //ms毫秒延时函数
void delay_us(u32 nus);  //us微秒延时函数

void Delay(uint32_t Count);


#endif



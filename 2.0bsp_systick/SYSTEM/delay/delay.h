#ifndef __delay_h__
#define __delay_h__

#include "sys.h"  //使用滴答定时器的普通计数进行延时管理


void delay_init(void); //初始化
void delay_ms(u16 nms); //毫秒延时函数
void delay_us(u32 nus);  //纳秒级延时函数


#endif


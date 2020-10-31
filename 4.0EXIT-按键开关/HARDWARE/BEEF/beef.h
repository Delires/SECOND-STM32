#ifndef __BEEF_H__
#define __BEEF_H__

#include "sys.h" //位带操作函数库

#define BEEP PBout(8)   //宏定义BEEP 是位带操作PB8引脚

void BEEP_Init(void);   //蜂鸣器初始化



#endif

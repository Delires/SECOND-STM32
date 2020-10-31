#ifndef __timer_General_h__
#define __timer_General_h__

#include "stm32f10x.h"   //这都不用想 配置啥都得使用这个

/**声明一个结构体  这种以后要多用  此处不是定义 不分配内存的***/
typedef  struct
{
	u16 Period_Value;
	u16 Time_Value;
	u8 CaptureStart_Bit;
	u8 CaptureStop_Bit;
}GPIO_Genral_Capture;

extern GPIO_Genral_Capture Capture_value;  //声明全局各个函数都可调用

extern u8 TIM2CH1_CAPTURE_STA;
extern u8 TIM2CH1_CAPTURE_VAL;

#define ZhengDian_Open   0    //打开关闭正点/自己的代码
#define Myslef_Open      1



void TIM_General_Capture_Init(void);

#endif

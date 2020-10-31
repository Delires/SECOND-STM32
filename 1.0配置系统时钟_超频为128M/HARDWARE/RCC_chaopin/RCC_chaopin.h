#ifndef __RCC_chaopin_H
#define __RCC_chaopin_H

#include "stm32f10x.h"
//配置HSE为system频率  输入形参2~16
void HSE_SetClock(uint32_t RCC_PLLMul_x);

//配置HSI为system频率  输入形参2~16
void HSI_SetClock(uint32_t RCC_PLLMul_x);

//复用PA8查看MCO波形  来源于system的配置
void MCO_Init(void); 
		 				    
#endif




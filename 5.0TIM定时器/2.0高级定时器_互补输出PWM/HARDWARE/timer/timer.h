#ifndef __timer_h__
#define __timer_h__

#include "sys.h"
#include "stm32f10x.h"

extern u16 time_save;
void Config_Base_TIM6(u16 arr, u16 psc); //TIM6����+�ж�ʹ��
void NVIC_Base_TIM6(void); //�������ȼ� 	
	
#endif


#ifndef __RCC_chaopin_H
#define __RCC_chaopin_H

#include "stm32f10x.h"
//����HSEΪsystemƵ��  �����β�2~16
void HSE_SetClock(uint32_t RCC_PLLMul_x);

//����HSIΪsystemƵ��  �����β�2~16
void HSI_SetClock(uint32_t RCC_PLLMul_x);

//����PA8�鿴MCO����  ��Դ��system������
void MCO_Init(void); 
		 				    
#endif




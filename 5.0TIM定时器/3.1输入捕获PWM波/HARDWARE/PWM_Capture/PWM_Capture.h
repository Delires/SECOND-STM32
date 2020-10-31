#ifndef __PWM_Capture_H
#define __PWM_Capture_H 
#include "stm32f10x.h"

extern u16 Value1_CCR1; 
extern u16 Value2_CCR2;
extern __O float DutyCycle; 
extern __O float Frequency;	
//__O 即 volatile 变量都要这样定义 这样值就不再编译中被优化

void TIM_Advance_CapturePWM_Init(void);

		 				    
#endif


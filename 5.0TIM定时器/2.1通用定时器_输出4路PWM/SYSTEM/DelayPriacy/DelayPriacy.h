#ifndef DelayPriacy__h__
#define DelayPriacy__h__

#include "stm32f10x.h"
#include "sys.h"

void delay_init(void); //��ʼ����������
void delay_ms(u16 nms); //ms������ʱ����
void delay_us(u32 nus);  //us΢����ʱ����

void Delay(uint32_t Count);


#endif



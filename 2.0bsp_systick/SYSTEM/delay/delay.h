#ifndef __delay_h__
#define __delay_h__

#include "sys.h"  //ʹ�õδ�ʱ������ͨ����������ʱ����


void delay_init(void); //��ʼ��
void delay_ms(u16 nms); //������ʱ����
void delay_us(u32 nus);  //���뼶��ʱ����


#endif


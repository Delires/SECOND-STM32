#ifndef __timer_General_h__
#define __timer_General_h__

#include "stm32f10x.h"   //�ⶼ������ ����ɶ����ʹ�����

/**����һ���ṹ��  �����Ժ�Ҫ����  �˴����Ƕ��� �������ڴ��***/
typedef  struct
{
	u16 Period_Value;
	u16 Time_Value;
	u8 CaptureStart_Bit;
	u8 CaptureStop_Bit;
}GPIO_Genral_Capture;

extern GPIO_Genral_Capture Capture_value;  //����ȫ�ָ����������ɵ���

extern u8 TIM2CH1_CAPTURE_STA;
extern u8 TIM2CH1_CAPTURE_VAL;

#define ZhengDian_Open   0    //�򿪹ر�����/�Լ��Ĵ���
#define Myslef_Open      1



void TIM_General_Capture_Init(void);

#endif

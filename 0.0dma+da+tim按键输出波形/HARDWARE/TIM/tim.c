#include "tim.h"

//�趨������һ����Ա�Ĵ���ʱ��
//DMA��������DA�Ĵ����ᣬȻ��ÿTIM��ʱһ��DA�ʹ���һ�Σ�����һ�ξ����һ�������ʱ�䡣Ȼ��һֱ�������������ֵľ���һ��Ƶ�ʵĲ���
//���й������£�tim����һ�Σ�da�����һ�����ݣ�dma�ڰ����һ��
             //���Դ˷�������
void Tim2_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//54931*65535/(72000000/4)=200us=200s �Ǵ�� �� ���ֲ᲻��ϸ
//2000*7200/72000000=0.2s  ����Ŷ� ��72000000���������s
	TIM_TimeBaseInitStruct.TIM_Period = 2000/f;   //ͨ�����������ÿ���������ı䲨�Σ�����ͨ��һ���������ı�               
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;        
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_Update);  //��ʱ�����´������������������һ����ʱ����DA�������
	TIM_Cmd(TIM2, ENABLE); //ʹ��
}



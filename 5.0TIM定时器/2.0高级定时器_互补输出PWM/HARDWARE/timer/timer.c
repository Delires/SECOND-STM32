/**
��ʼ��������ʱ��TIM6 /TIM7
��ʱ500ms
**/
#include "timer.h"

u16 time_save=0;

/************************************** 
����TIM6�Ĵ��� ������ RCCʹ�� ���� ���ܿ���
������� arr�Զ���װ��ֵ   pscԤ��Ƶϵ��
����һ��ʱ��=72M/(psc+1)  ��������=arr*����һ��ʱ��
 //psc=7199  ����һ��1000hz=1ms
**************************************************/
void Config_Base_TIM6(u16 arr, u16 psc) 
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct; 
//������--RCCʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

//������--����	
	//������ʱ��������ʱ�ӷ�Ƶ
	//TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	//������ʱ�������ڼ���ģʽ   �ӿ⺯�����Կ���
	//TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; //������ʱ��ֻ�����ϼ���
	TIM_TimeBaseInitStruct.TIM_Period= arr;                          //���ص�ARR�Ĵ�����ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;          //Ԥ��Ƶ
	//������ʱ���������ظ���������
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter =  
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);  
	
//������--���ܿ���
	TIM_Cmd(TIM6,  ENABLE); 


//�����ж�
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);   //����������жϱ�־λ

	TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE); //���������ж�

}


/*******
�����ж�NVIC �����жϷ���  NVIC_Init() ��  NVIC_PriorityGroupConfig()

�������жϾ�Ҫ�����жϺ���
********/

void NVIC_Base_TIM6(void)
{
	NVIC_InitTypeDef NVIC_InitStruct; 
  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //���ȼ�����(���ȼ�Ӧ��ֻ�ܷ�һ���飬��Ҷ������ĵȼ���)
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;  //�����ж�Դ
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =0x00;
	NVIC_Init(&NVIC_InitStruct);
}	





/********
TIM6���жϷ�����
�����������ļ���
�ϲ��裺�ж��жϱ�־  �����ж�  ����жϱ�־
*********/
void TIM6_IRQHandler()
{
	if(TIM_GetFlagStatus( TIM6, TIM_FLAG_Update) != RESET)   //if( TIM_GetFlagStatus(TIM6,TIM_FLAG_Update)==1 )
	{
          time_save++;
		  TIM_ClearFlag(TIM6,TIM_FLAG_Update);   //����������жϱ�־λ
	}			
}

//TIM_GetFlagStatus �Ƕ���־λ
//TIM_GetITStatus  �Ƕ��¼�        �������������һ���� ֻ�����ββ�ͬ    



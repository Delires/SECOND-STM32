#include "PWM_Capture.h"
#include "usart.h"
/*****************************************************************
*************TIM1��CH1(PA8)Ϊ���벶�񣬲���PWMƵ��****************
ԭ�����£�
         �ٲ�pwmƵ�ʣ�CH1ӳ�䵽IC1
		              ��һ�������ز����ж� ��¼value1 ��������ж�    
		              �ڶ����ٷ��������ز����ж��ټ�¼һ��ֵvalue2 ��������ж�  
					  (value2-value2)/1=Ƶ��ֵ
         �ٲ�pwmռ�ձȣ�CH1��ӳ�䵽IC2
		                ��ߵ�ƽʱ��PW1�������� ����ռ�ձ�   �������ж������ø�/�Ͳ��� ��ΪPWM�Ĳ���ģʽ��IC2���Զ�����ߵ�ƽ
		
		 CH1���ó�ֱ������IC1�����ش�����������  IC2����ߵ�ƽ
		         ��ֱ������IC1�����ش�����������  IC2����ߵ�ƽ
		 
ע�⣺��������� ��������Ҫ>PWM���ڣ��������ܲ�����һ��PWM���ڡ�
      ��֮��˵�����ǲ����Ƶ��<PWMƵ��  
	  ʵ�ʸ���PWMƵ�������ڣ�ʵ���е����PWMƵ��Ϊ10K~25K �����������벶��ʱ��һ������Ϊ1K
*****************************************************************/

/**
����GPIO����   TIM1 CH1(PA8)Ϊ���벶��
**/
static void TIM_Advance_GPIO_Config(void)   //���������static��ֻ������������ܵ���
 {
    GPIO_InitTypeDef GPIO_InitStruct;  

//ʹ��ʱ��+����+��������������(gpio���ò����ڿ�������)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;  //�ĳ��������� ͨ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	 
	GPIO_ResetBits(GPIOA, GPIO_Pin_8); 
}
  


/**
����TIM1 ʱ��
RCC����װ��ֵ   PSCԤ����ϵ��
ע�⣺��������� ��������Ҫ>PWM���ڣ��������ܲ�����һ��PWM���ڡ�
      ��֮��˵�����ǲ����Ƶ��<PWMƵ��  
����ΪRCC=99 PSC=71   72m/ ( (999+1)*(71+1) ) =1Khz
**/
static void TIM_Advance_Base_Config(u16 arr, u16 psc)   //���������static��ֻ������������ܵ���
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); ; //��ʹ��һ��TIM1  
//����TIM1ʱ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //�����ú�Tdts�й� CR1��CKDλ  Tdts������ʱ���ʱ��  
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00; //�ظ��Ĵ�������
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);  
//ʹ�ܹ���
	TIM_Cmd(TIM1, ENABLE); 
	
}




/**
����TIM1��CH1ΪPWM���벶�� (�������벶��CH1 �� ��ģʽ�Ĵ���)
	   ��������PWM����ģʽʱ,ֻ��Ҫ���ô����źŵ���һ·���ɣ����ڲ������ڣ�
       ����һ·�����ڲ���ռ�ձȣ�����Ӳ���Դ����ã�����Ҫ������

�����ϸ��ա��ٷ��ֲ�13.3.7 PWM����ģʽ��
**/
static void TIM_Advance_CapturePWM_Config()   
{
/**���õ�һ·IC1��PWM ����**/
	TIM_ICInitTypeDef TIM_ICInitStruct;
	 
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0x00;  //���˲�
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; //��������Ч
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;  //����ÿ���ٴδ����¼����㲶��һ��
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //ֱ��
	TIM_PWMIConfig(TIM1, &TIM_ICInitStruct); 
//TIMx_CCR1��ֵ������     TIMx_CCR2��ֵ������
	
/**���õڶ�·IC2��PWM ����**/	
    // ��������PWM����ģʽʱ,ֻ��Ҫ���ô����źŵ���һ·���ɣ����ڲ������ڣ�
    // ����һ·�����ڲ���ռ�ձȣ�����Ӳ���Դ����ã�����Ҫ������
	
/**���ô�ģʽ�Ĵ���**/ //��ģʽ�Ĵ������൱����һ��һ���յ��ź�����λ������������
//ѡ����Ч�Ĵ��������źţ���TIMx_SMCR�Ĵ����е�TS=101(ѡ��TI1FP1)��
	TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1); //�������벶��TI1FP1
	TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);// Rising edge of the selected trigger signal (TRGI) re-initializes the counter and triggers an update of the registers.
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);//ʹ�ܴ�ģʽ 

/**ʹ���ж�   ����ж�  ����ж����IC1�������ڵ�  ���������IC2���ù�**/
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
	
//���������ߣ�ʹ�ܹ���
    TIM_Cmd(TIM1, ENABLE);

}




/**
����TIM1 �ж� NVIC
**/
static void NVIC_Advance_CapturePWM_Config()
{
   NVIC_InitTypeDef NVIC_InitStruct;
   NVIC_InitStruct.NVIC_IRQChannel = TIM1_CC_IRQn;  //�����ж�
   NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
   NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
   NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
   NVIC_Init(&NVIC_InitStruct);
}	




/***
��ʼ��PWM���벶��
***/
void TIM_Advance_CapturePWM_Init() 
{
	TIM_Advance_GPIO_Config(); 
	TIM_Advance_Base_Config(999, 71);  //�������벶��PWMʱ�� 1kHz
	TIM_Advance_CapturePWM_Config();
	NVIC_Advance_CapturePWM_Config();
	
}



/**
�жϷ����� 
������IC1�����ж϶�CCR1 ֵ��value1(����)
������IC2�����ж϶�CCR2 ֵ��value2(����)      
        value2/value1=ռ�ձ�

������һ�������� ��λ������   �����ڶ��������ز���TIM1_CC_IRQn�жϽ��뺯��
(�꿴��ͼ)
**/
u16 Value1_CCR1=0; 
u16 Value2_CCR2=0;
__O float DutyCycle=0; 
__O float Frequency=0;
void TIM1_CC_IRQHandler(void) //�����ڶ���
{
//�ж� if�ж�+��ʲô+����ж�
   TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
	
   Value1_CCR1 = TIM_GetCapture1(TIM1);  //����
   Value2_CCR2 = TIM_GetCapture2(TIM1);  //����
	
   if( Value1_CCR1!=0 ) //һ��PWM�������
   {
	  DutyCycle = (float) ( ( (Value2_CCR2+1)*100 )/(Value1_CCR1+1) ); //�Ĵ�����0��ʼ����Ҫ+1
	                                                                   //ռ�ձ��ǰٷֱ� ����*100
	  Frequency = 72000000/72/(float)(Value1_CCR1+1); //u8ǿ��ת�������Ͳ�������
	                    //Fre = 72M(RCC*PSC)
	   
	   //printf("The value of DutyCycle is %f\r\n The value of Frequency is %f\r\n", DutyCycle, Frequency);
   }
   else //δ������� ռ�ձȺ�Ƶ��ֵ��0(��Ҫ��Value1_CCR1��Value2_CCR2)
   {
	   DutyCycle=0; 
       Frequency=0;
   }


	
}






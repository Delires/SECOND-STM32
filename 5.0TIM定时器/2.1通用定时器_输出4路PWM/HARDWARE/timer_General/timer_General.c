#include "timer_General.h"
/******************************************************************/
/**********������ʱ��TIM2 ���4·PWM**************/
/**********���˼·:����4·GPIO,����ʱ�������ö�ʱ�����PWM**************************/
/******************************************************************/


/**
����GPIO���  PA0---CH1     General_Time û����ȥ��ɲ������
              PA1---CH2
			  PA2---CH3
			  PA3---CH4
   ����������ô�����أ�����
��Ϊǣ����GPIO��������������Ҫʹ�ø��ù���
**/
static void TIM_General_GPIO_Config(void)   //���������static��ֻ������������ܵ���
{
	GPIO_InitTypeDef GPIO_InitStruct; //����һ���ṹ��
//ʹ��ʱ��+����+��������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ������ 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //����TIM2 
//PA0---CH1    
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  //�ֲ�8.1.11��֪
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init  (GPIOA, &GPIO_InitStruct); 
//PA1---CH2    
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  //�ֲ�8.1.11��֪
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init  (GPIOA, &GPIO_InitStruct); 
//PA2---CH3   
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  //�ֲ�8.1.11��֪
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init  (GPIOA, &GPIO_InitStruct); 
//PA3---CH4    
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  //�ֲ�8.1.11��֪
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init  (GPIOA, &GPIO_InitStruct); 
}


/**
����TIM2 ʱ��
RCC����װ��ֵ   PSCԤ����ϵ��
**/
static void TIM_General_Base_Config(u16 arr, u16 psc)   //���������static��ֻ������������ܵ���
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //��ʹ��һ��TIM2  
//����TIM2ʱ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //�����ú�Tdts�й� CR1 CKDλ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);  
//ʹ�ܹ���
	TIM_Cmd(TIM2, ENABLE); 
}


/**
������·PWM ��ͬ��·ռ�ձȲ�ͬ
**/
static void TIM_General_PWM_Config(u16 ccr1, u16 ccr2, u16 ccr3, u16 ccr4)   
{
   TIM_OCInitTypeDef TIM_OCInitStruct;
//����CH1��PWM  Genaeral_Timû�л��������������
	//TIM_OCInitStruct.TIM_OCIdleState =   //��ȥ�������ƽ ������
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //����ߵ�ƽ��Ч
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = ccr1;
    TIM_OC1Init(TIM2, &TIM_OCInitStruct); 
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);//Ԥ���أ������¼���������װռ�ձ�ֵ 

//����CH2��PWM  Genaeral_Timû�л��������������
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //����ߵ�ƽ��Ч
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = ccr2;
    TIM_OC2Init(TIM2, &TIM_OCInitStruct); 
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//Ԥ���أ������¼���������װռ�ձ�ֵ 
	
//����CH3��PWM  Genaeral_Timû�л��������������
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //����ߵ�ƽ��Ч
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//ʹ��
	TIM_OCInitStruct.TIM_Pulse = ccr3;
    TIM_OC3Init(TIM2, &TIM_OCInitStruct); 
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);//Ԥ���أ������¼���������װռ�ձ�ֵ 

//����CH4��PWM  Genaeral_Timû�л��������������
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //����ߵ�ƽ��Ч
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = ccr4;
    TIM_OC4Init(TIM2, &TIM_OCInitStruct); 
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);//Ԥ���أ������¼���������װռ�ձ�ֵ 

//ֻ�и߼���ʱ����Ҫʹ�� TIM_CtrlPWMOutputs(TIM1, ENABLE);  
}


void TIM_General_PWM_Init(void)
{
	TIM_General_GPIO_Config();
	TIM_General_Base_Config(300, 3000);
	TIM_General_PWM_Config(50, 120, 190, 260);   //����CH1/2/3/4��ռ�ձ�rcc
}
//       190 34.4*(190/301)=34.4*0.8637=21.7  ��21.1(Ӧ����Ϊʾ�������ڲ�ͬ�����Ĳ�һ��) 
//10ms   260 34.4*(260/301)=34.4*0.8637=29.71  (���1V)

#include "timer_General.h"
/******************************************************************/
/**********������ʱ��TIM2 ���4·PWM**************/
/**********���˼·:����4·GPIO,����ʱ�������ö�ʱ�����PWM**************************/
/******************************************************************/


/**
����GPIO���  PA0---CH1     General_Time û����ȥ��ɲ������

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
static void TIM_General_PWM_Config(u16 ccr1)   
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

}


void TIM_General_PWM_Init(void)
{
	TIM_General_GPIO_Config();
	TIM_General_Base_Config(9, 71);  //PWMƵ��Ϊ 72M / ( (9+1)*(71+1) )=100kHZ
	                                   //ע��PWM��Ƶ��Ҫ>1Hz��������
	//TIM_General_Base_Config(100, 300);
	TIM_General_PWM_Config(1);   //TIM_General_PWM_Config(301);
}


//        ռ�ձ�    ƽ��ֵ   ��ֵ    Ƶ��   CH1
//         0%       885mv    72.8    100K   20V
//        10%       4.26v    71.2    100K   20V
//        20%       7.57v    72.8    100K   20V
//        30%       10.9v    72.8    100K   20V
//        40%       14.3v    72.8    100K   20V
//        50%       17.5v    72.8    100K   20V
//        60%       20.9v    71.2    100K   20V
//        90%       30.9V    71.2    100K   20V
//        100%      34V      4.0        ��     20V/
//PWM��ѹƽ��ֵ=��ֵ*ռ�ձ�
//ƽ��ֵ�ǣ���ѹ��һ������T�ڻ���֮���ٳ���T��
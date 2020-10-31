#include "exit.h"
#include "delay.h"
#include "stm32f10x.h"
#include "led.h"
#include "key.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"

void exit_config(void)
{
	
	EXTI_InitTypeDef EXTI_InitStruct;    //�ṹ�嶨����ʵ����дһ�� ��Ϊÿ�θ�ֵ�󶼳����µ���
	EXTI_InitTypeDef EXTI_InitStruct_WK_UP;
	NVIC_InitTypeDef NVIC_InitStruct;

	
//ʹ��AFIO	
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE); //����һ��������� �����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
	KEY_Init();
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);  //key1�ж���ӳ��  �½���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);   //key0�ж���ӳ��   �½���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);   //wk_up�ж���ӳ��  ��������

//����key1 key0�ж� �½��ش���	
	EXTI_InitStruct.EXTI_Line= EXTI_Line3;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line= EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	
//����wk_up�ж� �����ش���		
	EXTI_InitStruct_WK_UP.EXTI_Line= EXTI_Line0;
	EXTI_InitStruct_WK_UP.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct_WK_UP.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct_WK_UP.EXTI_Trigger = EXTI_Trigger_Rising;   //�����ش���
	EXTI_Init(&EXTI_InitStruct_WK_UP);
	 
//�����ж����ȼ�  wk_up���ȼ���� 
	NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn; 
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00; //��Ϊ���õ��жϷ��鵼��
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x03;    //ԽС���ȼ�Խ��
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn; 
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00; //��Ϊ���õ��жϷ��鵼��
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x02;    //ԽС���ȼ�Խ��
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI3_IRQn; 
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00; 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x01;   
	NVIC_Init(&NVIC_InitStruct);	
}

//WK_UP�жϺ��� ����
void EXTI0_IRQHandler(void)  
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		
		
		TriangleVol();
		

		EXTI_ClearITPendingBit(EXTI_Line0); //
	}
}

//�ǵ�Ҫ����ж�
//KEY0�жϺ���   ���
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		SawVol();        //��ݲ�

		EXTI_ClearITPendingBit(EXTI_Line4); //
	}
	
}

//KEY1�жϺ��� ����
void EXTI3_IRQHandler(void)
{	
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	{
	   SquareVol(); 
		
  		EXTI_ClearITPendingBit(EXTI_Line3); //
	}
		     
}



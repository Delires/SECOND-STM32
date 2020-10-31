/**
д����4������  ��RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)����
                ��A~F��ʹ�ܶ�(���ĸ��˿ڴ��ĸ��˿ڵ�ʹ�ܶ�)
							 �ڳ�ʼ��IO��ģʽ  GPIO_Init()������һ��ָ�����GPIO_InitStructure���������Pin��Mode��Speed�Ķ���
							 �۲���IO������ߵ͵�ƽ  GPIO_setBits()  GPIO_ResetBits()
**/

#include "stm32f10x.h"
#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����һ��ָ����������ָ�����Pin��Mode��Speed
	
//�ٴ� GPIOB GPIOE ʹ�ܶ� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //��GPIOBʹ�ܶ� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //��GPIOEʹ�ܶ� 
	//����ʹ�û����㽫����дһ�� RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //��GPIO_InitStructure�����ַ�︳ֵPin��Mode��Speed 
	//������� 5���� �ٶ�50MHZ
//�ڳ�ʼ��B��IO��ģʽ
  GPIO_Init(GPIOB, &GPIO_InitStructure);

//�۲���IO������ߵ͵�ƽ  GPIO_setBits()
  GPIO_SetBits(GPIOB, GPIO_Pin_5);	

	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //��GPIO_InitStructure�����ַ�︳ֵPin��Mode��Speed 
	//������� 5���� �ٶ�50MHZ
//�ڳ�ʼ��E��IO��ģʽ
  GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
//�۲���IO������ߵ͵�ƽ  GPIO_setBits()
  GPIO_SetBits(GPIOE, GPIO_Pin_5);

}

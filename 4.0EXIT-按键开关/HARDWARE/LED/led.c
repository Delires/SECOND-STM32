#include "led.h"

void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	
 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��ʼ��PB5
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_SetBits(GPIOB, GPIO_Pin_5); //PB5���1
	
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��ʼ��PE5
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOC, &GPIO_InitStructure);
 GPIO_SetBits(GPIOC, GPIO_Pin_5); //PE5���1
	
}

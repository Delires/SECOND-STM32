#include "stm32f10x.h"
#include "key.h"
#include "delay.h"

void KEY_Init(void)//��ʼ��PB12 13 14 15
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //��PCʹ�ܶˣ�����8 9 10 11
	
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU; //����Ϊ��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;

	GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB8 PB9 PB10 PB11
		
}






int KEY_Scan(void) //��ʼ����������
{
	static int Key_up = 1;   //���徲̬����Key_up=1 ������������ֵ

	if(Key_up && (KEY1==0||KEY2==0||KEY3==0||KEY4==0))//����������ֵ �� key1~4�м�����
	{
		delay_us(20); //ȥ����
		Key_up=0;  //Key_up��0����������������ʱ��Ϊ1�������Ͳ���������ֵ
		
		if(KEY1==0)
		{
			return 1;
		}
		else if(KEY2==0)
		{
			return 2;
		}
		else if(KEY3==0)
		{
			return 3;
		}
		else if(KEY4==0)
		{
			return 4;
		}
  }
	
	else if(KEY1==1 && KEY2==1 && KEY3==1 && KEY4==1) //ͬʱ������ �͸�Key_up���¸�ֵ1�������´ξ��ܼ�����һ��ѭ������ֵ
	  	Key_up=1;
	return 0;
}

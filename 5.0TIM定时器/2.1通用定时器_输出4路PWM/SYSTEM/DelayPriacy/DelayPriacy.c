#include "DelayPriacy.h"
#include "sys.h"

uint16_t i;        



static u8 fac_us=0;    //��ʱ΢���Ƶ�ʣ����΢��������ʱ����
static u16 fac_ms=0;   //��ʱ�����Ƶ��

void delay_init(void)//��ʼ����������
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //����SysTickʱ��Ϊ �ⲿʱ��72MHZ
	fac_us = SystemCoreClock/8000000;  //SystemCoreClock�������úõ�ʱ��72MHZ
	                                  //72000000/1000000/8 ����1΢�룬���ڰ˷�Ƶ�����ⲿ��72��Ϊ1s �ڲ�9��Ϊ1s�����Ի�Ҫ��8
  fac_ms = (u16)fac_us*1000; //72000000/1000/8�Ǻ���
}


void delay_us(u32 nus)  //us΢����ʱ����
{
	u32 temp;
	SysTick->LOAD = nus*fac_us; //װ�ص�ֵΪ  ��ֵ��΢��*����1us�Ĵ���
	SysTick->VAL = 0X00;        //д�������ó�ʼֵ����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //ʹ��SysTick��ʱ��
	do
	{
		temp = SysTick->CTRL; //һֱ�������Ѽ�����ֵ����temp
	}while( (temp&0x01) && !( temp&(1<<16) ) );
	     //��temp��0λΪ1(��ʱ������) �����λ����1ʱ(����ûֹͣʱ)
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //�ر�ʹ��
	SysTick->VAL = 0X00; //���ü�ʱ��	
}


void delay_ms(u16 nms) //ms�����ʱ
{
	u32 temp;
	SysTick->LOAD = (u32)nms*fac_ms;   //��װֵ
	SysTick->VAL = 0x00;               //�����ʱ��
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //��ʹ�ܶ�
	do
	{
		 temp = SysTick->CTRL; //CTRL�Ĵ���������ֵ����temp
	}while( (temp&0x01) && !( temp&(1<<16) ) );
	    //��CTRL�Ĵ���0λΪ1(�������) ��16λΪ1(�������ֹͣ��)
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //�ر�ʹ��
	SysTick->VAL = 0X00; //���ü�ʱ��ֵ
}


void Delay(uint32_t Count)  //������ʱ�������������ʱ
{
	for(; Count!=0; Count--)
	{
	 i = 5000;
	 while(i--);
	}	
}

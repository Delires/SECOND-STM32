#include "stm32f10x.h"
#include "key.h"   //��key.h����
#include "DelayPriacy.h"  
#include "delay.h"





//���尴����ʼ������
void KEY_Init(void)  
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ָ��,���������ǰͷ
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE, ENABLE); //��GPIOA GPIOEʹ�ܶ�

//����GPIOBΪ�������룬234��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //�����ӵأ���IO���Ǹߵ�ƽ������Ϊ��������,����������1 ���¶�����0
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	//�������� û������ٶ�
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

//����GPIOAΪ�������룬0��  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //�����Ӹߵ�ƽ������IO�ڴ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}




//���尴������ѡ�񣬺ͷ��ذ���ֵ����
u8 KEY_Scan(u8 mode)    //��Ϊ�з���ֵ��������u8 xxx ������void xxx
{
	static u8 key_up = 1; //���þ�̬�����������´ε��õĻ���static�ͱ��0��
	if(mode)
	{             //mode=1 �����Ͱ���  key_up=1����Զ�ܽ���if()���
		key_up = 1; //mode=0�������Ͱ��� �����ȫ������̧�����Ż�	key_up = 1 ���ܽ���if()
	}
	if( key_up && (KEY0==0||KEY1==0||KEY2==0||WK_UP==1) ) 
	{      //KEY1 KEY2 KEY3 WK_UP�ֱ��Ǻ궨�����ֵ
		Delay(10);
		key_up = 0; //��Ϊ����

  //����ֵȷ�� Switch��䵽���ĸ�������		
		if(KEY0==0) return KEY0_PRES;   
		else if(KEY1==0) return KEY1_PRES;
		else if(KEY2==0) return KEY2_PRES;
		else if(WK_UP==1) return WKUP_PRES;
	}
	else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0) //��û�м�����
	{
		key_up = 1; //û�м����£����¸�ֵ key_up=1���ȴ��´ΰ��¼���ִ��
		      
	}
	return 0;
}



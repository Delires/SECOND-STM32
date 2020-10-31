#ifndef __KEY_H__
#define __KEY_H__

#include "sys.h"


#define KEY0 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)//�궨��KEY0Ϊ ������PE4��ֵ
#define KEY1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)//KEY1  PE3
#define KEY2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)//KEY2  PE2
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//WAKUP PA0
              //�⺯������0/1


#define KEY0_PRES 1  //Switch()������Ӧ���
#define KEY1_PRES 2
#define KEY2_PRES 3
#define WKUP_PRES 4


void KEY_Init(void); //����   ��ʼ����������

u8 KEY_Scan(u8 mode); //����  ѡ���������»���������º���

#endif

#ifndef __KEY_H__
#define __KEY_H__

#include "sys.h"


#define KEY0 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)//宏定义KEY0为 函数读PE4的值
#define KEY1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)//KEY1  PE3
#define KEY2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)//KEY2  PE2
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//WAKUP PA0
              //这函数返回0/1


#define KEY0_PRES 1  //Switch()函数对应序号
#define KEY1_PRES 2
#define KEY2_PRES 3
#define WKUP_PRES 4


void KEY_Init(void); //声明   初始化按键函数

u8 KEY_Scan(u8 mode); //声明  选择连续按下或非连续按下函数

#endif

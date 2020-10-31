#ifndef __key_h__
#define __key_h__

//宏定义 定义KEY为读取端口值
#define KEY1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) //GPIO_ReadInputDataBit读取输入值函数
#define KEY2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)
#define KEY3 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define KEY4 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)

void KEY_Init(void);  //按键初始化函数

int KEY_Scan(void); //定义读值函数  u8是因为此函数有返回值
                   //              void表示次函数使用时不用赋值


#endif

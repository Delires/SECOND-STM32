/**
写函数4个步骤  ①RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)函数
                打开A~F的使能端(用哪个端口打开哪个端口的使能端)
							 ②初始化IO口模式  GPIO_Init()，定义一个指针变量GPIO_InitStructure，给里面放Pin，Mode，Speed的定义
							 ③操作IO口输出高低电平  GPIO_setBits()  GPIO_ResetBits()
**/

#include "stm32f10x.h"
#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义一个指针变脸，这个指针里放Pin，Mode，Speed
	
//①打开 GPIOB GPIOE 使能端 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //打开GPIOB使能端 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //打开GPIOE使能端 
	//或者使用或运算将两个写一块 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //给GPIO_InitStructure这个地址里赋值Pin，Mode，Speed 
	//推挽输出 5引脚 速度50MHZ
//②初始化B的IO口模式
  GPIO_Init(GPIOB, &GPIO_InitStructure);

//③操作IO口输出高低电平  GPIO_setBits()
  GPIO_SetBits(GPIOB, GPIO_Pin_5);	

	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //给GPIO_InitStructure这个地址里赋值Pin，Mode，Speed 
	//推挽输出 5引脚 速度50MHZ
//②初始化E的IO口模式
  GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
//③操作IO口输出高低电平  GPIO_setBits()
  GPIO_SetBits(GPIOE, GPIO_Pin_5);

}

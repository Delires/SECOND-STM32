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
	
	EXTI_InitTypeDef EXTI_InitStruct;    //结构体定义其实可以写一个 因为每次赋值后都成了新的了
	EXTI_InitTypeDef EXTI_InitStruct_WK_UP;
	NVIC_InitTypeDef NVIC_InitStruct;

	
//使能AFIO	
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE); //找了一天错误在这 他妈的
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟
	KEY_Init();
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);  //key1中断线映射  下降沿
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);   //key0中断线映射   下降沿
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);   //wk_up中断线映射  读上升沿

//配置key1 key0中断 下降沿触发	
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
	
	
//配置wk_up中断 上升沿触发		
	EXTI_InitStruct_WK_UP.EXTI_Line= EXTI_Line0;
	EXTI_InitStruct_WK_UP.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct_WK_UP.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct_WK_UP.EXTI_Trigger = EXTI_Trigger_Rising;   //上升沿触发
	EXTI_Init(&EXTI_InitStruct_WK_UP);
	 
//设置中断优先级  wk_up优先级最低 
	NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn; 
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00; //因为设置的中断分组导致
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x03;    //越小优先级越高
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn; 
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00; //因为设置的中断分组导致
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x02;    //越小优先级越高
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI3_IRQn; 
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00; 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x01;   
	NVIC_Init(&NVIC_InitStruct);	
}

//WK_UP中断函数 三角
void EXTI0_IRQHandler(void)  
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		
		
		TriangleVol();
		

		EXTI_ClearITPendingBit(EXTI_Line0); //
	}
}

//记得要清楚中断
//KEY0中断函数   锯齿
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		SawVol();        //锯齿波

		EXTI_ClearITPendingBit(EXTI_Line4); //
	}
	
}

//KEY1中断函数 方波
void EXTI3_IRQHandler(void)
{	
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	{
	   SquareVol(); 
		
  		EXTI_ClearITPendingBit(EXTI_Line3); //
	}
		     
}



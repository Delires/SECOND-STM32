#include "tim.h"

//设定数组中一个成员的触发时间
//DMA把数据往DA寄存器搬，然后每TIM定时一次DA就触发一次，触发一次就输出一个数组的时间。然后一直触发，这样出现的就是一个频率的波形
//运行过程如下，tim触发一次，da就输出一个数据，dma在搬进来一个
             //所以此方法可行
void Tim2_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//54931*65535/(72000000/4)=200us=200s 是错的 亲 看手册不仔细
//2000*7200/72000000=0.2s  这个才对 除72000000算出来的是s
	TIM_TimeBaseInitStruct.TIM_Period = 2000/f;   //通过数组输出的每个部分来改变波形，不是通过一个周期来改变               
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;        
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_Update);  //定时器更新触发输出，用作触发另一个定时器，DA触发输出
	TIM_Cmd(TIM2, ENABLE); //使能
}



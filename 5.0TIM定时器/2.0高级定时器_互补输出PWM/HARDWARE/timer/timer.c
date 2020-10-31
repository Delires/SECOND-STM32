/**
初始化基本定时器TIM6 /TIM7
延时500ms
**/
#include "timer.h"

u16 time_save=0;

/************************************** 
配置TIM6寄存器 三件套 RCC使能 配置 功能开启
输入参数 arr自动重装载值   psc预分频系数
计数一次时间=72M/(psc+1)  计数周期=arr*计数一次时间
 //psc=7199  计数一次1000hz=1ms
**************************************************/
void Config_Base_TIM6(u16 arr, u16 psc) 
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct; 
//三件套--RCC使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

//三件套--配置	
	//基本定时器不存在时钟分频
	//TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	//基本定时器不存在计数模式   从库函数可以看出
	//TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; //基本定时器只能向上计数
	TIM_TimeBaseInitStruct.TIM_Period= arr;                          //加载到ARR寄存器的值
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;          //预分频
	//基本定时器不存在重复计数功能
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter =  
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);  
	
//三件套--功能开启
	TIM_Cmd(TIM6,  ENABLE); 


//开启中断
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);   //先清除更新中断标志位

	TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE); //开启更新中断

}


/*******
配置中断NVIC 配置中断分组  NVIC_Init() 和  NVIC_PriorityGroupConfig()

配置了中断就要配置中断函数
********/

void NVIC_Base_TIM6(void)
{
	NVIC_InitTypeDef NVIC_InitStruct; 
  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //优先级分组(优先级应该只能分一个组，大家都按它的等级分)
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;  //配置中断源
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =0x00;
	NVIC_Init(&NVIC_InitStruct);
}	





/********
TIM6的中断服务函数
函数名启动文件找
老步骤：判断中断标志  做出判断  清楚中断标志
*********/
void TIM6_IRQHandler()
{
	if(TIM_GetFlagStatus( TIM6, TIM_FLAG_Update) != RESET)   //if( TIM_GetFlagStatus(TIM6,TIM_FLAG_Update)==1 )
	{
          time_save++;
		  TIM_ClearFlag(TIM6,TIM_FLAG_Update);   //先清除更新中断标志位
	}			
}

//TIM_GetFlagStatus 是读标志位
//TIM_GetITStatus  是读事件        本质上这二者是一样的 只不过形参不同    



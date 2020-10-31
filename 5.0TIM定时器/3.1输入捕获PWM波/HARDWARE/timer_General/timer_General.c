#include "timer_General.h"
/******************************************************************/
/**********基本定时器TIM2 输出4路PWM**************/
/**********编程思路:配置4路GPIO,配置时基，配置定时器输出PWM**************************/
/******************************************************************/


/**
配置GPIO输出  PA0---CH1     General_Time 没有死去和刹车功能

因为牵扯到GPIO的其他输出，因此要使用复用功能
**/
static void TIM_General_GPIO_Config(void)   //函数定义成static则只有这个函数里能调用
{
	GPIO_InitTypeDef GPIO_InitStruct; //定义一个结构体
//使能时钟+配置+开启功能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能引脚 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //复用TIM2 
//PA0---CH1    
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  //手册8.1.11得知
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init  (GPIOA, &GPIO_InitStruct); 

}


/**
配置TIM2 时基
RCC重新装载值   PSC预分配系数
**/
static void TIM_General_Base_Config(u16 arr, u16 psc)   //函数定义成static则只有这个函数里能调用
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //再使能一遍TIM2  
//配置TIM2时基
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //该配置和Tdts有关 CR1 CKD位
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);  
//使能功能
	TIM_Cmd(TIM2, ENABLE); 
}


/**
配置四路PWM 不同四路占空比不同
**/
static void TIM_General_PWM_Config(u16 ccr1)   
{
   TIM_OCInitTypeDef TIM_OCInitStruct;
//配置CH1的PWM  Genaeral_Tim没有互补输出不用配置
	//TIM_OCInitStruct.TIM_OCIdleState =   //死去后输出电平 不配置
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //输出高电平有效
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = ccr1;
    TIM_OC1Init(TIM2, &TIM_OCInitStruct); 
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);//预加载，更新事件产生再重装占空比值 

}


void TIM_General_PWM_Init(void)
{
	TIM_General_GPIO_Config();
	TIM_General_Base_Config(9, 71);  //PWM频率为 72M / ( (9+1)*(71+1) )=100kHZ
	                                   //注意PWM的频率要>1Hz否则会出错
	//TIM_General_Base_Config(100, 300);
	TIM_General_PWM_Config(1);   //TIM_General_PWM_Config(301);
}


//        占空比    平均值   峰值    频率   CH1
//         0%       885mv    72.8    100K   20V
//        10%       4.26v    71.2    100K   20V
//        20%       7.57v    72.8    100K   20V
//        30%       10.9v    72.8    100K   20V
//        40%       14.3v    72.8    100K   20V
//        50%       17.5v    72.8    100K   20V
//        60%       20.9v    71.2    100K   20V
//        90%       30.9V    71.2    100K   20V
//        100%      34V      4.0        无     20V/
//PWM电压平均值=峰值*占空比
//平均值是：电压在一个周期T内积分之后再除以T；
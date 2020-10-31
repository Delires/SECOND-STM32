#include "timer_advanced.h"
/******************************************************************/
/**********此函数调用TIM61/8高级定时器来互补输出PWM波**************/
/**********带死区时间  带刹车控制的**************/
/******************************************************************/
/**********此函数调用TIM61/8高级定时器来互补输出PWM波**************/
/**
查表得 TIN1_CH1--PA8  TIN1_CH1N--PB13互补输出PWM   刹车BKIN---PB12  
                       由于PB13 PB12连接了其他的芯片所以PWM输出有毛刺(芯片启动产生的)
原理:CNT计数，CRR控制高电平时间(对应CRRx寄存器)
              ARR决定PWM周期(对应ARR寄存器)
			  时基决定计时一次时间(PSC寄存器)

编程路子:     使能(这就不用管 时钟使能＋功能使能)
              配置GPIO(因为GPIO输出PWM)   GPIO_Init()
              初始化定时器配置时基        TIM_TimeBaseInit() 
			  初始化输入/输出捕获         TIM_OC1Init()
                             			使能RCC  TIM_OC1PreloadConfig() 

			  初始化刹车功能              TIM_BDTRConfig()     Bakeup DeadTime
			  中断优先级配置              NVIC_Init()
**/

/**
配置GPIO输出  TIN1_CH1--PA8  1
              TIN1_CH1N--PB13互补输出PWM   
			  刹车BKIN---PB12 
因为牵扯到GPIO的其他输出，因此要使用复用功能
**/
static void TIM_Advanced_GPIO_Config(void)   //函数定义成static则只有这个函数里能调用
{
	GPIO_InitTypeDef GPIO_InitStruct; //定义一个结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   //复用GPIOA的话 直接使能相应功能即可	

//配置TIN1_CH1--PA8 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  //复用引脚 推挽劲大
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);  //取结构体地址 

//TIN1_CH1N--PB13 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  //复用引脚 推挽劲大
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);  //取结构体地址 
	
//刹车BKIN---PB12   刹车为毛也配置成输出呢(刹车也要配置成输出)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  //复用引脚 推挽劲大
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);  //取结构体地址 

//刹车先设置为低电平 到底什么电平是刹车在mode里具体配置 我们配置的是高电平刹车
	GPIO_ResetBits(GPIOB,GPIO_Pin_12); 

//	
}







/**
配置高级定时器的 模式和 刹车功能
还有时基
arr重装载值, psc预分配, high高电平周期(高电平的装载值)
**/
static void TIM_Advanced_Mode_Config(u16 arr, u16 psc, u16 high)
{
	 TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;   //时基配置结构体
     TIM_OCInitTypeDef TIM_OCInitStruct;               //PWM配置结构体
     TIM_BDTRInitTypeDef TIM_BDTRInitStruct;	       //刹车+死区时间配置结构体

/*********************配置PWM频率(TIM时基)*************************/
//配置时基  时基决定PWM频率   
//psc=8   rcc=7  PWM周期=(1/(72/psc))*arr=(psc*arr)/72M    所以PWM频率=72M/(psc*arr)
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//配置死区时间要用到Tds=CKD*Tck_int
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; //不使用重复寄存器
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct); 

/*********************配置TIM1的OC1的两路输出PWM1  PWM占空比*************************/	
// 配置为PWM模式1
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
// 输出通道 空闲电平极性配置 和 有效电平电平极性配置
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set; //PWM有刹车功能 当使能刹车PWM停止输出，停止输出时CHx空闲时刻输出高电平
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;   //CHx高电平有效 即CNT<RCC输出高电平
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//输出通道使能
	
// 互补输出通道 空闲电平极性配置 和 输出有效电平的极性配置
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //使能刹车PWM停止输出，停止输出时CHxN空闲时刻输出高电平
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;  //CHxN高电平有效 即CNT<RCC输出高电平
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;//互补输出使能
	
	TIM_OCInitStruct.TIM_Pulse = high;// (psc/high);       //这个值/arr就是占空比 因为用的是同一时钟 arr是总的计数周期
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);

//预加载是否使能,即写入ARR是否立即生效
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
/************************ 初始化刹车功能 死区时间**************************/	
    TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_1;                 //即不能该表刹车的配置   和 刹车后电平输出状态 
    TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Enable;  //空闲模式下 定时器不工作时 输出空闲电平 
	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Enable;  //输出模式下 定时器不工作时 输出无效电平
//死区时间 
	TIM_BDTRInitStruct.TIM_DeadTime = 11;  //配置死区时间 11d=0000 1011b 查手册知DTG[7:5]=0xx => DT=DTG[7:0]×Tdtg，Tdtg = TDTS；   TDTS=CKD*TCK_INT(看CR1寄存器) 
	                                        //11*Tdtg=11/72M=0.152ns   具体时间根据半桥/全桥MOS管的特性去确定这个时间
//刹车
    TIM_BDTRInitStruct.TIM_Break = TIM_Break_Enable; //刹车使能
	TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_High;  //刹车极性高电平 即PB12为高时PWM停止输出
	 TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable; //自动输出使能  如果刹车无效下一个更新事件自动输出
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStruct);

/************** 三步走 使能时钟+配置+使能功能 ****************************/
//使能定时器
   TIM_Cmd(TIM1, ENABLE);
//高级定时器独有的 开启OCx 和OCxN主输出 否则不输出     通用定时器则不许要这个使能 MOE位
   TIM_CtrlPWMOutputs(TIM1, ENABLE); 

}


//218





/**
定时器初始化Init，调用前两个函数
注意xxx_Config和xxx_Init不一样  前者是最基本的配置   后者是配置完后再调用前者去初始化Init
**/
void TIM_Advanced_Mode_Init(void)
{
	TIM_Advanced_GPIO_Config(); 
	TIM_Advanced_Mode_Config(8, 7, 4);
	                      //输入参数u16 arr, u16 psc, u16 high
	                      //arr重装载值  high高电平时间   (arr+1)/high=PWM占空比
	                      //psc预分配系数   PWM频率=72M/((arr+1)*(psc+1))
	//TIM_Advanced_Mode_Config(500, 600, 150);  //蜂鸣器听PWM的频率
}
//89 79 PWM 0.1ms 10khz    死区时间0.152us  必然看不到        
//8 7 pwm一个周期1us  所以能能看到死区时间
//如果想插到蜂鸣器听声音 插到P20左  注意PWM频率太大太小人耳听不到
//TIM_Advanced_Mode_Config(8, 7, 4); 配置函数调用两次，后面的会覆盖前面的 程序是线性的

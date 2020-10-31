#include "timer_General.h"
/******************************************************************************************************/
/**********基本定时器TIM2 捕获脉冲宽度*****************************************************************/
/**********编程思路:TIM2的CH1连接PA0,PA0连接WK_UP按键刚好能输出一个脉冲高电平**************************/
/**********我们配合WK_UP按键 配置TIM2的CH1为输入捕获***************************************************/
/******************************************************************************************************/
/******************************************************************************************************/

/**
配置GPIO  PA0---CH1 PA0输入捕获   配置成什么查询手册8.1.11
**/
static void TIM_General_GPIO_Config(void)   //函数定义成static则只有这个函数里能调用
 {
    GPIO_InitTypeDef GPIO_InitStruct;  
 
//使能时钟+配置+开启功能三件套(gpio配置不存在开启功能)
	 /**有是这的错误  **/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;  //改成下拉输入 因为要读取高电平
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	 
	GPIO_ResetBits(GPIOA, GPIO_Pin_0); //wk_up按键按下是上升沿，因此先提前下拉
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
配置TIM2的CH1为输入捕获
**/
static void TIM_General_Capture_Config()   
{
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct; 

	//配置输入捕获
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1; //TIM2的CH1
	TIM_ICInitStruct.TIM_ICFilter = 0x00;       //输入捕获过滤 
	              //比如输入的信号为1M，又存在高频的信号干扰，那么此时就很有必要进行滤波，我们可以设置采样频率为2M，可以在保证采样到有效信号的基础上把高于2M 的高频干扰信号过滤掉。
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; //捕获上升沿，中断里要改
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
                        	//设置每多少次触发事件才算捕获一次
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;//输入捕获通道ICx与输入引脚TIx的映射关系 直连 PWM要非直连
    TIM_ICInit(TIM2, &TIM_ICInitStruct);  
	
                      
//配置中断优先级
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&NVIC_InitStruct);  

    TIM_ClearFlag(TIM2, TIM_FLAG_CC1|TIM_FLAG_Update); //先清除一波中断标志  TIM_ClearITPendingBit()这个函数也可以
                                                  //清除中断防止误差
/**本人程序缺这部分**/
    TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_CC1, ENABLE);  //使能捕获和更新中断 

    TIM_Cmd(TIM2, ENABLE); 
}



/**
中断服务函数 
读取脉冲时间  注意:因为只有定时器中断，而不是上升沿下降沿中断
                   因此要用几个标志位来判断上升/下降沿中断  判断捕获结束
 debug用 printf("2"); 
**/
/***************************本人思路***********************************/
#if( Myslef_Open )
//定义一个结构体  此处是分配内存的 存在ROM的静态区
GPIO_Genral_Capture Capture_value={0, 0, 0, 0};
/**中断都进不去为什么呢？ RCC TIM2开启不正常，重新使能就好了**/

void TIM2_IRQHandler(void)
{
//中断 if判断+做什么+清楚中断
	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update)==1) //捕获高电平产生中断
	{
		//printf("1");     //debug用
		
        Capture_value.Period_Value ++;  
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	
	}
	
	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_CC1)==1) //捕获高/低电平产生中断
	{
		//printf("2");     //debug用

	   if(Capture_value.CaptureStart_Bit == 0) //没有捕获上升沿
	   {
		   Capture_value.CaptureStart_Bit = 1;  //上升沿标志位设置为1
	       Capture_value.CaptureStop_Bit = 0;  //结束标志位为0
		   
		   TIM_SetCounter(TIM2, 0);  //设置计时器为0
		   Capture_value.Period_Value = 0;
		   Capture_value.Time_Value = 0;
		   
		   TIM_OC1PolarityConfig(TIM2, TIM_OCPolarity_Low); //配置为低电平捕获	   
	   }
	   else //if判断时已把CaptureStart_Bit变为1了
	   {
		   //Capture_value.Time_Value =TIM_GetCounter(TIM2);  //与下面那个一样的
		   Capture_value.Time_Value=TIM_GetCapture1(TIM2);
		 
		   TIM_OC1PolarityConfig(TIM2, TIM_OCPolarity_High);//配置为高电平捕获
		   
		   Capture_value.CaptureStart_Bit = 0;  //上升沿变为0等待下次捕获
		   Capture_value.CaptureStop_Bit=1;//结束标志位
	   }
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);	
	}
	
}
#endif




/***************************正点原子 没人比他的更秀！！！！*************************************/
/**
    正点秀的有如下几个地方：①程序先执行else 后执行if
                            ②用位的值来赋值，怎么说呢。定义一个数 用他的第7位来作为开始的判断位
                                                                   用它的第6位来作为脉冲结束的判断位
                                                                   用它的5~0位来作为他的周期计数次数位
                            不知道正点是故意秀还是真正的大神就要把代码写得看不太懂
**/
#if( ZhengDian_Open )
u8 TIM2CH1_CAPTURE_STA=0;
//TIM2CH1_CAPTURE_STA=0X04代表捕获上升沿   0x08代表捕获结束  5~0位代表周期的次数
u8 TIM2CH1_CAPTURE_VAL=0;
void TIM2_IRQHandler(void)
{
//中断 if判断+做什么+清楚中断
	if( (TIM2CH1_CAPTURE_STA&0x80)==0 ) //判断捕获是否成功 捕获未结束进入判断
	{
		if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update)==1)//更新中断
		{
			if( TIM2CH1_CAPTURE_STA&0x40 )//如果捕获到上升沿，则证明脉冲超过一个计时周期
		     {
		 	      if( (TIM2CH1_CAPTURE_STA&0x3F)==0x3F ) //如果脉冲长度超过时限 直接强制最大
				  {
					  TIM2CH1_CAPTURE_VAL|=0xFFFF;
					  TIM2CH1_CAPTURE_STA|=0x80;  //强制捕获结束
				  }
				  else
				  {
					  TIM2CH1_CAPTURE_STA++;  //一直加到0x3F结束
				  }
		     }
		}

		if( TIM_GetFlagStatus(TIM2, TIM_FLAG_CC1)==1  )//捕获中断
		{
		 if( TIM2CH1_CAPTURE_STA&0x40) //捕获到上升沿
			{
				 TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2);
				 TIM2CH1_CAPTURE_STA|=0x80; //标记为捕获完成
				
				 TIM_OC1PolarityConfig(TIM2, TIM_OCPolarity_High);//配置为高电平捕获
				
			}
			else//没有捕获到上升沿
			{
				 TIM2CH1_CAPTURE_STA=0;//第一次捕获到电平所有值清0
                 TIM2CH1_CAPTURE_VAL=0;
				
				 TIM_SetCounter(TIM2, 0);//清零计时器
				 TIM_OC1PolarityConfig(TIM2, TIM_OCPolarity_Low); //配置为低电平捕获
				 TIM2CH1_CAPTURE_STA|=0x40;//状态位标记下
			}
		}
		
	}
	
	
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //清楚中断
	
}
#endif













/********************初始化定时器 引脚 时基 输入捕获配置*********************/
void TIM_General_Capture_Init(void)
{
	TIM_General_GPIO_Config();
	TIM_General_Base_Config(65535, 71); //psc越小越精确  计时周期6553us=6.5ms   计时一次1ms
	TIM_General_Capture_Config();  
}


#include "PWM_Capture.h"
#include "usart.h"
/*****************************************************************
*************TIM1的CH1(PA8)为输入捕获，捕获PWM频率****************
原理如下：
         ①测pwm频率：CH1映射到IC1
		              第一次上升沿捕获中断 记录value1 清除捕获中断    
		              第二次再发生上升沿捕获中断再记录一次值value2 清除捕获中断  
					  (value2-value2)/1=频率值
         ①测pwm占空比：CH1在映射到IC2
		                测高电平时间PW1捕获脉宽 计算占空比   不用在中断里配置高/低捕获 因为PWM的捕获模式，IC2会自动捕获高电平
		
		 CH1配置成直连，则IC1上升沿触发捕获周期  IC2捕获高电平
		         非直连，则IC1上升沿触发捕获周期  IC2捕获高电平
		 
注意：捕获输入的 捕获周期要>PWM周期，这样才能捕获完一个PWM周期。
      反之来说，就是捕获的频率<PWM频率  
	  实际根据PWM频率来调节，实际中电机的PWM频率为10K~25K 所以我们输入捕获时基一般配置为1K
*****************************************************************/

/**
配置GPIO引脚   TIM1 CH1(PA8)为输入捕获
**/
static void TIM_Advance_GPIO_Config(void)   //函数定义成static则只有这个函数里能调用
 {
    GPIO_InitTypeDef GPIO_InitStruct;  

//使能时钟+配置+开启功能三件套(gpio配置不存在开启功能)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;  //改成下拉输入 通常都这样
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	 
	GPIO_ResetBits(GPIOA, GPIO_Pin_8); 
}
  


/**
配置TIM1 时基
RCC重新装载值   PSC预分配系数
注意：捕获输入的 捕获周期要>PWM周期，这样才能捕获完一个PWM周期。
      反之来说，就是捕获的频率<PWM频率  
配置为RCC=99 PSC=71   72m/ ( (999+1)*(71+1) ) =1Khz
**/
static void TIM_Advance_Base_Config(u16 arr, u16 psc)   //函数定义成static则只有这个函数里能调用
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); ; //再使能一遍TIM1  
//配置TIM1时基
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //该配置和Tdts有关 CR1的CKD位  Tdts是死区时间的时钟  
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00; //重复寄存器不用
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);  
//使能功能
	TIM_Cmd(TIM1, ENABLE); 
	
}




/**
配置TIM1的CH1为PWM输入捕获 (配置输入捕获CH1 和 从模式寄存器)
	   当工作做PWM输入模式时,只需要设置触发信号的那一路即可（用于测量周期）
       另外一路（用于测量占空比）会由硬件自带设置，不需要再配置

配置严格按照《官方手册13.3.7 PWM输入模式》
**/
static void TIM_Advance_CapturePWM_Config()   
{
/**配置第一路IC1测PWM 周期**/
	TIM_ICInitTypeDef TIM_ICInitStruct;
	 
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0x00;  //不滤波
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; //上升沿有效
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;  //设置每多少次触发事件才算捕获一次
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; //直连
	TIM_PWMIConfig(TIM1, &TIM_ICInitStruct); 
//TIMx_CCR1的值是周期     TIMx_CCR2的值是脉宽
	
/**配置第二路IC2测PWM 脉宽**/	
    // 当工作做PWM输入模式时,只需要设置触发信号的那一路即可（用于测量周期）
    // 另外一路（用于测量占空比）会由硬件自带设置，不需要再配置
	
/**配置从模式寄存器**/ //从模式寄存器就相当于起一个一旦收到信号立马复位计数器的作用
//选择有效的触发输入信号：置TIMx_SMCR寄存器中的TS=101(选择TI1FP1)。
	TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1); //配置输入捕获TI1FP1
	TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);// Rising edge of the selected trigger signal (TRGI) re-initializes the counter and triggers an update of the registers.
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);//使能从模式 

/**使能中断   清除中断  这个中断针对IC1捕获周期的  捕获脉宽的IC2不用管**/
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
	
//配置三步走，使能功能
    TIM_Cmd(TIM1, ENABLE);

}




/**
配置TIM1 中断 NVIC
**/
static void NVIC_Advance_CapturePWM_Config()
{
   NVIC_InitTypeDef NVIC_InitStruct;
   NVIC_InitStruct.NVIC_IRQChannel = TIM1_CC_IRQn;  //捕获中断
   NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
   NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
   NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
   NVIC_Init(&NVIC_InitStruct);
}	




/***
初始化PWM输入捕获
***/
void TIM_Advance_CapturePWM_Init() 
{
	TIM_Advance_GPIO_Config(); 
	TIM_Advance_Base_Config(999, 71);  //配置输入捕获PWM时基 1kHz
	TIM_Advance_CapturePWM_Config();
	NVIC_Advance_CapturePWM_Config();
	
}



/**
中断服务函数 
当读到IC1产生中断读CCR1 值到value1(周期)
当读到IC2产生中断读CCR2 值到value2(脉宽)      
        value2/value1=占空比

读到第一个上升沿 复位计数器   读到第二个上升沿产生TIM1_CC_IRQn中断进入函数
(详看框图)
**/
u16 Value1_CCR1=0; 
u16 Value2_CCR2=0;
__O float DutyCycle=0; 
__O float Frequency=0;
void TIM1_CC_IRQHandler(void) //读到第二个
{
//中断 if判断+做什么+清除中断
   TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
	
   Value1_CCR1 = TIM_GetCapture1(TIM1);  //周期
   Value2_CCR2 = TIM_GetCapture2(TIM1);  //脉宽
	
   if( Value1_CCR1!=0 ) //一次PWM捕获完成
   {
	  DutyCycle = (float) ( ( (Value2_CCR2+1)*100 )/(Value1_CCR1+1) ); //寄存器从0开始所以要+1
	                                                                   //占空比是百分比 所以*100
	  Frequency = 72000000/72/(float)(Value1_CCR1+1); //u8强制转换浮点型才能运算
	                    //Fre = 72M(RCC*PSC)
	   
	   //printf("The value of DutyCycle is %f\r\n The value of Frequency is %f\r\n", DutyCycle, Frequency);
   }
   else //未捕获结束 占空比和频率值清0(不要清Value1_CCR1和Value2_CCR2)
   {
	   DutyCycle=0; 
       Frequency=0;
   }


	
}






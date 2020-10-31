#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	  
#include "timer_General.h"
/**
此实验配置1M的PWM的波   一般电机配置在10k-25k左右 
**/

//extern u16 small_Value;
//extern u16 big_Value;
int main(void)
{
   u32 temp;
   u32 all_time;
   delay_init();
   uart_init(115200);   
   LED_Init(); //初始化LED
/**初始化TIM2的捕获配置**/
   TIM_General_Capture_Init();
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组 

/*******************************************************/
/****************自己的思路******************************/	
/*******************************************************/
#if( Myslef_Open )  
while(1)
  { 
   if(Capture_value.CaptureStop_Bit == 1) //结束位置1
   {
	   all_time = Capture_value.Period_Value*(65535+1)+(Capture_value.Time_Value+1);
	                 //Period_Value和Time_Value都是从0开始计时的，因此要＋1
	   printf("本人！！！ 高电平时间为%d us \r\n" ,all_time);
   
       Capture_value.CaptureStop_Bit=0;  //捕获结束  防止循环打印
   }
	  

//LED表示程序在运行	  
	LED1=1;  
    delay_ms(100);
    LED1=0;  
    delay_ms(100);	  
	  
  } 
	 
}
#endif
	
/*********************************************************************/
/****************正点 此处没啥区别就是中断处正点比较秀****************/	
/*********************************************************************/
#if( ZhengDian_Open ) 
 while(1)
  { 
    delay_ms(100);
   if( (TIM2CH1_CAPTURE_STA&0x80)) //捕获结束
   {
	 temp = (TIM2CH1_CAPTURE_STA&0x3F)*(0xFFFF+1); //过滤出5~0位 看高位计数了多少位
     temp += (TIM2CH1_CAPTURE_VAL+1); //CNT从0开始计时
	   
	 printf("正点秀！！ 高电平时间为%d us\r\n", temp);
	 
     TIM2CH1_CAPTURE_STA &=0x00; //状态位清0	   
	  
   }

	  

//LED表示程序在运行	  
	LED1=1;  
    delay_ms(100);
    LED1=0;  
    delay_ms(100);	  
	  
  } 
	 
}

#endif


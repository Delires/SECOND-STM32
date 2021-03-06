#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	  
#include "timer_General.h"
#include "PWM_Capture.h"
/**
此实验配置1M的PWM的波   一般电机配置在10k-25k左右 
**/
int main(void)
{
   delay_init();
   uart_init(115200);   
   LED_Init(); //初始化LED
	
/**初始化TIM2的PWM配置  100kHz**/
   TIM_General_PWM_Init();
/**初始化TIM1 PWM输入捕获 1kHZ**/
   TIM_Advance_CapturePWM_Init(); 
	
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  
	
	
  while(1)
  { 
	//  printf("测试用");
   if( Value1_CCR1!=0 ) //一次PWM捕获完成
   {
	  printf("  The value of DutyCycle is %0.2f\r\n  The value of Frequency is %0.2f\r\n\r\n", DutyCycle, Frequency);   
   }

	  
    LED0=0;
	delay_ms(500);
	LED0=1;
	delay_ms(500);
  } 
	 
}
	


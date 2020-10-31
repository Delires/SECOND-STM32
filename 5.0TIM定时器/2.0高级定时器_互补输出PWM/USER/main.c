#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	  
#include "timer_advanced.h"
/**
此实验配置1M的PWM的波   一般电机配置在10k-25k左右 
**/
int main(void)
{
   delay_init();
   uart_init(115200);   
   LED_Init(); //初始化LED
/**初始化TIM1的PWM配置**/
   TIM_Advanced_Mode_Init(); 
  while(1)
  { 
    LED0=0;
	delay_ms(500);
	LED0=1;
	delay_ms(500);
  } 
	 
}
	

//
//作业：1.更改PWM频率
//      2.用通用定时器输出四路(CH1 CH2 CH3 CH4)PWM    通用定时器没互补输出


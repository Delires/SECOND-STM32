#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	  
#include "timer_General.h"
/**
��ʵ������1M��PWM�Ĳ�   һ����������10k-25k���� 
**/
int main(void)
{
   delay_init();
   uart_init(115200);   
   LED_Init(); //��ʼ��LED
/**��ʼ��TIM1��PWM����**/
   TIM_General_PWM_Init(); 
  while(1)
  { 
    LED0=0;
	delay_ms(500);
	LED0=1;
	delay_ms(500);
  } 
	 
}
	


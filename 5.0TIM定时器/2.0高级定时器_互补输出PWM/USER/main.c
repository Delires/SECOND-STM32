#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	  
#include "timer_advanced.h"
/**
��ʵ������1M��PWM�Ĳ�   һ����������10k-25k���� 
**/
int main(void)
{
   delay_init();
   uart_init(115200);   
   LED_Init(); //��ʼ��LED
/**��ʼ��TIM1��PWM����**/
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
//��ҵ��1.����PWMƵ��
//      2.��ͨ�ö�ʱ�������·(CH1 CH2 CH3 CH4)PWM    ͨ�ö�ʱ��û�������


#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	  
#include "timer_General.h"
#include "PWM_Capture.h"
/**
��ʵ������1M��PWM�Ĳ�   һ����������10k-25k���� 
**/
int main(void)
{
   delay_init();
   uart_init(115200);   
   LED_Init(); //��ʼ��LED
	
/**��ʼ��TIM2��PWM����  100kHz**/
   TIM_General_PWM_Init();
/**��ʼ��TIM1 PWM���벶�� 1kHZ**/
   TIM_Advance_CapturePWM_Init(); 
	
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  
	
	
  while(1)
  { 
	//  printf("������");
   if( Value1_CCR1!=0 ) //һ��PWM�������
   {
	  printf("  The value of DutyCycle is %0.2f\r\n  The value of Frequency is %0.2f\r\n\r\n", DutyCycle, Frequency);   
   }

	  
    LED0=0;
	delay_ms(500);
	LED0=1;
	delay_ms(500);
  } 
	 
}
	


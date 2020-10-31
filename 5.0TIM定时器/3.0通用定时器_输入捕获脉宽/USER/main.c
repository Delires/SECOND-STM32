#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	  
#include "timer_General.h"
/**
��ʵ������1M��PWM�Ĳ�   һ����������10k-25k���� 
**/

//extern u16 small_Value;
//extern u16 big_Value;
int main(void)
{
   u32 temp;
   u32 all_time;
   delay_init();
   uart_init(115200);   
   LED_Init(); //��ʼ��LED
/**��ʼ��TIM2�Ĳ�������**/
   TIM_General_Capture_Init();
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�жϷ��� 

/*******************************************************/
/****************�Լ���˼·******************************/	
/*******************************************************/
#if( Myslef_Open )  
while(1)
  { 
   if(Capture_value.CaptureStop_Bit == 1) //����λ��1
   {
	   all_time = Capture_value.Period_Value*(65535+1)+(Capture_value.Time_Value+1);
	                 //Period_Value��Time_Value���Ǵ�0��ʼ��ʱ�ģ����Ҫ��1
	   printf("���ˣ����� �ߵ�ƽʱ��Ϊ%d us \r\n" ,all_time);
   
       Capture_value.CaptureStop_Bit=0;  //�������  ��ֹѭ����ӡ
   }
	  

//LED��ʾ����������	  
	LED1=1;  
    delay_ms(100);
    LED1=0;  
    delay_ms(100);	  
	  
  } 
	 
}
#endif
	
/*********************************************************************/
/****************���� �˴�ûɶ��������жϴ�����Ƚ���****************/	
/*********************************************************************/
#if( ZhengDian_Open ) 
 while(1)
  { 
    delay_ms(100);
   if( (TIM2CH1_CAPTURE_STA&0x80)) //�������
   {
	 temp = (TIM2CH1_CAPTURE_STA&0x3F)*(0xFFFF+1); //���˳�5~0λ ����λ�����˶���λ
     temp += (TIM2CH1_CAPTURE_VAL+1); //CNT��0��ʼ��ʱ
	   
	 printf("�����㣡�� �ߵ�ƽʱ��Ϊ%d us\r\n", temp);
	 
     TIM2CH1_CAPTURE_STA &=0x00; //״̬λ��0	   
	  
   }

	  

//LED��ʾ����������	  
	LED1=1;  
    delay_ms(100);
    LED1=0;  
    delay_ms(100);	  
	  
  } 
	 
}

#endif


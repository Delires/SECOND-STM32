#include "led.h"
#include "DelayPriacy.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"	  


int main(void)
{
   u8 b, value;
   Config_Base_TIM6(1000, 71);  //初始化TIM6   10*(1/(72000000/(7199+1)))=0.001S =1ms
   delay_init();
   uart_init(115200);   
   NVIC_Base_TIM6();
   LED_Init(); //初始化LED
   value=0x00;
  while(1)
  {
	  if(time_save==1000)
				{
					printf("恒哥，我爱你");
					time_save = 0;
					
				}
	 
//	if(time_save==1000)  //1000ms=1s
//     {
//	   value=!value;
//	   time_save=0;
//	   LED0=value;
//       LED1=value;
//		 
//	   printf("%d", value);
//     } 
	 
  }
	
}


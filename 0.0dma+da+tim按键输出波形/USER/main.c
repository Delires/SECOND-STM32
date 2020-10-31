#include "delay.h"
#include "led.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "dac.h"
#include "exit.h"
#include "dma.h"
#include "tim.h"


int main(void)
{
	delay_init();	    	//��ʱ������ʼ��	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
  GPIOA_4();
	Dac1_Init(); 
	KEY_Init();
	LED_Init();
	exit_config();
  dma_config();  //dam��ʼ��
	Tim2_Config();
	
	
while(1)
	{
//		int a =sizeof(char);  //�ŵ�while(1)���û��ֵ
//		int b =sizeof(float);
//		int c =sizeof(double);
		RCC_ClocksTypeDef RCC_Clocks;
		RCC_GetClocksFreq(&RCC_Clocks);
		
		ConstantVol(3.0);  //����㶨��ѹ
		LED0=0;
		delay_ms(200);
		LED0=1;
		delay_ms(200);

	}
} 







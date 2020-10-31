#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "RCC_chaopin.h"
 
 

void Delay(uint32_t count)  //自减的延时是基于system频率减小的
{
	for(;count!=0;count--)
	{
		
	}
}

int main(void)
 {	
	LED_Init();		  	//初始化与LED连接的硬件接口
//使能PA8的MCO	
	MCO_Init();
	RCC_MCOConfig(RCC_MCO_SYSCLK);
//设置系统时钟system  HSE/HSI
    //HSE_SetClock(RCC_PLLMul_16); //X*8M
	HSI_SetClock(RCC_PLLMul_2);   //X*4M
	
	 

	while(1)
	{
		LED0=0;
		Delay(0xFFFFF);
		LED0=1;
		Delay(0xFFFFF);
	}
 }




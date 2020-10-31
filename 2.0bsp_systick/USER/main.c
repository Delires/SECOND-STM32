#include "stm32f10x.h"
#include "LED.h"
#include "delay.h"
int main(void)
{
	delay_init(); //初始化延时函数
	
	LED_Init(); //模块化编程，把初始化部分放到一个函数里直接调用
	while(1)
	{
		  GPIO_SetBits(GPIOE, GPIO_Pin_5);
		  GPIO_SetBits(GPIOB, GPIO_Pin_5);
		  
		  delay_ms(500);
		  
		  GPIO_ResetBits(GPIOE, GPIO_Pin_5);
		  GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		
		  delay_ms(500);
	}
}





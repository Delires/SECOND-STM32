#include "stm32f10x.h"
#include "key.h"
#include "delay.h"

void KEY_Init(void)//初始化PB12 13 14 15
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //打开PC使能端，引脚8 9 10 11
	
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU; //配置为上拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;

	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB8 PB9 PB10 PB11
		
}






int KEY_Scan(void) //初始化读键函数
{
	static int Key_up = 1;   //定义静态变量Key_up=1 即非连续读键值

	if(Key_up && (KEY1==0||KEY2==0||KEY3==0||KEY4==0))//非连续读键值 且 key1~4有见按下
	{
		delay_us(20); //去抖动
		Key_up=0;  //Key_up置0，非连续，当弹起时才为1，这样就不会连续读值
		
		if(KEY1==0)
		{
			return 1;
		}
		else if(KEY2==0)
		{
			return 2;
		}
		else if(KEY3==0)
		{
			return 3;
		}
		else if(KEY4==0)
		{
			return 4;
		}
  }
	
	else if(KEY1==1 && KEY2==1 && KEY3==1 && KEY4==1) //同时弹起来 就给Key_up重新赋值1，这样下次就能继续第一个循环读键值
	  	Key_up=1;
	return 0;
}

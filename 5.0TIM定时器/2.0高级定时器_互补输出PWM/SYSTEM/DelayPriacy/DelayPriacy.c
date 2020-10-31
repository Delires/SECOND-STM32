#include "DelayPriacy.h"
#include "sys.h"

uint16_t i;        



static u8 fac_us=0;    //延时微妙的频率，存放微妙毫秒的延时参数
static u16 fac_ms=0;   //延时毫秒的频率

void delay_init(void)//初始化函数声明
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //设置SysTick时钟为 外部时钟72MHZ
	fac_us = SystemCoreClock/8000000;  //SystemCoreClock就是配置好的时钟72MHZ
	                                  //72000000/1000000/8 就是1微秒，由于八分频，即外部计72下为1s 内部9下为1s，所以还要除8
  fac_ms = (u16)fac_us*1000; //72000000/1000/8是毫秒
}


void delay_us(u32 nus)  //us微秒延时函数
{
	u32 temp;
	SysTick->LOAD = nus*fac_us; //装载的值为  赋值的微妙*计数1us的次数
	SysTick->VAL = 0X00;        //写入数据让初始值清零
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //使能SysTick定时器
	do
	{
		temp = SysTick->CTRL; //一直计数，把计数的值赋给temp
	}while( (temp&0x01) && !( temp&(1<<16) ) );
	     //当temp第0位为1(计时器打开了) 且最高位不是1时(计数没停止时)
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭使能
	SysTick->VAL = 0X00; //重置计时器	
}


void delay_ms(u16 nms) //ms毫秒计时
{
	u32 temp;
	SysTick->LOAD = (u32)nms*fac_ms;   //重装值
	SysTick->VAL = 0x00;               //清零计时器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //打开使能端
	do
	{
		 temp = SysTick->CTRL; //CTRL寄存器计数的值赋给temp
	}while( (temp&0x01) && !( temp&(1<<16) ) );
	    //当CTRL寄存器0位为1(代表打开了) 且16位为1(代表计数停止了)
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭使能
	SysTick->VAL = 0X00; //重置计时器值
}


void Delay(uint32_t Count)  //定义延时函数，多变量延时
{
	for(; Count!=0; Count--)
	{
	 i = 5000;
	 while(i--);
	}	
}

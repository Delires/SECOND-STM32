#include "stm32f10x.h"
#include "key.h"   //和key.h相连
#include "DelayPriacy.h"  
#include "delay.h"





//定义按键初始化函数
void KEY_Init(void)  
{
	GPIO_InitTypeDef GPIO_InitStructure; //声明指针,必须放在最前头
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE, ENABLE); //打开GPIOA GPIOE使能端

//配置GPIOB为上拉输入，234脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //按键接地，则IO口是高电平，配置为上拉输入,不按读键是1 按下读键是0
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	//配置输入 没有输出速度
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

//配置GPIOA为下拉输入，0脚  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //按键接高电平，所以IO口处是下拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}




//定义按键类型选择，和返回按键值函数
u8 KEY_Scan(u8 mode)    //因为有返回值，所以是u8 xxx 而不是void xxx
{
	static u8 key_up = 1; //设置静态变量，否则下次调用的话，static就变成0了
	if(mode)
	{             //mode=1 连续型按键  key_up=1则永远能进入if()语句
		key_up = 1; //mode=0非连续型按键 必须等全部按键抬起来才会	key_up = 1 才能进入if()
	}
	if( key_up && (KEY0==0||KEY1==0||KEY2==0||WK_UP==1) ) 
	{      //KEY1 KEY2 KEY3 WK_UP分别是宏定义读键值
		Delay(10);
		key_up = 0; //视为按下

  //返回值确定 Switch语句到底哪个键按下		
		if(KEY0==0) return KEY0_PRES;   
		else if(KEY1==0) return KEY1_PRES;
		else if(KEY2==0) return KEY2_PRES;
		else if(WK_UP==1) return WKUP_PRES;
	}
	else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0) //即没有键按下
	{
		key_up = 1; //没有键按下，重新赋值 key_up=1，等待下次按下继续执行
		      
	}
	return 0;
}



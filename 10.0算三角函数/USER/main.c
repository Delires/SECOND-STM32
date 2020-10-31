#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "math.h"
 
#define PI 3.141592653f 
 
/************************************************

调用math.h头文件  函数cos() acos()....和弧度角度转换函数来计算三角函数
************************************************/


/************************************************
把角度值转换为弧度的函数   算正切用
输入参数:角度值
************************************************/
float ToRad(float degree)  //radian弧度  degree角度
{
	return (degree*PI/180.0);
}

/************************************************
把弧度制转换为角度的函数   算反三角函数
输入参数:弧度制
************************************************/
float ToAng(float radian)  //radian弧度  degree角度
{
	return (radian*180.0/PI);
}



 int main(void)
 {		 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口

/**************** 调用三角函数计算值********************************/
//正弦 余弦  正切	 
	printf("%f\r\n", cos( ToRad(60)) );
	printf("%f\r\n", sin( ToRad(60)) );
	printf("%f\r\n", tan( ToRad(60)) );

//反正弦 反余弦  反正切		 
	 printf("%f\r\n",  ToAng(acos(0.500000)) );
	 printf("%f\r\n",  ToAng(asin(0.866025)) );
	 printf("%f\r\n",  ToAng(atan(1.732051)) );
	

 	while(1)
	{
		LED0=1;
		LED1=1;
		delay_ms(500);
		
		LED0=0;
		LED1=0;
		delay_ms(500);
	}	

 }


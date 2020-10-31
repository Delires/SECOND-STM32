#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "math.h"
#include "trigfunction.h"

/************************************************
查表法计算三角函数值
1.先通过函数把所有角度的三角函数cos sin tan 的值搞出来
2.把他们构成数组，然后通过查表来计算值
3.计算的时候数组查即可
************************************************/


int main(void)
 {	
    //int i;
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口

	
/**************** 调用三角函数计算值********************************/
//输出0-360的正弦 余弦  正切	 
//	 for(i=0; i<361; i++)
//	 {
//	   printf("%.6f,", cos( ToRad(i)) );
//	 }
//	 printf("\r\n\r\n\r\n");
//	 
//	 for(i=0; i<361; i++)
//	 {
//	   printf("%.6f,", sin( ToRad(i)) );
//	 }
//	 printf("\r\n\r\n\r\n");
//	 
//	 for(i=0; i<361; i++)
//	 {
//	   printf("%.6f,", tan( ToRad(i)) );
//	 }

     printf("%.6f\r\n",  Look_cos(60));
	 printf("%.6f\r\n",  Look_sin(60));
	 printf("%.6f\r\n",  Look_tan(60));
	 
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


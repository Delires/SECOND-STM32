#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "math.h"
#include "trigfunction.h"

/************************************************
����������Ǻ���ֵ
1.��ͨ�����������нǶȵ����Ǻ���cos sin tan ��ֵ�����
2.�����ǹ������飬Ȼ��ͨ�����������ֵ
3.�����ʱ������鼴��
************************************************/


int main(void)
 {	
    //int i;
	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�

	
/**************** �������Ǻ�������ֵ********************************/
//���0-360������ ����  ����	 
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


#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "math.h"
 
#define PI 3.141592653f 
 
/************************************************

����math.hͷ�ļ�  ����cos() acos()....�ͻ��ȽǶ�ת���������������Ǻ���
************************************************/


/************************************************
�ѽǶ�ֵת��Ϊ���ȵĺ���   ��������
�������:�Ƕ�ֵ
************************************************/
float ToRad(float degree)  //radian����  degree�Ƕ�
{
	return (degree*PI/180.0);
}

/************************************************
�ѻ�����ת��Ϊ�Ƕȵĺ���   �㷴���Ǻ���
�������:������
************************************************/
float ToAng(float radian)  //radian����  degree�Ƕ�
{
	return (radian*180.0/PI);
}



 int main(void)
 {		 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�

/**************** �������Ǻ�������ֵ********************************/
//���� ����  ����	 
	printf("%f\r\n", cos( ToRad(60)) );
	printf("%f\r\n", sin( ToRad(60)) );
	printf("%f\r\n", tan( ToRad(60)) );

//������ ������  ������		 
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


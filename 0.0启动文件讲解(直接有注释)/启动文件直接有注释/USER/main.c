#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "RCC_chaopin.h"
 


void Delay(uint32_t count)  //�Լ�����ʱ�ǻ���systemƵ�ʼ�С��
{
	for(;count!=0;count--)
	{
		
	}
}

int main(void)
 {	
	int j,i=3; 
	
	int *l;
	l = (int*)malloc(sizeof(int));
	
	delay_init();
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
//ʹ��PA8��MCO	
	MCO_Init();
	
	
	RCC_MCOConfig(RCC_MCO_SYSCLK);
//����ϵͳʱ��system  HSE/HSI
    //HSE_SetClock(RCC_PLLMul_16); //X*8M
	HSI_SetClock(RCC_PLLMul_2);   //X*4M
	
	 
   j=ZZZ_ABC[7];
	while(1)
	{
		LED0=0;
		Delay(0xFFFFF);
		LED0=1;
		Delay(0xFFFFF);
	}
 }




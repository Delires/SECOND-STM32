/**
LED0 -- PB5
LED1 -- PE5   PC5
BEEF -- PB8   PC8

KEY0 -- PE2   PC2
KEY1 -- PE3   PC3
KEY2 -- PE4   PC4
WK_UP -- PA0
**/





#include "stm32f10x.h"

#include "DelayPriacy.h" 
#include "delay.h"
#include "sys.h"

#include "beef.h"
#include "key.h"
#include "led.h"

int main(void)
{
	u8 key=0;         //  typedef __IO uint8_t  vu8; �����ֵΪ0
	delay_init();
	BEEP_Init();
	LED_Init();
	KEY_Init();    //��ʱ��LED,BEEP,KEY������ʼ��
	
	LED0 = 0;     // ��Ϊ #define LED0 PBout(5) ֱ�Ӷ�PB5����  �ȵ������
	
	while(1)
	{
		key = KEY_Scan(0);  //0������  1����
		if(key) //����м�ֵ
		{
			switch(key)
			{
				case WKUP_PRES: BEEP=!BEEP; break;
				               //#define BEEP PBout(8)  ֱ�Ӷ�PB8�ڲ���
				case KEY2_PRES: LED0=!LED0; break;
				               //#define LED0 PBout(5)  ֱ�Ӷ�PB5�ڲ���
				case KEY1_PRES: LED1=!LED1; break;
				               //#define LED1 PEout(5)  ֱ�Ӷ�PB5�ڲ���
				case KEY0_PRES: LED0=!LED0; LED1=!LED1; break;
				
			}
		}
		else
		{
			Delay(10); //keyû�м�ֵ��һֱ�ȴ�
		}
	}
}




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
	u8 key=0;         //  typedef __IO uint8_t  vu8; 定义键值为0
	delay_init();
	BEEP_Init();
	LED_Init();
	KEY_Init();    //延时，LED,BEEP,KEY函数初始化
	
	LED0 = 0;     // 因为 #define LED0 PBout(5) 直接对PB5操作  先点亮红灯
	
	while(1)
	{
		key = KEY_Scan(0);  //0非连续  1连续
		if(key) //如果有键值
		{
			switch(key)
			{
				case WKUP_PRES: BEEP=!BEEP; break;
				               //#define BEEP PBout(8)  直接对PB8口操作
				case KEY2_PRES: LED0=!LED0; break;
				               //#define LED0 PBout(5)  直接对PB5口操作
				case KEY1_PRES: LED1=!LED1; break;
				               //#define LED1 PEout(5)  直接对PB5口操作
				case KEY0_PRES: LED0=!LED0; LED1=!LED1; break;
				
			}
		}
		else
		{
			Delay(10); //key没有键值就一直等待
		}
	}
}




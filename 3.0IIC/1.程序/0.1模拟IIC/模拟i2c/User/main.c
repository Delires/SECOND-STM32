/**
 
**/
  
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h" 
#include "./i2c/bsp_i2c_ee.h"
#include "bsp_i2c_gpio.h"

 /**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	u16 i;
    u8 WriteData[256];//����Ҫ�ڳ�ʼ��ǰ����ֵҪ�ڳ�ʼ����
	u8 ReadData[256];
    i2c_CfgGpio();
	LED_GPIO_Config();
    LED_BLUE;
    /* ���ڳ�ʼ�� */
	USART_Config();
    printf("eeprom ���ģ��i2c�������� \r\n");	
	
//д���ݵ��ȼ��IIC�Ƿ����  �����������������I2C_GPIO
   if (ee_CheckOk() == 0)
	{
		/* û�м�⵽EEPROM */
		printf("û�м�⵽����EEPROM!\r\n");
				
		return 0;
	}
	
	
	
    if(ee_Test() == 1)
  	{
			LED_GREEN;
    }
    else
    {
        LED_RED;
    }
    while(1);
}
/*********************************************END OF FILE**********************/

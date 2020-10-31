/**
 
**/
  
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h" 
#include "./i2c/bsp_i2c_ee.h"
#include "bsp_i2c_gpio.h"

 /**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	u16 i;
    u8 WriteData[256];//定义要在初始化前，赋值要在初始化后
	u8 ReadData[256];
    i2c_CfgGpio();
	LED_GPIO_Config();
    LED_BLUE;
    /* 串口初始化 */
	USART_Config();
    printf("eeprom 软件模拟i2c测试例程 \r\n");	
	
//写数据得先检查IIC是否存在  这个函数包含了配置I2C_GPIO
   if (ee_CheckOk() == 0)
	{
		/* 没有检测到EEPROM */
		printf("没有检测到串行EEPROM!\r\n");
				
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

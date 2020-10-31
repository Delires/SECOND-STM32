#ifndef __EEpromI2C_H
#define __EEpromI2C_H	 
#include "sys.h"

/**为增加移植性  用宏定义  改变串口直接改变宏即可 **/
//I2C宏名
#define           EEPROM_Address                0xA0  //8位地址
#define           STM32_I2C_OWM_Address         0x5f//定义一个32自己的地址(7位) 别和EEPROM一样

#define           EEPROM_I2C                    I2C1
#define           EEPROM_I2C_CLK                RCC_APB1Periph_I2C1
#define           EEPROM_I2C_APBxClockcmd       RCC_APB1PeriphClockCmd
#define           EEPROM_I2C_BAUDRATE           400000

//I2CGPIO宏
#define           EEPROM_I2C_SCL_GPIO_CLK        RCC_APB2Periph_GPIOB
#define           EEPROM_I2C_SDA_GPIO_CLK        RCC_APB2Periph_GPIOB
#define           EEPROM_I2C_GPIO_APBxClkCmd     RCC_APB2PeriphClockCmd 

#define           EEPROM_I2C_SCL_GPIO_PORT            GPIOB
#define           EEPROM_I2C_SCL_GPIO_PIN             GPIO_Pin_6
#define           EEPROM_I2C_SDA_GPIO_PORT            GPIOB
#define           EEPROM_I2C_SDA_GPIO_PIN             GPIO_Pin_7


void I2C_EEPROM_config(void); //I2C配置函数
void EEPROM_WaitForWriteEnd(void);//检测写完成函数
	
void EEPROM_Byte_Write(u8 add_word, u8 data);  //写一个字节  存储字地址 存储字值

void EEPROM_Read(u8 add_word, u8 *data, u8 Readnumber);//读数据  地址 数据 读字节数

void EEPROM_8Byte_Write(u8 add_word, u8 *data, u8 Writernumber);//写多个地址(一次最大不能超过8)	
#endif



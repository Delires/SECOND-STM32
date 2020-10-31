#ifndef __EEpromI2C_H
#define __EEpromI2C_H	 
#include "sys.h"

/**Ϊ������ֲ��  �ú궨��  �ı䴮��ֱ�Ӹı�꼴�� **/
//I2C����
#define           EEPROM_Address                0xA0  //8λ��ַ
#define           STM32_I2C_OWM_Address         0x5f//����һ��32�Լ��ĵ�ַ(7λ) ���EEPROMһ��

#define           EEPROM_I2C                    I2C1
#define           EEPROM_I2C_CLK                RCC_APB1Periph_I2C1
#define           EEPROM_I2C_APBxClockcmd       RCC_APB1PeriphClockCmd
#define           EEPROM_I2C_BAUDRATE           400000

//I2CGPIO��
#define           EEPROM_I2C_SCL_GPIO_CLK        RCC_APB2Periph_GPIOB
#define           EEPROM_I2C_SDA_GPIO_CLK        RCC_APB2Periph_GPIOB
#define           EEPROM_I2C_GPIO_APBxClkCmd     RCC_APB2PeriphClockCmd 

#define           EEPROM_I2C_SCL_GPIO_PORT            GPIOB
#define           EEPROM_I2C_SCL_GPIO_PIN             GPIO_Pin_6
#define           EEPROM_I2C_SDA_GPIO_PORT            GPIOB
#define           EEPROM_I2C_SDA_GPIO_PIN             GPIO_Pin_7


void I2C_EEPROM_config(void); //I2C���ú���
void EEPROM_WaitForWriteEnd(void);//���д��ɺ���
	
void EEPROM_Byte_Write(u8 add_word, u8 data);  //дһ���ֽ�  �洢�ֵ�ַ �洢��ֵ

void EEPROM_Read(u8 add_word, u8 *data, u8 Readnumber);//������  ��ַ ���� ���ֽ���

void EEPROM_8Byte_Write(u8 add_word, u8 *data, u8 Writernumber);//д�����ַ(һ������ܳ���8)	
#endif



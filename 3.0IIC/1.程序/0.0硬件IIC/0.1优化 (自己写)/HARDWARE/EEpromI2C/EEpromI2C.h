#ifndef __EEpromI2C_H
#define __EEpromI2C_H	 
#include "sys.h"

extern u8 ReadData[256];
extern u8 WriteData[256];

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

//�궨��ʱ�� ����ʱ����
#define I2C_Short_time   ((u32)0x1000)  
#define I2C_Long_time   ((u32)0xa000)

//���ڶ���
#define I2C_PAGESIZE  8

//DEBUG ��ӡ�������
#define EEPROM_DEBUG(message,agr...)  printf("Debug: file:[%s] line:[%d] Message:"message" \r\n",__FILE__, __LINE__, ##agr)

//printf�궨�� ��ӡ������Ϣ
#define EEPROM__ERROR(message,agr...)    printf("<I2C ERROR>"message"\r\n",##agr);





void I2C_configation(void);
u32 EEPROM_WaitForWriteEnd(void);//���д��ɺ���


u32 EEPROM_Byte_Write(u8 add_word, u8 *data);  //дһ���ֽ�  �洢�ֵ�ַ �洢��ֵ  ����ֵ�Ǵ�����Ϣ
u32 EEPROM_Page_Write(u8 add_word, u8 *Buff_data, u8 Writernumber);//д�����ַ(һ������ܳ���8)	
u32 Buffer_Write(u8 Buffer_Addr, u8 *Buff_data, u8 BufferNumber);  //��ַ�Զ������д����


void EEPROM_Read(u8 add_word, u8 *data, u8 Readnumber);//������  ��ַ ���� ���ֽ���


#endif



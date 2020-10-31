#ifndef __EEpromI2C_H
#define __EEpromI2C_H	 
#include "sys.h"

extern u8 ReadData[256];
extern u8 WriteData[256];

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

//宏定义时间 做超时处理
#define I2C_Short_time   ((u32)0x1000)  
#define I2C_Long_time   ((u32)0xa000)

//用于对齐
#define I2C_PAGESIZE  8

//DEBUG 打印输出行列
#define EEPROM_DEBUG(message,agr...)  printf("Debug: file:[%s] line:[%d] Message:"message" \r\n",__FILE__, __LINE__, ##agr)

//printf宏定义 打印错误信息
#define EEPROM__ERROR(message,agr...)    printf("<I2C ERROR>"message"\r\n",##agr);





void I2C_configation(void);
u32 EEPROM_WaitForWriteEnd(void);//检测写完成函数


u32 EEPROM_Byte_Write(u8 add_word, u8 *data);  //写一个字节  存储字地址 存储字值  返回值是错误信息
u32 EEPROM_Page_Write(u8 add_word, u8 *Buff_data, u8 Writernumber);//写多个地址(一次最大不能超过8)	
u32 Buffer_Write(u8 Buffer_Addr, u8 *Buff_data, u8 BufferNumber);  //地址自动对齐的写函数


void EEPROM_Read(u8 add_word, u8 *data, u8 Readnumber);//读数据  地址 数据 读字节数


#endif



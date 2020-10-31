#ifndef __EEpromI2C_H
#define __EEpromI2C_H	 
#include "sys.h"

/**为增加移植性  用宏定义  改变串口直接改变宏即可 **/
//I2C宏名
#define           EEPROM_Address                0xA0  //8位地址
#define           STM32_I2C_OWM_Address         0x5f//定义一个32自己的地址(7位) 别和EEPROM一样

/********** eeprom 一页大小 和总内存****************/
#define           EEPROM_PAGE_SIZE              8
#define           EEPROM_SIZE                   256



/********debug 部分 输出错误信息**********************/
#define EEPROM_DEBUG(message,agr...)  printf("\r\nDebug: file:[%s] line:[%d] Message:"message" \r\n",__FILE__, __LINE__, ##agr)

//printf宏定义 打印错误信息
#define EEPROM__ERROR(message,agr...)    printf("<I2C ERROR>"message"\r\n",##agr);



/********函数名部分**********************/
u8 ee_CheckOk(void);  //检测设备存在函数

u8 ee_ReadBytes(u8 SaveAddress, u8 *ReadBuf, u16 Size); //读数据函数
u8 ee_WriteBytes(u8 SaveAddress, u8 *WriteBuf, u16 Size); //写数据函数


void ee_Delay(__IO uint32_t nCount); //延时函数

#endif





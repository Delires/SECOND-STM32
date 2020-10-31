#ifndef __SPIFlash_H
#define __SPIFlash_H	 
#include "sys.h"

/**********************************************************/
/**为增加移植性  用宏定义  改变串口直接改变宏即可 ********/
/**宏定义真的超级能增强程序的移植性，硬件部分一定要宏定义 太舒服了 ********/
/**********************************************************/

extern u8 ReadData[4096];
extern u8 WriteData[4096];
#define        ONE_WRIT_MORENUMBER        256  //一次最多写256

/*************************************************************************/
/***********************硬件部分定义 可改变用来兼容匹配 ***********************************/
/*************************************************************************/
/*********************** 宏定义 SPI通讯的引脚  ***********************************/
//战舰用的SPI2 PB12/13/14/15 
#define           SPI_NUMBER                    SPI2  

#define           SPI_GPIO_PORT                 GPIOB      
#define           SPI_NSS_CS_PIN                GPIO_Pin_12  
#define           SPI_SCK_PIN                   GPIO_Pin_13 
#define           SPI_MISO_PIN                  GPIO_Pin_14 
#define           SPI_MOSI_PIN                  GPIO_Pin_15 


/*********************** SPI通讯的一些函数名 ***********************************/
//SPI使能  定义
#define           SPI_APBxClock_Fun             RCC_APB1PeriphClockCmd
#define           SPI_CLK                       RCC_APB1Periph_SPI2
//GPIO使能 定义
#define           GPIO_SPI_APBxClock_Fun        RCC_APB2PeriphClockCmd
#define           GPIO_SPI_CLK                  RCC_APB2Periph_GPIOB

//宏定义函数 设置SPI通讯使能(0使能)
#define           FLASH_SPI_CS_HIGH             GPIO_SetBits(SPI_GPIO_PORT, SPI_NSS_CS_PIN) 
#define           FLASH_SPI_CS_LOW              GPIO_ResetBits(SPI_GPIO_PORT, SPI_NSS_CS_PIN)            
                


/*************************************************************************/
/*********************** SPIFLASH 指令 ***********************************/
/*************************************************************************/
#define            DUMMY                        0x00
//读寄存器指令
#define            Read_Status_Register_1       0x05 //(S7-S0)
#define            Read_Status_Register_2       0x35  //(S15-S8)
#define            Read_Status_Register_3       0x15  //(S23-S16)
//写寄存器
#define            Write_Status_Register_1      0x01
#define            Write_Status_Register_2      0x31
#define            Write_Status_Register_3      0x11

//读ID指令
#define            JEDEC_ID                     0x9F
#define            Device_ID                    0x90
//擦出指令
#define            Sector_Erase                 0x20   //4k (sector is mini unit)
#define            Block_Erase                  0x52   //32k
#define            Block_Erase_2                0xD8   //64k
#define            Chip_Erase                   0xC7   //ALL
//读扇区值
#define            Read_Data                    0x03
//写扇区值
#define            Page_Program                 0x02
//写使能  
#define            Write_Enable                 0x06
#define            Write_Disable                0x04
//睡眠
#define            Power_down                   0xb9
#define            Release_Powerdown            0xab //唤醒
//设备标号
#define            W25Q128                      0xEF4018
/*************************************************************************/
/*********************** SPI的相关函数 ***********************************/
/*************************************************************************/
void SPI_Communit_Init(void);

u8 SPI_FLASH_Send_Byte(u8 data); //写1byte+读
u8 SPI_FLASH_Read_Byte(void);  //读1byte 调用上函数
void SPI_Flash_Write_Enable(void); //写使能


u32 SPI_FLASH_Read_ID(void);  //读取读取Flash的ID号ID
u32 SPI_FLASH_SHORT_ID(void);
//u32 SPI_FLASH_Read_ID2(void);

void Wait_Erase_Write_End(void); 
void Flash_Sector_Erase(u32 address);

void Read_Sector_Flash(u32 address, u8 *buffer, u32 ReadByteNumber);

u8 Write_Less256byte_Flash(u32 address, u8 *writeBuffer, u32 numWriteByte);
u8 Write_More_256Byte(u32 address, u8 *writeBuffer, u32 numWriteByte);  //写多字节 >256
void Write_More_256Byte_YeHuo(u32 Maddress, u8 *MwriteBuffer, u32 numWriteByte);


void Power_Down_Flash(void);
void Release_Power_Down_Flash(void);









/*************************************************************************/
/*********************** Debug部分  Linux ********************************/
/*************************************************************************/
/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))
#define PRINTF                     1
/*信息输出*/
#define  FLASH_DEBUG(message, connect...)      printf("\r\nDebug: File[%s]  Line[%d]  "message"   \r\n",__FILE__, __LINE__, ##connect);                   
                                                     //结构为 "    "message" "
													 //"message" 是调用  FLASH_DEBUG("")  时里头的值
													 //"       " 是原本的值FLASH_DEBUG("")里有没有都会打印

#if(PRINTF)
   #define  PROGRAM_DEBUG(message, connect...)      printf("\r\nDebug: File[%s]  Line[%d]  "message"   \r\n",__FILE__, __LINE__, ##connect);                   
                                                     //结构为 "    "message" "
												  //"message" 是调用  FLASH_DEBUG("")  时里头的值
												   //"       " 是原本的值FLASH_DEBUG("")里有没有都会打印
   #define  PROGRAM_PRINTF(message, connect...)      printf(" "message" \r\n", ##connect);
#else													
   #define  PROGRAM_DEBUG(message, connect...)        0
   #define  PROGRAM_PRINTF(message, connect...)     0
#endif




#endif





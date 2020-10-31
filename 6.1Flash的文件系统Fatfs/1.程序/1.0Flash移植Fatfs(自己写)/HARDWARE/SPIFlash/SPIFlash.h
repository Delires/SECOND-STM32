#ifndef __SPIFlash_H
#define __SPIFlash_H	 
#include "sys.h"

/**********************************************************/
/**Ϊ������ֲ��  �ú궨��  �ı䴮��ֱ�Ӹı�꼴�� ********/
/**�궨����ĳ�������ǿ�������ֲ�ԣ�Ӳ������һ��Ҫ�궨�� ̫����� ********/
/**********************************************************/

extern u8 ReadData[4096];
extern u8 WriteData[4096];
#define        ONE_WRIT_MORENUMBER        256  //һ�����д256

/*************************************************************************/
/***********************Ӳ�����ֶ��� �ɸı���������ƥ�� ***********************************/
/*************************************************************************/
/*********************** �궨�� SPIͨѶ������  ***********************************/
//ս���õ�SPI2 PB12/13/14/15 
#define           SPI_NUMBER                    SPI2  

#define           SPI_GPIO_PORT                 GPIOB      
#define           SPI_NSS_CS_PIN                GPIO_Pin_12  
#define           SPI_SCK_PIN                   GPIO_Pin_13 
#define           SPI_MISO_PIN                  GPIO_Pin_14 
#define           SPI_MOSI_PIN                  GPIO_Pin_15 


/*********************** SPIͨѶ��һЩ������ ***********************************/
//SPIʹ��  ����
#define           SPI_APBxClock_Fun             RCC_APB1PeriphClockCmd
#define           SPI_CLK                       RCC_APB1Periph_SPI2
//GPIOʹ�� ����
#define           GPIO_SPI_APBxClock_Fun        RCC_APB2PeriphClockCmd
#define           GPIO_SPI_CLK                  RCC_APB2Periph_GPIOB

//�궨�庯�� ����SPIͨѶʹ��(0ʹ��)
#define           FLASH_SPI_CS_HIGH             GPIO_SetBits(SPI_GPIO_PORT, SPI_NSS_CS_PIN) 
#define           FLASH_SPI_CS_LOW              GPIO_ResetBits(SPI_GPIO_PORT, SPI_NSS_CS_PIN)            
                


/*************************************************************************/
/*********************** SPIFLASH ָ�� ***********************************/
/*************************************************************************/
#define            DUMMY                        0x00
//���Ĵ���ָ��
#define            Read_Status_Register_1       0x05 //(S7-S0)
#define            Read_Status_Register_2       0x35  //(S15-S8)
#define            Read_Status_Register_3       0x15  //(S23-S16)
//д�Ĵ���
#define            Write_Status_Register_1      0x01
#define            Write_Status_Register_2      0x31
#define            Write_Status_Register_3      0x11

//��IDָ��
#define            JEDEC_ID                     0x9F
#define            Device_ID                    0x90
//����ָ��
#define            Sector_Erase                 0x20   //4k (sector is mini unit)
#define            Block_Erase                  0x52   //32k
#define            Block_Erase_2                0xD8   //64k
#define            Chip_Erase                   0xC7   //ALL
//������ֵ
#define            Read_Data                    0x03
//д����ֵ
#define            Page_Program                 0x02
//дʹ��  
#define            Write_Enable                 0x06
#define            Write_Disable                0x04
//˯��
#define            Power_down                   0xb9
#define            Release_Powerdown            0xab //����
//�豸���
#define            W25Q128                      0xEF4018
/*************************************************************************/
/*********************** SPI����غ��� ***********************************/
/*************************************************************************/
void SPI_Communit_Init(void);

u8 SPI_FLASH_Send_Byte(u8 data); //д1byte+��
u8 SPI_FLASH_Read_Byte(void);  //��1byte �����Ϻ���
void SPI_Flash_Write_Enable(void); //дʹ��


u32 SPI_FLASH_Read_ID(void);  //��ȡ��ȡFlash��ID��ID
u32 SPI_FLASH_SHORT_ID(void);
//u32 SPI_FLASH_Read_ID2(void);

void Wait_Erase_Write_End(void); 
void Flash_Sector_Erase(u32 address);

void Read_Sector_Flash(u32 address, u8 *buffer, u32 ReadByteNumber);

u8 Write_Less256byte_Flash(u32 address, u8 *writeBuffer, u32 numWriteByte);
u8 Write_More_256Byte(u32 address, u8 *writeBuffer, u32 numWriteByte);  //д���ֽ� >256
void Write_More_256Byte_YeHuo(u32 Maddress, u8 *MwriteBuffer, u32 numWriteByte);


void Power_Down_Flash(void);
void Release_Power_Down_Flash(void);









/*************************************************************************/
/*********************** Debug����  Linux ********************************/
/*************************************************************************/
/*�ȴ���ʱʱ��*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))
#define PRINTF                     1
/*��Ϣ���*/
#define  FLASH_DEBUG(message, connect...)      printf("\r\nDebug: File[%s]  Line[%d]  "message"   \r\n",__FILE__, __LINE__, ##connect);                   
                                                     //�ṹΪ "    "message" "
													 //"message" �ǵ���  FLASH_DEBUG("")  ʱ��ͷ��ֵ
													 //"       " ��ԭ����ֵFLASH_DEBUG("")����û�ж����ӡ

#if(PRINTF)
   #define  PROGRAM_DEBUG(message, connect...)      printf("\r\nDebug: File[%s]  Line[%d]  "message"   \r\n",__FILE__, __LINE__, ##connect);                   
                                                     //�ṹΪ "    "message" "
												  //"message" �ǵ���  FLASH_DEBUG("")  ʱ��ͷ��ֵ
												   //"       " ��ԭ����ֵFLASH_DEBUG("")����û�ж����ӡ
   #define  PROGRAM_PRINTF(message, connect...)      printf(" "message" \r\n", ##connect);
#else													
   #define  PROGRAM_DEBUG(message, connect...)        0
   #define  PROGRAM_PRINTF(message, connect...)     0
#endif




#endif





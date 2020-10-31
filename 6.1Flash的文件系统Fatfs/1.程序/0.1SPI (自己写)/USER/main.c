#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "SPIFlash.h"

#define   countsize(a)     ( sizeof(a)/sizeof(*a) )  //计算字符串数组长度
#define   char_long        (countsize(WriteData_char) - 1) //数组长是从0开始

u8 WriteData_char[]="加油！ 奥利给！！！";
u8 ReadData_char[char_long];

u8 ReadData[4096]={0};
u8 WriteData[4096]={0};

float Float_WriteData[20]={0};
float Float_ReadData[20]={0};

 int main(void)
 {	
	 u8 i;
	 int j,l;
	 int m;
	 u32 spi_id, spi_id2;
	 
	 SPI_Commit_Init();
	 delay_init();
	 uart_init(115200);
	 LED_Init();
	 
     printf("\r\n 程序由NoMax(无极)所有 \r\n 供大家参考\r\n 这是一个SPIFLASH实验 SPI2--W25Q128\r\n ");

	 Wait_Erase_Write_End(); //等待上次擦出完成
     Flash_Sector_Erase(0);
            
		  //FLASH_DEBUG("调试 %d", 7);
/************************* 检查设备地址*************************/  
		 spi_id = SPI_FLASH_Read_ID();
		 spi_id2=SPI_FLASH_SHORT_ID();
        printf("\r\n JEDEC_ID: 0x%X", spi_id);
		 printf("\r\n Device_ID: 0x%X\r\n", spi_id2);

/***************  写数据 **************************************/
//		/** 擦除Flash **/
//		Flash_Sector_Erase( 0x000000 );

//		/** 写Flash **/
//		printf("\r\n\r\n\r\n\r\n\r\n\r\n"); 
//		for(j=0; j<500; j++)//赋值
//		{
//			l=j;
//			if(l>255)
//			{
//				l/=2;
//			}
//			WriteData[j]=l;
//			
//		}
//		printf("\r\n 往Flash存储的的数据为\r\n"); 
//		for(j=0; j<500; j++) //打印向Flash写入值
//		{
//			printf("0x%04X ", WriteData[j]); 
//			if(j%10 == 9)
//			{
//				printf("\r\n");
//			}
//		}
//		//Write_Less256byte_Flash(0x000000, WriteData,200); //写入<256字节
//		Write_More_256Byte_YeHuo(0x000302, WriteData,500); //写入>256字节
//		/*** 读取FLSH ***/ 
//		Read_Sector_Flash(0x000302, ReadData, 500);   //1k = 1024byte (不是位)
//		printf("\r\n Flash读取的数据为\r\n"); 
//		for(j=0; j<500; j++) //打印向Flash读出值
//		{
//			printf("0x%04X ", ReadData[j]); 
//			if(j%10 == 9)
//			{
//				printf("\r\n");
//			}
//		}

//		
///***************  写字符串 **************************************/
//        /** 擦除Flash **/
//		Flash_Sector_Erase( 0x000302 );
//		printf("----------------往Flash写入字符串------------\r\n"); 
//		/** 写Flash **/
//		printf("\r\n\r\n\r\n\r\n\r\n\r\n"); 
//		printf("\r\n 往Flash存储的的数据为:\r\n%s\r\n", WriteData_char );  //打印字符串 直接输数组名
//     	Write_Less256byte_Flash(0x000302, WriteData_char, char_long);

//		/*** 读取FLSH ***/ 
//		Read_Sector_Flash(0x000302, ReadData_char, char_long);   //1k = 1024byte (不是位)
//		printf("\r\n Flash读取的数据为:\r\n%s\r\n", ReadData_char); 

//		
///***************  写小数 **************************************/
//		/** 擦除Flash **/
//	
//		Flash_Sector_Erase( 0x000302 );
//        printf("----------------往Flash写入字符串------------\r\n"); 
//		/** 写Flash **/
//		for(j=0; j<20; j++)//赋值
//		{
//			Float_WriteData[j]=j+0.1*j;
//		}
//		printf("\r\n 往Flash存储的小数的数据为\r\n"); 
//		for(j=0; j<20; j++) //打印向Flash写入值
//		{
//			printf("%f \r\n", Float_WriteData[j]); 
//		}
//		Write_More_256Byte_YeHuo(0x000302, (void*)Float_WriteData, sizeof(Float_WriteData)); //写入>256字节
//		//Write_Less256byte_Flash(0x000000, (void*)Float_WriteData, 20);   //注意float型数据的大小 4byte 因此写一个小数要发生四次
//		/** 读Flash **/
        Read_Sector_Flash(0x000400, (void*)Float_ReadData, sizeof(Float_WriteData));
		printf("\r\n 读取的小数的数据为\r\n");
		for(j=0; j<20; j++) //打印向Flash写入值
		{
			printf("%c \r\n", Float_ReadData[j]); 
		}

		
/************************ 基本部分 ************************/	
	 while(1)
	 {
//		   i++;	
//		  delay_ms(500);
//		  LED1=!LED1;//显示函数正在运行
//		  if(i % 10 == 0)
//		  {
//			  i = 0;
//			   printf("\r\n 程序由NoMax(无极)所有 \r\n 供大家参考\r\n 这是一个SPIFLASH实验 SPI2--W25Q128\r\n ");

//		  }
//	  

	 }	 
 }


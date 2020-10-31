#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "SPIFlash.h"

#define   countsize(a)     ( sizeof(a)/sizeof(*a) )  //计算字符串数组长度
#define   char_long        (countsize(WriteData_char) - 1) //数组长是从0开始


 int main(void)
 {	
	 u8 i;

	 u32 spi_id, spi_id2;
	 
	 SPI_Commit_Init();
	 delay_init();
	 uart_init(115200);
	 LED_Init();
	 Init_MPU9250();      
     printf("\r\n 程序由NoMax(无极)所有 \r\n 供大家参考\r\n 这是一个SPIFLASH实验 SPI2--W25Q128\r\n ");

	 
	 
      
	 while(1)
	 {
		  i++;	
		 //FLASH_DEBUG("调试 %d", 7);
/************************* 读9250的设备地址 *************************/  
		 spi_id=SPI_FLASH_Read_ID();
         printf("\r\n MPU9250_ID: 0x%X", spi_id);
/************************* AK8963磁力计的设备地址 *************************/  

		 

///***************  写数据 **************************************/
//		Write_More_256Byte_YeHuo(0x000015, WriteData,500); //写入>256字节

//		printf("\r\n Flash读取的数据为\r\n"); 
//		for(j=0; j<500; j++) //打印向Flash读出值
//		{
//		}
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		  delay_ms(500);
		  LED1=!LED1;//显示函数正在运行
		  if(i % 10 == 0)
		  {
			  i = 0;
			   printf("\r\n 程序由NoMax(无极)所有 \r\n 供大家参考\r\n 这是一个SPIFLASH实验 SPI2--W25Q128\r\n ");

		  }
	  

	 }	 
 }


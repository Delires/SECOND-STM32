#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "EEpromI2C.H"
 
 //1,2,3,4,5,6,7,8,9
 int main(void)
 {	
    u16 i=0;	 
	u16 times=0;
	u8 ReadData[256]={0};
	u8 WriteData[256]={0};
	for(i=0;i<256;i++)
	{
		WriteData[i]=(255-i);
	}
	
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
    I2C_EEPROM_config(); //i2c初始化

////写一个字节读	
//    printf("IIC读1个字节数据实验\r\n");
//	EEPROM_Byte_Write(0x09, 0x55);  //写一个字节  存储字地址 存储字值	
//	EEPROM_WaitForWriteEnd(); //检测写结束
//    EEPROM_Read(0x11, ReadData, 1);
//	//显示
//	printf("接收的数据是:0x%x\r\n", ReadData[0]);
//	
//	printf("\r\n");

//	
////连续写8个字节读	
//	printf("IIC读8个字节数据实验\r\n");
//	printf("接收的数据是:");
//	EEPROM_8Byte_Write(0x08, WriteData, 8);
//	EEPROM_WaitForWriteEnd(); //检测写结束
//    EEPROM_Read(0x08, ReadData, 8);
//	for(i=0; i<8; i++)
//	{
//		printf("0x%x  ", ReadData[i]);
//	}
//    printf("\r\n\r\n\r\n");
	
	
//打印数组
printf("EEPROM写满实验\r\n ");
	for(i=0; i<256; i++)
	{
		printf("0x%02x ", WriteData[i]);
		if(i%16==15)//因为是从0开始 第15个数回车  第31个数回车 第47，63，79，95打印回车
		{           //而如果i%15==0的话 则0，15，30，45打印回车 则每15个数回车
			        //而如果i%16==0的话 则0，16，32，48打印回车 虽然每16个数回车  但一开始第0个数就回车 照样对不齐
			printf("\r\n ");
		}
			
	}
//写满	
	for(i=0;i<255;i++)
	{
		//EEPROM_8Byte_Write(0x00+i, WriteData, 8);
		EEPROM_Byte_Write(0x00+i, 255-i);
		EEPROM_WaitForWriteEnd(); //检测写结束
		}
//	}

	
    printf("---------------------分隔符----------\r\n ");
	EEPROM_WaitForWriteEnd(); //检测写结束
	

	EEPROM_Read(0x00, ReadData, 255); 
	for(i=0; i<256; i++)
	{
		printf("0x%02x ", ReadData[i]);
		if(i%16==15)
		{
			printf("\r\n ");
		}
			
	}

			

	
	
	while(1)
	{
	  times++;
	  if(times%50000==0)
	  {
		LED1=!LED1;
		 delay_ms(300);	  
	  }
	}	 
 }


#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "EEpromI2C.H"

u8 WriteData[256];
u8 ReadData[256];
 int main(void)
 {	
	u8 *number; 
    u16 i,j=0;	 
	u16 times=0;
	
	//*number=45;//     //指针跑飞的原因，这是悬空指针，虽然有值但是在乱指，并不知道指针具体指向哪
	number=&ReadData[1];//这才是正确的指针，指针指的方向是具体的，是栈开辟的一端内存
	
   /**初始化配置**/	
	delay_init();
	uart_init(115200);
	LED_Init();
	I2C_configation();
	
	for(i=0;i<256;i++)
	{
		WriteData[i]=(255-i);
	}
	
	
////写一字节测试	
//	EEPROM_Byte_Write(0, number);
//	EEPROM_WaitForWriteEnd();
//	EEPROM_Read(0,ReadData,1);
//	printf("%d\r\n",ReadData[0]);
	
	

	for(j=0; j<5;j++)
	{
	     printf("\r\n0x%02x", 0x00+j);
		 EEPROM_Page_Write(0x00+j, WriteData, 6);  //如果对齐写数据，超过8byte就会出现问题 应该是硬件IIC的BUG
		 EEPROM_WaitForWriteEnd(); 
		 EEPROM_Read(0x00+j, ReadData, 6);
		 printf("\r\n");
		 for(i=0;i<20;i++)
		 {
			printf("%d ",ReadData[i]);
		 }
    }
	
//	for(j=0; j<5;j++)
//	{
//	     printf("\r\n0x%02x", 0x00+j);
//		 Buffer_Write(0x00+j, WriteData, 6);  //如果对齐写数据，超过8byte就会出现问题 应该是硬件IIC的BUG
//		 EEPROM_Read(0x00+j, ReadData, 6);
//		 printf("\r\n");
//		 for(i=0;i<20;i++)
//		 {
//			printf("%d ",ReadData[i]);
//		 }
//    }
/****************************测试宏定义的 printf函数******************************************************************/	
	//EEPROM_DEBUG("666666=%d",6);
	//EEPROM__ERROR("错误代码为%d",i);
    // TXET("666666=%d",6);  //printf宏定义的测试函数  
	
	
	
	
	
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


#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "EEpromI2C.h"
#include "IIC_gpio.h"
 
u8 WriteData[256]={0};
u8 ReadData[256]={0};

long double WriteFloat[8]={0};
long double ReadFloat[8]={0};
char WriteChar[30]="Keep foolish keep hungery";
char ReadChar[30];
	 
	 
int main(void)
 {	
    int times,i;

/**初始化必不可少**/
	 delay_init();
	 uart_init(115200);
	 LED_Init();
	 IIC_GPIO_Config();

/**初始化必不可少**/
	 delay_init();
	 uart_init(115200);
	 LED_Init();
	 IIC_GPIO_Config();

	 
/**************** ************* ************* ********/
/**************** 一次性写满************* ********/
/**************** ************* ************* ********/
	 //数组赋值	 
	 for(i=0; i<256; i++)
	 {
		 WriteData[i]=i;
	 }


//判断eeprom写数据是否成功 
//	ee_WriteBytes(0X00, WriteData,256)
//	 YHee_WriteBytes(WriteData, 0x00, 256)
	 if( ee_WriteBytes(0x00, WriteData,256) ==1)
	 {
	    printf("\r\n模拟IIC写数据成功 数据为：\r\n");
		for(i=0; i<256; i++)
	    {
			 printf("0x%02X ", WriteData[i]);
			 if(i%16 == 15)
			 {
				 printf("\r\n");
			 }
	     }
	 }
	 else
	 {
		 printf("\r\n模拟IIC写数据失败\r\n");
	 }
	 
	 
	 ee_Delay(0x0FFFFF); //延时 再读数据 否则会出错


	 //判断读数据是否成功
	 if( ee_ReadBytes(0x00, ReadData, 256)==1)
	 {
		 printf("\r\n模拟IIC读到的数据为：\r\n");
		 //size是255 因为从0开始读 到255一共256个
		 for(i=0; i<256; i++)
		 {
			 printf("0x%02X ", ReadData[i]);
			 if(i%16 == 15)
			 {
				 printf("\r\n");
			 }
		 }
	 		 
	 }
	 else
	{
		 printf("\r\n模拟IIC读数据失败\r\n");
	}
	
	
	
/**************** ************* ************* ********/		 
/************* EEPROM存储小数 dobule******************/	
/**************** ************* ************* ********/	
    ee_Delay(0x0FFFFF); 
	for(i=0; i<8; i++)
	{
		WriteFloat[i]=i*10+i*0.1;
	}
	ee_WriteBytes(0x00, (void*)WriteFloat,sizeof(WriteFloat));
	ee_Delay(0x0FFFFF); 
	ee_ReadBytes(0x00, (void*)ReadFloat, sizeof(ReadFloat));
	            //(void*)ReadFloat WriteFloat一个元素是8byte，(void*)空指针将WriteFloat传递给函数时，将元素自动分割成一个字节一个字节来储存
	            //即10.1二进制位1010.00011001100110011
	            //在内存中存储的值为0 10000010 01000110011001100110011 总共8个字节，分存在了地址0x00~0x03中  读数据则分别安每字节读取然后结合存在ReadFloat中
	printf("\r\n\r\nEEPROM读到的浮点数为：\r\n");
	for(i=0; i<8; i++)
	{
		printf("%LF\r\n", ReadFloat[i]);
	}
	
	
/**************** ************* ************* ********/
/************* EEPROM存储字符串 char******************/	 
/**************** ************* ************* ********/
	ee_Delay(0x0FFFFF); 
	ee_WriteBytes(0x60, (void*)WriteChar,sizeof(WriteChar));
	ee_Delay(0x0FFFFF); 
	ee_ReadBytes(0x60, (void*)ReadChar, sizeof(ReadChar));
	            //(void*))WriteChar  (void*)空指针将)WriteChar传递给函数时，将元素自动分割成一个字节一个字节来储存
	      
	printf("\r\n\r\nEEPROM读到的字符串为：\r\n");
	printf("%s", ReadChar);

	
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



//int main(void)
// {	
//    int times,i=0;
//    u8 j;
//	 u8 *pin;
//	 
///**初始化必不可少**/
//	 delay_init();
//	 uart_init(115200);
//	 LED_Init();
//	 IIC_GPIO_Config();
//	 
//	 
//	 pin = &j;
//	 ee_WriteBytes(0x00, (void*)&i, 1);
//	 ee_Delay(0x0FFFFF); 
//	 ee_ReadBytes(0x00, pin, 1);
//	 printf("%d", *pin);
//	 
//	 
// while(1)
//	{
//	  times++;
//	  if(times%50000==0)
//	  {
//		LED1=!LED1;
//		 delay_ms(300);	  
//	  }
//	}
//	 
// }

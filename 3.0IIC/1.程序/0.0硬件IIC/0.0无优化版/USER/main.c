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
	
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
    I2C_EEPROM_config(); //i2c��ʼ��

////дһ���ֽڶ�	
//    printf("IIC��1���ֽ�����ʵ��\r\n");
//	EEPROM_Byte_Write(0x09, 0x55);  //дһ���ֽ�  �洢�ֵ�ַ �洢��ֵ	
//	EEPROM_WaitForWriteEnd(); //���д����
//    EEPROM_Read(0x11, ReadData, 1);
//	//��ʾ
//	printf("���յ�������:0x%x\r\n", ReadData[0]);
//	
//	printf("\r\n");

//	
////����д8���ֽڶ�	
//	printf("IIC��8���ֽ�����ʵ��\r\n");
//	printf("���յ�������:");
//	EEPROM_8Byte_Write(0x08, WriteData, 8);
//	EEPROM_WaitForWriteEnd(); //���д����
//    EEPROM_Read(0x08, ReadData, 8);
//	for(i=0; i<8; i++)
//	{
//		printf("0x%x  ", ReadData[i]);
//	}
//    printf("\r\n\r\n\r\n");
	
	
//��ӡ����
printf("EEPROMд��ʵ��\r\n ");
	for(i=0; i<256; i++)
	{
		printf("0x%02x ", WriteData[i]);
		if(i%16==15)//��Ϊ�Ǵ�0��ʼ ��15�����س�  ��31�����س� ��47��63��79��95��ӡ�س�
		{           //�����i%15==0�Ļ� ��0��15��30��45��ӡ�س� ��ÿ15�����س�
			        //�����i%16==0�Ļ� ��0��16��32��48��ӡ�س� ��Ȼÿ16�����س�  ��һ��ʼ��0�����ͻس� �����Բ���
			printf("\r\n ");
		}
			
	}
//д��	
	for(i=0;i<255;i++)
	{
		//EEPROM_8Byte_Write(0x00+i, WriteData, 8);
		EEPROM_Byte_Write(0x00+i, 255-i);
		EEPROM_WaitForWriteEnd(); //���д����
		}
//	}

	
    printf("---------------------�ָ���----------\r\n ");
	EEPROM_WaitForWriteEnd(); //���д����
	

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


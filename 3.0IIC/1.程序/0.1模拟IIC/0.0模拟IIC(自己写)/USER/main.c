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

/**��ʼ���ز�����**/
	 delay_init();
	 uart_init(115200);
	 LED_Init();
	 IIC_GPIO_Config();

/**��ʼ���ز�����**/
	 delay_init();
	 uart_init(115200);
	 LED_Init();
	 IIC_GPIO_Config();

	 
/**************** ************* ************* ********/
/**************** һ����д��************* ********/
/**************** ************* ************* ********/
	 //���鸳ֵ	 
	 for(i=0; i<256; i++)
	 {
		 WriteData[i]=i;
	 }


//�ж�eepromд�����Ƿ�ɹ� 
//	ee_WriteBytes(0X00, WriteData,256)
//	 YHee_WriteBytes(WriteData, 0x00, 256)
	 if( ee_WriteBytes(0x00, WriteData,256) ==1)
	 {
	    printf("\r\nģ��IICд���ݳɹ� ����Ϊ��\r\n");
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
		 printf("\r\nģ��IICд����ʧ��\r\n");
	 }
	 
	 
	 ee_Delay(0x0FFFFF); //��ʱ �ٶ����� ��������


	 //�ж϶������Ƿ�ɹ�
	 if( ee_ReadBytes(0x00, ReadData, 256)==1)
	 {
		 printf("\r\nģ��IIC����������Ϊ��\r\n");
		 //size��255 ��Ϊ��0��ʼ�� ��255һ��256��
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
		 printf("\r\nģ��IIC������ʧ��\r\n");
	}
	
	
	
/**************** ************* ************* ********/		 
/************* EEPROM�洢С�� dobule******************/	
/**************** ************* ************* ********/	
    ee_Delay(0x0FFFFF); 
	for(i=0; i<8; i++)
	{
		WriteFloat[i]=i*10+i*0.1;
	}
	ee_WriteBytes(0x00, (void*)WriteFloat,sizeof(WriteFloat));
	ee_Delay(0x0FFFFF); 
	ee_ReadBytes(0x00, (void*)ReadFloat, sizeof(ReadFloat));
	            //(void*)ReadFloat WriteFloatһ��Ԫ����8byte��(void*)��ָ�뽫WriteFloat���ݸ�����ʱ����Ԫ���Զ��ָ��һ���ֽ�һ���ֽ�������
	            //��10.1������λ1010.00011001100110011
	            //���ڴ��д洢��ֵΪ0 10000010 01000110011001100110011 �ܹ�8���ֽڣ��ִ����˵�ַ0x00~0x03��  ��������ֱ�ÿ�ֽڶ�ȡȻ���ϴ���ReadFloat��
	printf("\r\n\r\nEEPROM�����ĸ�����Ϊ��\r\n");
	for(i=0; i<8; i++)
	{
		printf("%LF\r\n", ReadFloat[i]);
	}
	
	
/**************** ************* ************* ********/
/************* EEPROM�洢�ַ��� char******************/	 
/**************** ************* ************* ********/
	ee_Delay(0x0FFFFF); 
	ee_WriteBytes(0x60, (void*)WriteChar,sizeof(WriteChar));
	ee_Delay(0x0FFFFF); 
	ee_ReadBytes(0x60, (void*)ReadChar, sizeof(ReadChar));
	            //(void*))WriteChar  (void*)��ָ�뽫)WriteChar���ݸ�����ʱ����Ԫ���Զ��ָ��һ���ֽ�һ���ֽ�������
	      
	printf("\r\n\r\nEEPROM�������ַ���Ϊ��\r\n");
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
///**��ʼ���ز�����**/
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

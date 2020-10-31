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
	
	//*number=45;//     //ָ���ܷɵ�ԭ����������ָ�룬��Ȼ��ֵ��������ָ������֪��ָ�����ָ����
	number=&ReadData[1];//�������ȷ��ָ�룬ָ��ָ�ķ����Ǿ���ģ���ջ���ٵ�һ���ڴ�
	
   /**��ʼ������**/	
	delay_init();
	uart_init(115200);
	LED_Init();
	I2C_configation();
	
	for(i=0;i<256;i++)
	{
		WriteData[i]=(255-i);
	}
	
	
////дһ�ֽڲ���	
//	EEPROM_Byte_Write(0, number);
//	EEPROM_WaitForWriteEnd();
//	EEPROM_Read(0,ReadData,1);
//	printf("%d\r\n",ReadData[0]);
	
	

	for(j=0; j<5;j++)
	{
	     printf("\r\n0x%02x", 0x00+j);
		 EEPROM_Page_Write(0x00+j, WriteData, 6);  //�������д���ݣ�����8byte�ͻ�������� Ӧ����Ӳ��IIC��BUG
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
//		 Buffer_Write(0x00+j, WriteData, 6);  //�������д���ݣ�����8byte�ͻ�������� Ӧ����Ӳ��IIC��BUG
//		 EEPROM_Read(0x00+j, ReadData, 6);
//		 printf("\r\n");
//		 for(i=0;i<20;i++)
//		 {
//			printf("%d ",ReadData[i]);
//		 }
//    }
/****************************���Ժ궨��� printf����******************************************************************/	
	//EEPROM_DEBUG("666666=%d",6);
	//EEPROM__ERROR("�������Ϊ%d",i);
    // TXET("666666=%d",6);  //printf�궨��Ĳ��Ժ���  
	
	
	
	
	
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


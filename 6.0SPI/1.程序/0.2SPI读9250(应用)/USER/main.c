#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "SPIFlash.h"

#define   countsize(a)     ( sizeof(a)/sizeof(*a) )  //�����ַ������鳤��
#define   char_long        (countsize(WriteData_char) - 1) //���鳤�Ǵ�0��ʼ


 int main(void)
 {	
	 u8 i;

	 u32 spi_id, spi_id2;
	 
	 SPI_Commit_Init();
	 delay_init();
	 uart_init(115200);
	 LED_Init();
	 Init_MPU9250();      
     printf("\r\n ������NoMax(�޼�)���� \r\n ����Ҳο�\r\n ����һ��SPIFLASHʵ�� SPI2--W25Q128\r\n ");

	 
	 
      
	 while(1)
	 {
		  i++;	
		 //FLASH_DEBUG("���� %d", 7);
/************************* ��9250���豸��ַ *************************/  
		 spi_id=SPI_FLASH_Read_ID();
         printf("\r\n MPU9250_ID: 0x%X", spi_id);
/************************* AK8963�����Ƶ��豸��ַ *************************/  

		 

///***************  д���� **************************************/
//		Write_More_256Byte_YeHuo(0x000015, WriteData,500); //д��>256�ֽ�

//		printf("\r\n Flash��ȡ������Ϊ\r\n"); 
//		for(j=0; j<500; j++) //��ӡ��Flash����ֵ
//		{
//		}
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		  delay_ms(500);
		  LED1=!LED1;//��ʾ������������
		  if(i % 10 == 0)
		  {
			  i = 0;
			   printf("\r\n ������NoMax(�޼�)���� \r\n ����Ҳο�\r\n ����һ��SPIFLASHʵ�� SPI2--W25Q128\r\n ");

		  }
	  

	 }	 
 }


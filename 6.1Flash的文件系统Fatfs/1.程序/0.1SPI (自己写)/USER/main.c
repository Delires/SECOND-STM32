#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "SPIFlash.h"

#define   countsize(a)     ( sizeof(a)/sizeof(*a) )  //�����ַ������鳤��
#define   char_long        (countsize(WriteData_char) - 1) //���鳤�Ǵ�0��ʼ

u8 WriteData_char[]="���ͣ� ������������";
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
	 
     printf("\r\n ������NoMax(�޼�)���� \r\n ����Ҳο�\r\n ����һ��SPIFLASHʵ�� SPI2--W25Q128\r\n ");

	 Wait_Erase_Write_End(); //�ȴ��ϴβ������
     Flash_Sector_Erase(0);
            
		  //FLASH_DEBUG("���� %d", 7);
/************************* ����豸��ַ*************************/  
		 spi_id = SPI_FLASH_Read_ID();
		 spi_id2=SPI_FLASH_SHORT_ID();
        printf("\r\n JEDEC_ID: 0x%X", spi_id);
		 printf("\r\n Device_ID: 0x%X\r\n", spi_id2);

/***************  д���� **************************************/
//		/** ����Flash **/
//		Flash_Sector_Erase( 0x000000 );

//		/** дFlash **/
//		printf("\r\n\r\n\r\n\r\n\r\n\r\n"); 
//		for(j=0; j<500; j++)//��ֵ
//		{
//			l=j;
//			if(l>255)
//			{
//				l/=2;
//			}
//			WriteData[j]=l;
//			
//		}
//		printf("\r\n ��Flash�洢�ĵ�����Ϊ\r\n"); 
//		for(j=0; j<500; j++) //��ӡ��Flashд��ֵ
//		{
//			printf("0x%04X ", WriteData[j]); 
//			if(j%10 == 9)
//			{
//				printf("\r\n");
//			}
//		}
//		//Write_Less256byte_Flash(0x000000, WriteData,200); //д��<256�ֽ�
//		Write_More_256Byte_YeHuo(0x000302, WriteData,500); //д��>256�ֽ�
//		/*** ��ȡFLSH ***/ 
//		Read_Sector_Flash(0x000302, ReadData, 500);   //1k = 1024byte (����λ)
//		printf("\r\n Flash��ȡ������Ϊ\r\n"); 
//		for(j=0; j<500; j++) //��ӡ��Flash����ֵ
//		{
//			printf("0x%04X ", ReadData[j]); 
//			if(j%10 == 9)
//			{
//				printf("\r\n");
//			}
//		}

//		
///***************  д�ַ��� **************************************/
//        /** ����Flash **/
//		Flash_Sector_Erase( 0x000302 );
//		printf("----------------��Flashд���ַ���------------\r\n"); 
//		/** дFlash **/
//		printf("\r\n\r\n\r\n\r\n\r\n\r\n"); 
//		printf("\r\n ��Flash�洢�ĵ�����Ϊ:\r\n%s\r\n", WriteData_char );  //��ӡ�ַ��� ֱ����������
//     	Write_Less256byte_Flash(0x000302, WriteData_char, char_long);

//		/*** ��ȡFLSH ***/ 
//		Read_Sector_Flash(0x000302, ReadData_char, char_long);   //1k = 1024byte (����λ)
//		printf("\r\n Flash��ȡ������Ϊ:\r\n%s\r\n", ReadData_char); 

//		
///***************  дС�� **************************************/
//		/** ����Flash **/
//	
//		Flash_Sector_Erase( 0x000302 );
//        printf("----------------��Flashд���ַ���------------\r\n"); 
//		/** дFlash **/
//		for(j=0; j<20; j++)//��ֵ
//		{
//			Float_WriteData[j]=j+0.1*j;
//		}
//		printf("\r\n ��Flash�洢��С��������Ϊ\r\n"); 
//		for(j=0; j<20; j++) //��ӡ��Flashд��ֵ
//		{
//			printf("%f \r\n", Float_WriteData[j]); 
//		}
//		Write_More_256Byte_YeHuo(0x000302, (void*)Float_WriteData, sizeof(Float_WriteData)); //д��>256�ֽ�
//		//Write_Less256byte_Flash(0x000000, (void*)Float_WriteData, 20);   //ע��float�����ݵĴ�С 4byte ���дһ��С��Ҫ�����Ĵ�
//		/** ��Flash **/
        Read_Sector_Flash(0x000400, (void*)Float_ReadData, sizeof(Float_WriteData));
		printf("\r\n ��ȡ��С��������Ϊ\r\n");
		for(j=0; j<20; j++) //��ӡ��Flashд��ֵ
		{
			printf("%c \r\n", Float_ReadData[j]); 
		}

		
/************************ �������� ************************/	
	 while(1)
	 {
//		   i++;	
//		  delay_ms(500);
//		  LED1=!LED1;//��ʾ������������
//		  if(i % 10 == 0)
//		  {
//			  i = 0;
//			   printf("\r\n ������NoMax(�޼�)���� \r\n ����Ҳο�\r\n ����һ��SPIFLASHʵ�� SPI2--W25Q128\r\n ");

//		  }
//	  

	 }	 
 }


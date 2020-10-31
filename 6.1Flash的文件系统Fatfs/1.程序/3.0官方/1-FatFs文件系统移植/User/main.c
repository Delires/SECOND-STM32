/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SPI FLASH�ļ�ϵͳ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F103-�Ե� ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "ff.h"
#include "./flash/bsp_spi_flash.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"


//FATFS fs;													/* FatFs�ļ�ϵͳ���� */
//FIL fnew;													/* �ļ����� */
//FRESULT res_flash;                /* �ļ�������� */
//UINT fnum;            					  /* �ļ��ɹ���д���� */
//BYTE ReadBuffer[1024]={0};        /* �������� */
//BYTE WriteBuffer[] =              /* д������*/
//"��ӭʹ��Ұ��STM32������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n"; 

//int main(void)
//{
//	/* ��ʼ��LED */
//	LED_GPIO_Config();	
//	LED_BLUE;
//	
//	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
//	USART_Config();	
//  printf("****** ����һ��SPI FLASH �ļ�ϵͳʵ�� ******\r\n");
//  
//	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
//	//��ʼ������������������
//	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
//	res_flash = f_mount(&fs,"1:",1);
//	
///*----------------------- ��ʽ������ -----------------*/  
//	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
//	if(res_flash == FR_NO_FILESYSTEM)
//	{
//		printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\r\n");
//    /* ��ʽ�� */
//		res_flash=f_mkfs("1:",0,0);							
//		
//		if(res_flash == FR_OK)
//		{
//			printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ��\r\n");
//      /* ��ʽ������ȡ������ */
//			res_flash = f_mount(NULL,"1:",1);			
//      /* ���¹���	*/			
//			res_flash = f_mount(&fs,"1:",1);
//		}
//		else
//		{
//			LED_RED;
//			printf("������ʽ��ʧ�ܡ�����\r\n");
//			while(1);
//		}
//	}
//  else if(res_flash!=FR_OK)
//  {
//    printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
//    printf("��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");
//		while(1);
//  }
//  else
//  {
//    printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
//  }
//  
///*----------------------- �ļ�ϵͳ���ԣ�д���� -------------------*/
//	/* ���ļ���ÿ�ζ����½�����ʽ�򿪣�����Ϊ��д */
//	printf("\r\n****** ���������ļ�д�����... ******\r\n");	
//	res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",FA_CREATE_ALWAYS | FA_WRITE );
//	if ( res_flash == FR_OK )
//	{
//		printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
//    /* ��ָ���洢������д�뵽�ļ��� */
//		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
//    if(res_flash==FR_OK)
//    {
//      printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
//      printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
//    }
//    else
//    {
//      printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_flash);
//    }    
//		/* ���ٶ�д���ر��ļ� */
//    f_close(&fnew);
//	}
//	else
//	{	
//		LED_RED;
//		printf("������/�����ļ�ʧ�ܡ�\r\n");
//	}
//	
///*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
//	printf("****** ���������ļ���ȡ����... ******\r\n");
//	res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",FA_OPEN_EXISTING | FA_READ); 	 
//	if(res_flash == FR_OK)
//	{
//		LED_GREEN;
//		printf("�����ļ��ɹ���\r\n");
//		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
//    if(res_flash==FR_OK)
//    {
//      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
//      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
//    }
//    else
//    {
//      printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_flash);
//    }		
//	}
//	else
//	{
//		LED_RED;
//		printf("�������ļ�ʧ�ܡ�\r\n");
//	}
//	/* ���ٶ�д���ر��ļ� */
//	f_close(&fnew);	
//  
//	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
//	f_mount(NULL,"1:",1);
//  
//  /* ������ɣ�ͣ�� */
//	while(1)
//	{
//	}
//}

///*********************************************END OF FILE**********************/















FATFS FlashfsObject;  //FLASH--FAFTS�ļ�ϵͳ�ṹ�� ����Ϊȫ�� ���嵽���� ��ֹ���
//FATFS FlashfsObject;  //ÿ��һ���洢���ʣ���Ҫ����һ�����ڵ�FAFTS�ļ�ϵͳ�ṹ��
FIL fp; //����һ���ļ����

u8 WriteData_char[]="���ͣ� ������������";
u8 ReadData_char[80]="";


BYTE Fatfs_WriteData_char[]="Fatfs�ļ�ϵͳд ���� ����������!";
BYTE Fatfs_ReadData_char[800]="";
//����Ҫ�ó���const����
UINT bw;
UINT br;





 int main(void)
 {	
	 FRESULT res; //�����ļ�ϵͳ�������ص�״ֵ̬
	 
     USART_Config();
	 LED_GPIO_Config();	
	 LED_BLUE;
	
	 
	 
	 
	 
	 
	 
	 
	 
 
//�滻���� �ⲿ��	 
	 
/***************  FATFS�ļ�ϵͳ���� **************************************/
	 
/*�洢���ʹ����ļ�ϵͳ*/	
	  res=f_mount( &FlashfsObject, "1:", 1);  //������ ��number����������ʽ����洢���ʱ�š��硰1���� ��2����  Ϊɶ��Ҳ��֪�����ǹ涨
	  printf("\r\n fmount_res=%d \r\n", res);
	 
/*��ʽ������*/	 
	 
	  if(res == FR_NO_FILESYSTEM) //���û�ļ�ϵͳ �͸�ʽ�������¹���
	  {
		res = f_mkfs("1:", 0 ,0);
		printf("\r\n f_mkfs_res=%d \r\n", res);
		  
		//��ʽ����Ҫȡ������ �����¹���
		f_mount( NULL, "1:", 1);  //ȡ������
		res = f_mount( &FlashfsObject, "1:", 1);//���¹���	
        printf("\r\n f_mountsecond_res=%d \r\n", res);		  
		  
	  }
	  
	  
	  /*************** дһ���ļ� ��open-write-close **************************************/ 	  
/*��һ���ļ�*/
	  //�ļ��������ȴ��ļ���þ�������ܽ��к����Ĳ���
	  //res = f_open(&fp, "1:mengdi.txt",    FA_OPEN_EXISTING|FA_CREATE_ALWAYS|FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
       res = f_open(&fp, "1:cpx.txt",    FA_CREATE_ALWAYS | FA_WRITE);                             //��/д/����Ȩ�޶�����	  
	  printf("\r\n f_open_res=%d \r\n", res);	
/*д��һ���ļ�*/
	  //ִ����������ǰ�����ȴ�һ���ļ� ��þ����Ȩ��
	  if(res == FR_OK)
	  { 
		  res = f_write(&fp, Fatfs_WriteData_char,sizeof(Fatfs_WriteData_char), &bw);
		  
          if(res == FR_OK)
		  {
            printf("\r\n f_write_res=%d д���ֽ�����bw=%d \r\n", res, bw);
			 
		  }			  
 	  }
	  else
	  {
		  printf("\r\n д��ʧ�� \r\n");
	  }
      res = f_close(&fp);
	  printf("\r\n �ر��ļ�  f_close=%d  \r\n", res);

	  
     /*************** ��ȡһ���ļ�  ��open-read-close**************************************/ 	  
/*��ȡһ���ļ�*//*��ȡһ���ļ�*/
	  
	  res = f_open(&fp, "1:cpx.txt" ,FA_OPEN_EXISTING | FA_READ); 
		if(res == FR_OK)
		{
			printf("\r\n �ڶ��δ��ļ��ɹ� f_open_res=%d \r\n", res);
             res=f_read(&fp, Fatfs_ReadData_char, f_size(&fp), &br);
			 printf("\r\n f_read_res=%d  ��ȡ�ֽ�����br=%d \r\n", res, br);
			 if(res == FR_OK)
			 {
				 printf("\r\n �ļ�ֵΪ��%s \r\n", Fatfs_ReadData_char);
			 }
			 else
			 {
				 printf("\r\n �����󣬴���ֵf_write_res=%d \r\n", res);
			 }
		 }
		else
		{
			printf("\r\n �ڶ��δ��ļ�ʧ�� f_open_res=%d \r\n", res);
		}
		 f_close(&fp);
			  
	 while(1)
	 {
		 
	 }
	  

}	 


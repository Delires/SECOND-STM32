/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SPI FLASH文件系统例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F103-霸道 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "ff.h"
#include "./flash/bsp_spi_flash.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"


//FATFS fs;													/* FatFs文件系统对象 */
//FIL fnew;													/* 文件对象 */
//FRESULT res_flash;                /* 文件操作结果 */
//UINT fnum;            					  /* 文件成功读写数量 */
//BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
//BYTE WriteBuffer[] =              /* 写缓冲区*/
//"欢迎使用野火STM32开发板 今天是个好日子，新建文件系统测试文件\r\n"; 

//int main(void)
//{
//	/* 初始化LED */
//	LED_GPIO_Config();	
//	LED_BLUE;
//	
//	/* 初始化调试串口，一般为串口1 */
//	USART_Config();	
//  printf("****** 这是一个SPI FLASH 文件系统实验 ******\r\n");
//  
//	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
//	//初始化函数调用流程如下
//	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
//	res_flash = f_mount(&fs,"1:",1);
//	
///*----------------------- 格式化测试 -----------------*/  
//	/* 如果没有文件系统就格式化创建创建文件系统 */
//	if(res_flash == FR_NO_FILESYSTEM)
//	{
//		printf("》FLASH还没有文件系统，即将进行格式化...\r\n");
//    /* 格式化 */
//		res_flash=f_mkfs("1:",0,0);							
//		
//		if(res_flash == FR_OK)
//		{
//			printf("》FLASH已成功格式化文件系统。\r\n");
//      /* 格式化后，先取消挂载 */
//			res_flash = f_mount(NULL,"1:",1);			
//      /* 重新挂载	*/			
//			res_flash = f_mount(&fs,"1:",1);
//		}
//		else
//		{
//			LED_RED;
//			printf("《《格式化失败。》》\r\n");
//			while(1);
//		}
//	}
//  else if(res_flash!=FR_OK)
//  {
//    printf("！！外部Flash挂载文件系统失败。(%d)\r\n",res_flash);
//    printf("！！可能原因：SPI Flash初始化不成功。\r\n");
//		while(1);
//  }
//  else
//  {
//    printf("》文件系统挂载成功，可以进行读写测试\r\n");
//  }
//  
///*----------------------- 文件系统测试：写测试 -------------------*/
//	/* 打开文件，每次都以新建的形式打开，属性为可写 */
//	printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
//	res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",FA_CREATE_ALWAYS | FA_WRITE );
//	if ( res_flash == FR_OK )
//	{
//		printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
//    /* 将指定存储区内容写入到文件内 */
//		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
//    if(res_flash==FR_OK)
//    {
//      printf("》文件写入成功，写入字节数据：%d\n",fnum);
//      printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
//    }
//    else
//    {
//      printf("！！文件写入失败：(%d)\n",res_flash);
//    }    
//		/* 不再读写，关闭文件 */
//    f_close(&fnew);
//	}
//	else
//	{	
//		LED_RED;
//		printf("！！打开/创建文件失败。\r\n");
//	}
//	
///*------------------- 文件系统测试：读测试 --------------------------*/
//	printf("****** 即将进行文件读取测试... ******\r\n");
//	res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",FA_OPEN_EXISTING | FA_READ); 	 
//	if(res_flash == FR_OK)
//	{
//		LED_GREEN;
//		printf("》打开文件成功。\r\n");
//		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
//    if(res_flash==FR_OK)
//    {
//      printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
//      printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
//    }
//    else
//    {
//      printf("！！文件读取失败：(%d)\n",res_flash);
//    }		
//	}
//	else
//	{
//		LED_RED;
//		printf("！！打开文件失败。\r\n");
//	}
//	/* 不再读写，关闭文件 */
//	f_close(&fnew);	
//  
//	/* 不再使用文件系统，取消挂载文件系统 */
//	f_mount(NULL,"1:",1);
//  
//  /* 操作完成，停机 */
//	while(1)
//	{
//	}
//}

///*********************************************END OF FILE**********************/















FATFS FlashfsObject;  //FLASH--FAFTS文件系统结构体 定义为全局 定义到堆里 防止溢出
//FATFS FlashfsObject;  //每有一个存储介质，就要定义一个挂在的FAFTS文件系统结构体
FIL fp; //建立一个文件句柄

u8 WriteData_char[]="加油！ 奥利给！！！";
u8 ReadData_char[80]="";


BYTE Fatfs_WriteData_char[]="Fatfs文件系统写 加油 奥利给！！!";
BYTE Fatfs_ReadData_char[800]="";
//数据要用常量const？？
UINT bw;
UINT br;





 int main(void)
 {	
	 FRESULT res; //定义文件系统操作返回的状态值
	 
     USART_Config();
	 LED_GPIO_Config();	
	 LED_BLUE;
	
	 
	 
	 
	 
	 
	 
	 
	 
 
//替换下文 这部分	 
	 
/***************  FATFS文件系统操作 **************************************/
	 
/*存储介质挂载文件系统*/	
	  res=f_mount( &FlashfsObject, "1:", 1);  //必须以 “number：”这种形式输入存储介质标号。如“1：” “2：”  为啥我也不知道就是规定
	  printf("\r\n fmount_res=%d \r\n", res);
	 
/*格式化磁盘*/	 
	 
	  if(res == FR_NO_FILESYSTEM) //如果没文件系统 就格式化在重新挂载
	  {
		res = f_mkfs("1:", 0 ,0);
		printf("\r\n f_mkfs_res=%d \r\n", res);
		  
		//格式化后要取消挂载 再重新挂载
		f_mount( NULL, "1:", 1);  //取消挂载
		res = f_mount( &FlashfsObject, "1:", 1);//重新挂载	
        printf("\r\n f_mountsecond_res=%d \r\n", res);		  
		  
	  }
	  
	  
	  /*************** 写一个文件 先open-write-close **************************************/ 	  
/*打开一个文件*/
	  //文件操作，先打开文件获得句柄，才能进行后来的操作
	  //res = f_open(&fp, "1:mengdi.txt",    FA_OPEN_EXISTING|FA_CREATE_ALWAYS|FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
       res = f_open(&fp, "1:cpx.txt",    FA_CREATE_ALWAYS | FA_WRITE);                             //打开/写/读的权限都给上	  
	  printf("\r\n f_open_res=%d \r\n", res);	
/*写入一个文件*/
	  //执行其他操作前必须先打开一个文件 获得句柄和权限
	  if(res == FR_OK)
	  { 
		  res = f_write(&fp, Fatfs_WriteData_char,sizeof(Fatfs_WriteData_char), &bw);
		  
          if(res == FR_OK)
		  {
            printf("\r\n f_write_res=%d 写入字节数：bw=%d \r\n", res, bw);
			 
		  }			  
 	  }
	  else
	  {
		  printf("\r\n 写入失败 \r\n");
	  }
      res = f_close(&fp);
	  printf("\r\n 关闭文件  f_close=%d  \r\n", res);

	  
     /*************** 读取一个文件  再open-read-close**************************************/ 	  
/*读取一个文件*//*读取一个文件*/
	  
	  res = f_open(&fp, "1:cpx.txt" ,FA_OPEN_EXISTING | FA_READ); 
		if(res == FR_OK)
		{
			printf("\r\n 第二次打开文件成功 f_open_res=%d \r\n", res);
             res=f_read(&fp, Fatfs_ReadData_char, f_size(&fp), &br);
			 printf("\r\n f_read_res=%d  读取字节数：br=%d \r\n", res, br);
			 if(res == FR_OK)
			 {
				 printf("\r\n 文件值为：%s \r\n", Fatfs_ReadData_char);
			 }
			 else
			 {
				 printf("\r\n 读错误，错误值f_write_res=%d \r\n", res);
			 }
		 }
		else
		{
			printf("\r\n 第二次打开文件失败 f_open_res=%d \r\n", res);
		}
		 f_close(&fp);
			  
	 while(1)
	 {
		 
	 }
	  

}	 


#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "SPIFlash.h"
#include "string.h"

/* FATFSͷ�ļ����� */
#include "integer.h"
#include "ff.h"
#include "diskio.h"
#include "ffconf.h"
#include "fatfuction.h"

/**
blog
20200818��
      1.�Ʋ�Ӧ����Ӳ�����⣬Ӧ����д����������   ----  �������Ǹ�����û����ֵ
20200819��
      1.f_open�Ϸ���10 д���� Ϊʲô��  �²⻹�ǵײ�ԭ��   ��ֲ��Ұ��ĺ���Ҳ��������
	     ��Ϊдdiskio����ʱ����ʼ��DSTATUS statû�и���ֵ����˵��Ի������ֵ���·���ֵ����
         �мǶ������������һ����ʼֵ����������ѧ����
		 
20200824��1.flash��U�̴�û���½��ļ��У���Ұ��ȴ�½��ļ�����  Ϊʲô
             ���ǻ��ɵײ�������
          2.��Ұ�����ֲ�ҳ����ļ������� ��û������
		     ����������Ұ�� ��open-write-close   ��open-read-close����˳��д����ֵ��
		  3.�ҵ������������ֵڶ��δ�ʧ�ܡ�Ұ��ĺ����ڶ���ȴ�ܴ򿪳ɹ���
		     ��д�ļ�-----���ٶ�   ʧ��
			 Ұ��д�ļ�----Ұ���  �ɹ�
			 Ұ��д----�Ҷ�        �ɹ�          �ҵ�д�ļ��������⣡����diskioд/SPIд�����⣿
			 ����SPIд���������⣬��diskioд�ӿڵ����� 
			 
20200923: FLASH��ͷû��ϵͳ�ļ������԰�sector+770ע�͵�������Ҫƫ�Ƶ�ַ����
          ����scan_files()��������ɨ�赽
"1:"
		  
		  
**/
u8 WriteData_char[]="SPI��ͨд������ʵ����";  //abcasdasdasdasd
u8 ReadData_char[80];
char path[100];  

FATFS FlashfsObject;  //FLASH--FAFTS�ļ�ϵͳ�ṹ�� ����Ϊȫ�� ���嵽���� ��ֹ���
//FATFS FlashfsObject;  //ÿ��һ���洢���ʣ���Ҫ����һ�����ڵ�FAFTS�ļ�ϵͳ�ṹ��
FIL fp; //����һ���ļ����
//����FATFS�ļ�ϵͳд�ļ��Ļ�  ��ش洢���ݵı����ͱ���Ҫ��FATFS�ļ�ϵͳ�е�"��������"�������   
//���������FATFS�ļ�ϵͳ�е�"��������"����������ͻῨ���ˡ������ջ�Ĵ�С��ƥ��Ϳ�����
//FATFS�ļ�ϵͳ�ĺ����ǰ����ļ�ϵͳ�е�"��������"���������������������ı��������ƥ�䣬��ô���������ı�����ջ��С �� �Լ����������ջ��С��ƥ�䣬���ԾͿ����ˣ��ͻ����ջ���
BYTE  Fatfs_WriteData_char[]="FATFS�����ļ�д�����ݣ����ͣ� ������������";
BYTE  Fatfs_ReadData_char[800]="";    //����Ҫ��FATFS�ļ�ϵͳ�е�"��������"�������   

UINT bw;
UINT br;


 int main(void)
 {	
	 u8 i;
	 FRESULT res; //�����ļ�ϵͳ�������ص�״ֵ̬
	 
  
	 
	 delay_init();
	 uart_init(115200);
	 LED_Init();

    //PROGRAM_PRINTF("\r\nFATFS�ڴ�ֵΪ: %d\r\n", sizeof(FATFS) );  //��ӡ�ַ��� ֱ����������
	//drive_number = &j;
	// PROGRAM_PRINTF("\r\n%d   %d\r\n", (&drive_number),    (*drive_number)); 
	
	 


	 
/***************  ����FATFS�ļ�ϵͳ **************************************/
/*�洢���ʹ����ļ�ϵͳ*/	
	  res=f_mount( &FlashfsObject, "1:", 1);  //������ ��number����������ʽ����洢���ʱ�š��硰1���� ��2����  Ϊɶ��Ҳ��֪�����ǹ涨
	  PROGRAM_PRINTF("\r\n fmount_res=%d \r\n", res);
	 
/*��ʽ������ ���¼���FATFS   ���ֿ��ļ��޹�*/	 
	  if(res == FR_NO_FILESYSTEM) //���û�ļ�ϵͳ �͸�ʽ�������¹���
	  {
		res = f_mkfs("1:", 0 ,0);
		PROGRAM_PRINTF("\r\n f_mkfs_res=%d \r\n", res);
		  
		//��ʽ����Ҫȡ������ �����¹���
		f_mount( NULL, "1:", 1);  //ȡ������
		res = f_mount( &FlashfsObject, "1:", 1);//���¹���	
        PROGRAM_PRINTF("\r\n f_mountsecond_res=%d \r\n", res);		  
		  
	  }

	  
/***************  FLASH�Ļ�����Ϣ��ȡ **************************************/	 
		miscellaneous();
	  
/***************  ��ȡFLASH�ļ�����Ϣ **************************************/	 	  
	    file_check();
/***************  ɨ��洢���������ļ� **************************************/	  
		printf("-----------------------------------------------------------------------------------------------\r\n");
		printf("-------------------- scan_files����Ч�� ----------------------------------------------------\r\n");
		printf("-----------------------------------------------------------------------------------------------\r\n");
		strcpy(path,"1:"); //strcpy�����ַ������Ѵ洢�豸1: �����ָ��Ƹ�fpath
	             //����Ҫʹ�����ֲ���������ֱ������"1:"����������
		scan_files(path);  //scan_files Ӧ�ò㺯�����Լ�д��   ɨ��fpath��ɨ��1:

	  
//	  
///*************** ��FATFS�ļ��Ļ������� ���ļ�д����**************************************/	 
//printf("\r\n\r\n*********** ��FATFS�ļ��Ļ������� ���ļ�д���� ***********\r\n");	  
///*��һ���ļ�*/
//	  //�ļ��������ȴ��ļ���þ�������ܽ��к����Ĳ���
//	  res = f_open(&fp, "1:asf.txt",  FA_OPEN_EXISTING|FA_CREATE_ALWAYS|FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
//                                    //��/д/����Ȩ�޶�����	   
///*д��һ���ļ�*/
//	  //ִ����������ǰ�����ȴ�һ���ļ� ��þ����Ȩ��
//		if(res == FR_OK)//�ļ��򿪳ɹ�
//		{ 
//			PROGRAM_PRINTF("\r\n f_open_res=%d \r\n", res);	
//			
//			res = f_write(&fp, Fatfs_WriteData_char,sizeof(Fatfs_WriteData_char), &bw);
//			PROGRAM_PRINTF("\r\n f_write_res=%d д���ֽ�����bw=%d \r\n", res, bw);
//			if(res == FR_OK)//�ļ�д��ɹ�
//			{
//				/*��ȡһ���ļ�*//*��ȡһ���ļ�*/
//				f_lseek(&fp, 0); //�ù��ص��ļ��ʼ;
//				res=f_read(&fp, Fatfs_ReadData_char, f_size(&fp), &br);  //fp�Ķ�����  FIL fp;
//																	  //bw�Ķ�����  UINT bw     ���Ƕ��ǰ����ݶ���ģ������صĲ�����ָ�룬����ҪдΪָ��
//				PROGRAM_PRINTF("\r\n f_read_res=%d  ��ȡ�ֽ�����br=%d \r\n", res, br);
//				if(res == FR_OK)//�ļ���ȡ�ɹ�
//				{
//					PROGRAM_PRINTF("\r\n ��ȡ�ļ�ֵΪ��%s \r\n", Fatfs_ReadData_char);
//				}
//				else
//				{
//					PROGRAM_DEBUG("\r\n ��ȡ�ļ����󣬴���ֵf_write_res=%d \r\n", res);
//				}

//		    }
//			else
//			{
//				PROGRAM_DEBUG("\r\n �ļ�д��ʧ�ܣ�ֵΪf_write_res=%d \r\n", res);
//			}
//		}
//		else
//		{
//		  PROGRAM_DEBUG("\r\n f_open�ļ��򿪴��󣬴���ֵf_open_res=%d \r\n", res);
//		}
///*�ļ��ر�*/	
//		f_close(&fp);  //�ǵ�ִ���ⲽ����������ֵ���洢���ļ���
//		
//		
//		
//		//miscellaneous();
//		
///*ɾ��һ���ļ�*/		
//		res = f_unlink("1:mengdi.txt");//ɾ���ļ�����	  
//		if(res == FR_OK)
//		{
//			PROGRAM_PRINTF("\r\n f_unlink()����ɾ���ļ��ɹ� \r\n");
//		}
//		else
//		{
//			PROGRAM_DEBUG("\r\n ɾ��ʧ�ܣ�����ֵf_unlink_res=%d \r\n", res);
//		}

//	 
//	 
//	 
///***************  д�ַ��� **************************************/
//        /** ����Flash **/
//		Flash_Sector_Erase( 0x000000 );
//		printf("----------------��Flashд���ַ���------------\r\n"); 
//		/** дFlash **/
//		printf("\r\n\r\n\r\n\r\n\r\n\r\n"); 
//		printf("\r\n ��Flash�洢�ĵ�����Ϊ:\r\n%s\r\n", WriteData_char );  //��ӡ�ַ��� ֱ����������
//     	Write_Less256byte_Flash(0x000000, WriteData_char, 80);

//		/*** ��ȡFLSH ***/ 
//		Read_Sector_Flash(0x000000, ReadData_char, 80);   //1k = 1024byte (����λ)
//		printf("\r\n Flash��ȡ������Ϊ:\r\n%s\r\n", ReadData_char); 

//		

/************************ �������� ************************/	
	 while(1)
	 {
		   i++;	
		  delay_ms(500);
		  LED1=!LED1;//��ʾ������������
//		  if(i % 10 == 0)
//		  {
//			  i = 0;
//			   printf("\r\n ������NoMax(�޼�)���� \r\n ����Ҳο�\r\n ����һ�ļ�����ϵͳ SPI2--W25Q128-FAFFS\r\n ");

//		  }
	  

	 }	 
 }

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
// 
//FATFS FlashfsObject;  //FLASH--FAFTS�ļ�ϵͳ�ṹ�� ����Ϊȫ�� ���嵽���� ��ֹ���
////FATFS FlashfsObject;  //ÿ��һ���洢���ʣ���Ҫ����һ�����ڵ�FAFTS�ļ�ϵͳ�ṹ��
//FIL fp; //����һ���ļ����

//u8 WriteData_char[]="���ͣ� ������������";
//u8 ReadData_char[80]="";


//BYTE Fatfs_WriteData_char[]="�ε� �Ұ���";
//BYTE Fatfs_ReadData_char[800]="";
////����Ҫ�ó���const����
//UINT bw;
//UINT br;





// int main(void)
// {	
//	 FRESULT res; //�����ļ�ϵͳ�������ص�״ֵ̬
//	 
//	 delay_init();
//	 uart_init(115200);
//	 LED_Init();
//	
//	 
//	 
//	 
//	 
//	 
//	 
//	 
//	 
////�滻���� �ⲿ��	 
//	 
///***************  FATFS�ļ�ϵͳ���� **************************************/
///*�洢���ʹ����ļ�ϵͳ*/	
//	  res=f_mount( &FlashfsObject, "1:", 1);  //������ ��number����������ʽ����洢���ʱ�š��硰1���� ��2����  Ϊɶ��Ҳ��֪�����ǹ涨
//	  printf("\r\n fmount_res=%d \r\n", res);
//	 
///*��ʽ������*/	 
//	 
//	  if(res == FR_NO_FILESYSTEM) //���û�ļ�ϵͳ �͸�ʽ�������¹���
//	  {
//		res = f_mkfs("1:", 0 ,0);
//		printf("\r\n f_mkfs_res=%d \r\n", res);
//		  
//		//��ʽ����Ҫȡ������ �����¹���
//		f_mount( NULL, "1:", 1);  //ȡ������
//		res = f_mount( &FlashfsObject, "1:", 1);//���¹���	
//        printf("\r\n f_mountsecond_res=%d \r\n", res);		  
//		  
//	  }
//	  
///*��һ���ļ�*/
//	  //�ļ��������ȴ��ļ���þ�������ܽ��к����Ĳ���
//	  //res = f_open(&fp, "1:mengdi.txt",    FA_OPEN_EXISTING|FA_CREATE_ALWAYS|FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
//       res = f_open(&fp, "1:apx.txt",    FA_CREATE_ALWAYS | FA_WRITE);                             //��/д/����Ȩ�޶�����	  
//	  printf("\r\n f_open_res=%d \r\n", res);	
///*д��һ���ļ�*/
//	  //ִ����������ǰ�����ȴ�һ���ļ� ��þ����Ȩ��
//	  if(res == FR_OK)
//	  { 
//		  res = f_write(&fp, Fatfs_WriteData_char,sizeof(Fatfs_WriteData_char), &bw);
//		  
//          if(res == FR_OK)
//		  {
//            printf("\r\n f_write_res=%d д���ֽ�����bw=%d \r\n", res, bw);
//			 
//		  }			  
// 	  }
//	  else
//	  {
//		  printf("\r\n д��ʧ�� \r\n");
//	  }
//      res = f_close(&fp);
//	  printf("\r\n �ر��ļ�  f_close=%d  \r\n", res);

//	  
//	  
///*��ȡһ���ļ�*//*��ȡһ���ļ�*/
//	  
//	  res = f_open(&fp, "1:apx.txt" ,FA_OPEN_EXISTING | FA_READ); 
//		if(res == FR_OK)
//		{
//             res=f_read(&fp, Fatfs_ReadData_char, f_size(&fp), &br);
//			 printf("\r\n f_read_res=%d  ��ȡ�ֽ�����br=%d \r\n", res, br);
//			 if(res == FR_OK)
//			 {
//				 printf("\r\n �ļ�ֵΪ��%s \r\n", Fatfs_ReadData_char);
//			 }
//			 else
//			 {
//				 printf("\r\n �����󣬴���ֵf_write_res=%d \r\n", res);
//			 }
//		 }
//		else
//		{
//			printf("\r\n �ڶ��δ��ļ�ʧ�� f_open_res=%d \r\n", res);
//		}
//		 f_close(&fp);
//			  
//	 while(1)
//	 {
//		 
//	 }
//	
//	  

//}	 


 


















//FATFS fs;													/* FatFs�ļ�ϵͳ���� */
//FIL fnew;													/* �ļ����� */
//FRESULT res_flash;                /* �ļ�������� */
//UINT fnum;            					  /* �ļ��ɹ���д���� */
//char fpath[100];                  /* ���浱ǰɨ��·�� */
//char readbuffer[512];             
// 
///**
//  ******************************************************************************
//  *                                ������
//  ******************************************************************************
//  */
///* FatFs����ܲ��� */
//static FRESULT miscellaneous(void)
//{
//  DIR dir;
//  FATFS *pfs;
//  DWORD fre_clust, fre_sect, tot_sect;
//  
//  printf("\n*************** �豸��Ϣ��ȡ ***************\r\n");
//  /* ��ȡ�豸��Ϣ�Ϳմش�С */
//  res_flash = f_getfree("1:", &fre_clust, &pfs);

//  /* ����õ��ܵ����������Ϳ��������� */
//  tot_sect = (pfs->n_fatent - 2) * pfs->csize;
//  fre_sect = fre_clust * pfs->csize;

//  /* ��ӡ��Ϣ(4096 �ֽ�/����) */
//  printf("���豸�ܿռ䣺%10lu KB��\n�����ÿռ䣺  %10lu KB��\n", tot_sect *4, fre_sect *4);
//  
//  printf("\n******** �ļ���λ�͸�ʽ��д�빦�ܲ��� ********\r\n");
//  res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",
//                            FA_OPEN_ALWAYS|FA_WRITE|FA_READ );
//	if ( res_flash == FR_OK )
//	{
//    /*  �ļ���λ */
//    res_flash = f_lseek(&fnew,f_size(&fnew));
//    if (res_flash == FR_OK)
//    {
//      /* ��ʽ��д�룬������ʽ����printf���� */
//      f_printf(&fnew,"\n��ԭ���ļ������һ������\n");
//      f_printf(&fnew,"���豸�ܿռ䣺%10lu KB��\n�����ÿռ䣺  %10lu KB��\n", tot_sect *4, fre_sect *4);
//      /*  �ļ���λ���ļ���ʼλ�� */
//      res_flash = f_lseek(&fnew,0);
//      /* ��ȡ�ļ��������ݵ������� */
//      res_flash = f_read(&fnew,readbuffer,f_size(&fnew),&fnum);
//      if(res_flash == FR_OK)
//      {
//        printf("���ļ����ݣ�\n%s\n",readbuffer);
//      }
//    }
//    f_close(&fnew);    
//    
//    printf("\n********** Ŀ¼���������������ܲ��� **********\r\n");
//    /* ���Դ�Ŀ¼ */
//    res_flash=f_opendir(&dir,"1:TestDir");
//    if(res_flash!=FR_OK)
//    {
//      /* ��Ŀ¼ʧ�ܣ��ʹ���Ŀ¼ */
//      res_flash=f_mkdir("1:TestDir");
//    }
//    else
//    {
//      /* ���Ŀ¼�Ѿ����ڣ��ر��� */
//      res_flash=f_closedir(&dir);
//      /* ɾ���ļ� */
//      f_unlink("1:TestDir/testdir.txt");
//    }
//    if(res_flash==FR_OK)
//    {
//      /* ���������ƶ��ļ� */
//      res_flash=f_rename("1:FatFs��д�����ļ�.txt","1:TestDir/testdir.txt");      
//    } 
//	}
//  else
//  {
//    printf("!! ���ļ�ʧ�ܣ�%d\n",res_flash);
//    printf("!! ������Ҫ�ٴ����С�FatFs��ֲ���д���ԡ�����\n");
//  }
//  return res_flash;
//}

//  FILINFO fno;
///**
//  * �ļ���Ϣ��ȡ
//  */
//static FRESULT file_check(void)
//{

//  
//  /* ��ȡ�ļ���Ϣ */
//  res_flash=f_stat("1:TestDir/testdir.txt",&fno);
//  if(res_flash==FR_OK)
//  {
//    printf("��testdir.txt���ļ���Ϣ��\n");
//    printf("���ļ���С: %ld(�ֽ�)\n", fno.fsize);
//    printf("��ʱ���: %u/%02u/%02u, %02u:%02u\n",
//           (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,fno.ftime >> 11, fno.ftime >> 5 & 63);
//    printf("������: %c%c%c%c%c\n\n",
//           (fno.fattrib & AM_DIR) ? 'D' : '-',      // ��һ��Ŀ¼
//           (fno.fattrib & AM_RDO) ? 'R' : '-',      // ֻ���ļ�
//           (fno.fattrib & AM_HID) ? 'H' : '-',      // �����ļ�
//           (fno.fattrib & AM_SYS) ? 'S' : '-',      // ϵͳ�ļ�
//           (fno.fattrib & AM_ARC) ? 'A' : '-');     // �����ļ�
//  }
//  return res_flash;
//}

///**
//  * @brief  scan_files �ݹ�ɨ��FatFs�ڵ��ļ�
//  * @param  path:��ʼɨ��·��
//  * @retval result:�ļ�ϵͳ�ķ���ֵ
//  */
//static FRESULT scan_files (char* path) 
//{ 
//  FRESULT res; 		//�����ڵݹ���̱��޸ĵı���������ȫ�ֱ���	
//  FILINFO fno; 
//  DIR dir; 
//  int i;            
//  char *fn;        // �ļ���	
//	
//#if _USE_LFN 
//  /* ���ļ���֧�� */
//  /* ����������Ҫ2���ֽڱ���һ�����֡�*/
//  static char lfn[_MAX_LFN*2 + 1]; 	
//  fno.lfname = lfn; 
//  fno.lfsize = sizeof(lfn); 
//#endif 
//  //��Ŀ¼
//  res = f_opendir(&dir, path); 
//  if (res == FR_OK) 
//	{ 
//    i = strlen(path); 
//    for (;;) 
//		{ 
//      //��ȡĿ¼�µ����ݣ��ٶ����Զ�����һ���ļ�
//      res = f_readdir(&dir, &fno); 								
//      //Ϊ��ʱ��ʾ������Ŀ��ȡ��ϣ�����
//      if (res != FR_OK || fno.fname[0] == 0) break; 	
//#if _USE_LFN 
//      fn = *fno.lfname ? fno.lfname : fno.fname; 
//#else 
//      fn = fno.fname; 
//#endif 
//      //���ʾ��ǰĿ¼������			
//      if (*fn == '.') continue; 	
//      //Ŀ¼���ݹ��ȡ      
//      if (fno.fattrib & AM_DIR)         
//			{ 			
//        //�ϳ�����Ŀ¼��        
//        sprintf(&path[i], "/%s", fn); 		
//        //�ݹ����         
//        res = scan_files(path);	
//        path[i] = 0;         
//        //��ʧ�ܣ�����ѭ��        
//        if (res != FR_OK) 
//					break; 
//      } 
//			else 
//			{ 
//				printf("%s/%s\r\n", path, fn);								//����ļ���	
//        /* ������������ȡ�ض���ʽ���ļ�·�� */        
//      }//else
//    } //for
//  } 
//  return res; 
//}






///**
//  * @brief  ������
//  * @param  ��
//  * @retval ��
//  */
//int main(void)
//{    	
//	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
//	uart_init(115200);
//  printf("******** ����һ��SPI FLASH �ļ�ϵͳʵ�� *******\r\n");
//  
//	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
//	res_flash = f_mount(&fs,"1:",1);
//  if(res_flash!=FR_OK)
//  {
//    printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
//    printf("��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");
//		while(1);
//  }
//  else
//  {
//    printf("���ļ�ϵͳ���سɹ������Խ��в���\r\n");    
//  }
//  
//  /* FatFs����ܲ��� */
//  res_flash = miscellaneous();

//  
//  printf("\n*************** �ļ���Ϣ��ȡ���� **************\r\n");
//  res_flash = file_check();

//  
//  printf("***************** �ļ�ɨ����� ****************\r\n");
//  strcpy(fpath,"1:");
//  scan_files(fpath);
//  
//  
//	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
//	f_mount(NULL,"1:",1);
//  
//  /* ������ɣ�ͣ�� */
//	while(1)
//	{
//	}
//}

///************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

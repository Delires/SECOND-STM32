/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SPI FLASH�ļ�ϵͳ���ù�����ʾ
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
#include "./usart/bsp_usart.h"
#include "ff.h"
#include "string.h"
/**
  ******************************************************************************
  *                              �������
  ******************************************************************************
  */
  
  
/**
��־
20200903���ο�����ĸ���discio��д����ƫ�Ƶ�ַ ������ƫ��  ��  disk_ioctl() FLASH��С�������
                                  ��һ������ д����֮ǰ�Ĳ��� ��������������⣬�������⣿��

**/
FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_flash;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
char fpath[100];                  /* ���浱ǰɨ�������·�� */
char readbuffer[512];             //�������������
 
/**
  ******************************************************************************
  *                                ������
  ******************************************************************************
  */
/* FatFs����ܲ��� */
static FRESULT miscellaneous(void)
{
  DIR dir;
  FATFS *pfs;
  DWORD fre_clust, fre_sect, tot_sect;
  
  printf("\n*************** �豸��Ϣ��ȡ ***************\r\n");
  /* ��ȡ�豸��Ϣ�Ϳմش�С */
  res_flash = f_getfree("1:", &fre_clust, &pfs);
                            //fre_clust ���ؿ���صĸ���
  /* ����õ��ܵ����������Ϳ��������� */
  tot_sect = (pfs->n_fatent - 2) * pfs->csize; //�ܵ������� = �ظ���*ÿ���������� 
	          //pfs->n_fatent ȡ�ṹ���ֵ     pfs->csizeȡ�ṹ���ֵ
  fre_sect = fre_clust * pfs->csize; //����������� = fre_clust ���ؿ���صĸ��� * ÿ����������

  /* ��ӡ��Ϣ(4096 �ֽ�/����) */
  printf("���豸�ܿռ䣺%10lu KB��\n�����ÿռ䣺  %10lu KB��\n", tot_sect *4, fre_sect *4); //һ������4kb ��*4 ���kb
                       //10lu �������Ϊ10�� lu�޷��ų�����
  
	
	
	
  printf("\n******** �ļ���λ�͸�ʽ��д�빦�ܲ��� ********\r\n");
  res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",
                            FA_OPEN_ALWAYS|FA_WRITE|FA_READ );//���ļ�Ȩ��
	if ( res_flash == FR_OK )
	{
    /*  �ļ���λ */
    res_flash = f_lseek(&fnew,f_size(&fnew));//�Ȱѹ��ŵ��ļ���ĩβ��������ʼд�� fnew�ļ��Ĺ��ƫ���ļ��Ĵ�С
    if (res_flash == FR_OK)
    {
      /* f_printf ��ʽ��д�룬������ʽ����printf���� */
	//f_printf�Ĳ����������� f_write ֱ�Ӱ����ݴ�ӡ���ļ���
      f_printf(&fnew,"\n��ԭ���ļ������һ������\n"); //��������f_printf��ӡ���ļ�����
      f_printf(&fnew,"���豸�ܿռ䣺%10lu KB��\n�����ÿռ䣺  %10lu KB��\n", tot_sect *4, fre_sect *4);
      /*  �ļ���λ���ļ���ʼλ�� */
      res_flash = f_lseek(&fnew,0); //���°ѹ��ŵ��ļ���ͷ
      /* ��ȡ�ļ��������ݵ������� */
      res_flash = f_read(&fnew,readbuffer,f_size(&fnew),&fnum); //�������ļ���ֵ��readbuffer������Ҫ�㹻��������ֵ����  
		                    //�ж��ļ���ȡ�Ƿ����  btr != br �����ֽ��������ڷ����ֽ���  ֤���������
      if(res_flash == FR_OK)
      {
        printf("���ļ����ݣ�\n%s\n",readbuffer);
      }
    }
    f_close(&fnew);    //�ر��ļ�

//���ղŴ����� FatFs��д�����ļ�.txt ������	
    printf("\n********** Ŀ¼���������������ܲ��� **********\r\n");
    /* ���Դ�Ŀ¼ */
    res_flash=f_opendir(&dir,"1:TestDir"); //f_opendir��һ���ļ���TestDir
    if(res_flash!=FR_OK) //�ж��Ƿ�򿪳ɹ�
    {
      /* ��Ŀ¼ʧ�ܣ��ʹ���Ŀ¼ */
      res_flash=f_mkdir("1:TestDir"); //f_mkdir ����һ���ļ���(Ŀ¼) TestDir
    }
    else
    {
      /* ���Ŀ¼�Ѿ����ڣ��ر��� */
      res_flash=f_closedir(&dir);
      /* ɾ���ļ� */
      f_unlink("1:TestDir/testdir.txt"); //f_unlink ��TestDir�ļ������testdir.txtɾ����ȷ�����½��в���
    }
    if(res_flash==FR_OK)
    {
      /* ���������ƶ��ļ� */
      res_flash=f_rename("1:FatFs��д�����ļ�.txt","1:TestDir/testdir.txt");   //f_rename �������ļ��� �����ƶ�"��·�����Զ��ƶ���"   
    } 
	}
  else
  {
    printf("!! ���ļ�ʧ�ܣ�%d\n",res_flash);
    printf("!! ������Ҫ�ٴ����С�FatFs��ֲ���д���ԡ�����\n");
  }
  return res_flash;
}






  FILINFO fno;
/**
  * �ļ���Ϣ��ȡ  ����f_stat()����
  */
static FRESULT file_check(void)
{ 
  /* ��ȡ�ļ���Ϣ */
  res_flash=f_stat("1:TestDir/testdir.txt",&fno);//��ȡ�ļ�״̬��״̬��Ϣ���ظ��ṹ��fno
  if(res_flash==FR_OK)
  {
    printf("��testdir.txt���ļ���Ϣ��\n");
    printf("���ļ���С: %ld(�ֽ�)\n", fno.fsize);
    printf("��ʱ���: %u/%02u/%02u, %02u:%02u\n",
           (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,fno.ftime >> 11, fno.ftime >> 5 & 63);
    printf("������: %c%c%c%c%c\n\n",
	       (fno.fattrib & AM_DIR) ? 'D' : '-',      // ��һ��Ŀ¼   x?x:x��Ŀ�������ж��ļ�������
           (fno.fattrib & AM_RDO) ? 'R' : '-',      // ֻ���ļ�
           (fno.fattrib & AM_HID) ? 'H' : '-',      // �����ļ�
           (fno.fattrib & AM_SYS) ? 'S' : '-',      // ϵͳ�ļ�
           (fno.fattrib & AM_ARC) ? 'A' : '-');     // �����ļ�
  }
  return res_flash;
}




/**
  * @brief  scan_files �ݹ�ɨ��FatFs�ڵ��ļ�
  * @param  path:��ʼɨ��·��
  * @retval result:�ļ�ϵͳ�ķ���ֵ   ��Ҫ����f_readdir()����  �ο�����������
     ����һ���ݹ麯�����������Լ������Լ��� ����������ļ������ʹ��ļ������ݹ飬��������ļ��ͽ����ݹ�ֱ�����
*/
static FRESULT scan_files (char* path) 
{ 
  FRESULT res; 		//�����ڵݹ���̱��޸ĵı���������ȫ�ֱ������ݹ麯������ֲ������������ڵݹ�Ĵ�������ֲܾ���������ർ�����������ջ�ռ�Ҫ�Ĵ�	
  FILINFO fno;      
  DIR dir;          //����Ŀ¼��Ϣ�Ľṹ��
  int i;            //pathp[i]  
  char *fn;        // �ļ���	
	
#if _USE_LFN 
  /* ���ļ���֧�� */
  /* ����������Ҫ2���ֽڱ���һ�����֡�*/
  static char lfn[_MAX_LFN*2 + 1]; 	//�洢���ļ�������
  fno.lfname = lfn; 
  fno.lfsize = sizeof(lfn); 
#endif 
	
	printf("����ݹ�\r\n");
  //��Ŀ¼
  res = f_opendir(&dir, path); //f_opendir ���ļ�1��
  if (res == FR_OK) 
	{ 
    i = strlen(path); //���Ŀ¼�ĳ��ȣ���������ƴ���ַ��� ��ӡ����
    for (;;)//û�����Ƶ�ѭ������break��������һֱѭ�� 
		{ 
      //��ȡĿ¼�µ����ݣ��ٶ����Զ�����һ���ļ�
      res = f_readdir(&dir, &fno); 								
      //Ϊ��ʱ��ʾ������Ŀ��ȡ��ϣ�����
      if (res != FR_OK || fno.fname[0] == 0) break; 	
#if _USE_LFN 
      fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
      fn = fno.fname; 
#endif 
      //���ʾ��ǰĿ¼������			
      if (*fn == '.') continue; 	
      //Ŀ¼���ݹ��ȡ      
      if (fno.fattrib & AM_DIR) //�����Ŀ¼  ƴ�� �������õݹ�      
			{ 			
        //�ϳ�����Ŀ¼��        
        sprintf(&path[i], "/%s", fn);//sprintf()ƴ�Ӻ��� ƴ���ļ��� 		
        //�ݹ����         
        res = scan_files(path);	
        path[i] = 0;         
        
        if (res != FR_OK)  //��ʧ�ܣ�����ѭ�� ����return �����ݹ�  
					break; 
      } 
			else //�������Ŀ¼�����ļ����Ļ�
			{ 
				printf("%s/%s\r\n", path, fn);	//ֱ�������ӡ����
				//����ļ���	
				printf("���������ӡ\r\n");
        /* ������������ȡ�ض���ʽ���ļ�·�� */        
      }//else
    } //for
  } 
  return res; 
}






/****************************************************************************************/
/********************************����������***********************************************/
/****************************************************************************************/
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{    	
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	USART_Config();	
  printf("******** ����һ��SPI FLASH �ļ�ϵͳʵ�� *******\r\n");
  
	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	res_flash = f_mount(&fs,"1:",1); //һ��Ҫ�ȹ��أ������������ܼ�����ʼ
  if(res_flash!=FR_OK)
  {
    printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
    printf("��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");
		while(1);
  }
  else
  {
    printf("���ļ�ϵͳ���سɹ������Խ��в���\r\n");    
  }
  
  /* FatFs����ܲ��� */
  res_flash = miscellaneous();  //miscellaneous Ӧ�ò㺯�����Լ�д��

  
  printf("\n*************** �ļ���Ϣ��ȡ���� **************\r\n");
  res_flash = file_check(); //file_check Ӧ�ò㺯�����Լ�д��

  //�����Ϻ���file_check�õ����ļ����� �Ϳ��Խ������µ��ļ�ɨ��
  printf("***************** �ļ�ɨ����� ****************\r\n");
  strcpy(fpath,"1:"); //strcpy�����ַ������Ѵ洢�豸1: �����ָ��Ƹ�fpath
  scan_files(fpath);  //scan_files Ӧ�ò㺯�����Լ�д��   ɨ��fpath��ɨ��1:
  
  
	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
	f_mount(NULL,"1:",1);
  
  /* ������ɣ�ͣ�� */
	while(1)
	{
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

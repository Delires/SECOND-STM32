#include "fatfuction.h"
#include "string.h" //strlen()����ͷ�ļ�
/**********************************************************************************************
�������дһЩFATF����غ���1.�洢���ʴ�С��ȡ  2.�ļ���Ϣ��ȡ  3.�ļ�ɨ��
�������������ǻ���FATFSϵͳ�Ļ���������������ģ����ڶ��㣬�������вο�д��

**********************************************************************************************/
FRESULT res_flash;                /* �ļ������������ֵ */
FIL fnew;						  /* �ļ�������  ���ò��ֺ���Ҫ�� */
char readbuff[521];      //�洢����������
char fpath[100];         //�洢�ļ�·��

UINT f_br;    //fopen�õı���

FRESULT miscellaneous(void)
{
	FATFS *fps;
	DWORD fre_clust, fre_sector, tot_sector;
	DIR  dir; //f_opendir���� �ļ��о��
    
	printf("-----------------------------------------------------------------------------------------------\r\n");
	printf("-------------------- miscellaneous����Ч�� ----------------------------------------------------\r\n");
	printf("-----------------------------------------------------------------------------------------------\r\n");
/*************** ��ȡ�洢���ʴ�С���� ***************/
	printf("*********** �洢���ʵ��ڴ��С ***************************\r\n");
	
	res_flash = f_getfree("1:", &fre_clust, &fps); //f_getfree������ȡ����ռ�
	tot_sector = (fps->n_fatent-2) * fps->csize;
	fre_sector = fre_clust * fps->csize;  

    printf(" �ļ��ܿռ�Ϊ%10lu kb \r\n", tot_sector*4);
	printf(" �ļ�ʣ��ռ�Ϊ%10lu kb \r\n", fre_sector*4);
	

/*************** ��洢����д��һ���ļ� ***************/	
	printf("*********** �ļ���λ�͸�ʽ��д�빦�ܲ��� *********************\r\n");
	res_flash = f_open( &fnew, "1:ceshi.txt", FA_OPEN_ALWAYS|FA_READ|FA_WRITE); //��һ���ļ�������Ȩ��
	if( res_flash == FR_OK ) //�򿪳ɹ�
	{
		res_flash = f_lseek(&fnew, f_size(&fnew) ); //�Ȱѹ��ŵ��ļ���ĩβ��������ʼд�� fnew�ļ��Ĺ��ƫ���ļ��Ĵ�С
		if( res_flash == FR_OK ) //�򿪳ɹ�
		{
			f_printf( &fnew,"miscellaneous����д���ԣ����ļ���д����Ϣ���ļ��ܿռ�Ϊ%lu kb,  �ļ�ʣ��ռ�Ϊ%lu kb \r\n", tot_sector*4,  fre_sector*4 );//ֱ�����ļ�д����Ϣ ����f_write
		}

	    res_flash = f_lseek(&fnew, 0); //д�����ƶ����ļ���ͷ
		
		res_flash = f_read( &fnew, readbuff, f_size(&fnew), &f_br);
		if( res_flash == FR_OK ) //��ȡ�ɹ�
		{
			printf("��������ϢΪ��%s ", readbuff); //��ȡ��Ϣ
		
		}
		f_close(&fnew); //�ر��ļ�
	
	
/*************** ��һ���ļ��У����������Ǹ��ļ��ƽ�ȥ  ���������������Ǹ��ļ� ***************/	
		printf("*********** �����ļ����ƶ��ļ� ��  �������ļ��� ***********\r\n");
		res_flash = f_opendir( &dir, "1:DirText");
		if(res_flash != FR_OK) //��ʧ��
		{
			res_flash = f_mkdir("1:DirText"); //����һ���ļ���
		}
		else
		{
			res_flash = f_closedir(&dir); //�ر��ļ���
			f_unlink("1:DirText/shi.txt"); //ɾ��ceshi.txt�ļ�
			
		}
		if(res_flash == FR_OK) //����һ���ļ��� �ɹ�
		{
			res_flash = f_rename ("1:ceshi.txt", "1:DirText/shi.txt"); //������+ ��/�ƶ��ļ�
			if(res_flash == FR_OK) 
			{
				printf(" ������+ ��/�ƶ��ļ� �ɹ� �� FLASH�ĳ�U�̿����� \r\n");
			}
		}
	}
	else
	{
		printf(" ��洢����д���ļ�ʧ�� \r\n");
	}
	
	
	return res_flash;
}









/***********************************************
  �ļ���Ϣ��ȡ����  ����f_stat()����
  ����:��ȡ�洢�����ڵ��ļ����ļ��е���Ϣ
  �˺����ǹ�������FATFS��׼����
***********************************************/
FILINFO fno;
FRESULT file_check(void)
{
	printf("-----------------------------------------------------------------------------------------------\r\n");
	printf("-------------------- miscellaneous����Ч�� ----------------------------------------------------\r\n");
	printf("-----------------------------------------------------------------------------------------------\r\n");
	
	res_flash = f_stat ("1:DirText/shi.txt", &fno);
	if( res_flash == FR_OK )
	{
		printf("shi.txt���ļ���Ϣ:\r\n");
		printf("�ļ���СΪ%ld(�ֽ�):\r\n",  fno.fsize);
		printf("�ļ�����ʱ��: %u/%02u/%02u, %02u:%02u\n",
               (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,
               fno.ftime >> 11, fno.ftime >> 5 & 63);
		//�и��涨ʱ��� 1980/1/1 00:00��ʼ��  
		
		 printf("Attributes: %c%c%c%c%c%c%c%c      ",           //�ļ�����
				(fno.fattrib & AM_RDO) ? 'R': '-' ,  //��λ���㣬�����AM_DIR���� �ʹ�ӡ D ���Ǵ�ӡ-
				(fno.fattrib & AM_HID) ? 'H': '-' ,
				(fno.fattrib & AM_SYS) ? 'S': '-' , 
				(fno.fattrib & AM_VOL) ? 'V': '-' ,
				(fno.fattrib & AM_LFN) ? 'L': '-' ,
				(fno.fattrib & AM_DIR) ? 'D': '-' ,
				(fno.fattrib & AM_ARC) ? 'A': '-' ,
				(fno.fattrib & AM_MASK) ? 'M': '-'	    
                );	
          printf("�ļ�����д��ĸ������ѯff.h\r\n");	

          return res_flash;		
	}
	else
	{
		printf(" f_stat()������ȡ�洢��������ļ���ʧ�� \r\n");
		  return res_flash;		
	}
}









/***********************************************
  ɨ��洢���ʵ������ļ�  ������ f_readdir()  f_opendir ����
  ����:��ȡ�洢�����ڵ��ļ����ļ��е���Ϣ
  �˺����ǹ�������FATFS��׼����
  ����һ���ݹ麯��
  ***********************************************/
FRESULT scan_files (char* path)//�������һ��ָ�룬������
{ 
//�ݹ麯������ֲ�������ȫ�ֲ�����ģ������ڵݹ�Ĵ�������ֲܾ���������ർ�����������ջ�ռ�Ҫ�Ĵ�	
    FRESULT res; 	//�洢�ļ�������������״̬
    FILINFO fno;    //f_readdir()������Ҫ�ķ���ֵ�ýṹ��
	DIR dir;          //�ļ��о��
	int i;          //�洢Ŀ¼����
	char *fn;      //�ļ���
	

#if _USE_LFN 
/* ���ļ���֧�� */
	static char lfn[_MAX_LFN*2 +1]; //����������ڴ洢���ļ���
	fno.lfname = lfn;            //�ļ�����
	lfn.lfsize = sizeof(lfn);   //�ļ�������
#endif	
	
	res = f_opendir(&dir,  path); //f_opendir��'1:'�ļ��� ���൱��ֱ�Ӵ򿪴洢�豸
	if( res == FR_OK)
	{
		i = strlen( path ); //��ȡĿ¼���ȣ����ں���ƴ�Ӵ�ӡ
		for(; ; ) //��ѭ��
		{
			res = f_readdir(&dir, &fno);
			if( res!=FR_OK || fno.fname[0] == 0) break;
			     //һֱѭ�������������ļ�ʧ�� �� �ļ���Ϊ���˳�
#if _USE_LFN
			fn = *fno.lfname ? fno.lfname : fno.fname //fno.lfname��ֵfn��ָ��fno.lfname ����ָ�� fno.name 
#else
			fn = fno.fname; //�ļ�����ֵ��fn  fno.fname����������ָ��
#endif
			if( *fn == '.') continue; //��������ļ��ͼ������ļ�������'.'
			
			if(fno.fattrib & AM_DIR) //�ļ��������ļ��� �ݹ�ͼ�������
			{
				sprintf(&path[i], "/%s", fn );  // sprintf(char *string, char *format [,argument,...]);��formatƴ��/�滻��string   argument��format��ֵ
			            //����������ļ���  �Ͱѵ�ǰ���ļ����洢��path�ַ�������
				        //i = strlen( path ); ��path�ĵڼ�����������ƴ�Ӹ�����Ҫ��ӡ��ֵ
				res = scan_files( path); //�ټ����ݹ�
				
				path[i] = 0;  //�ݹ������path[i]ֵ��0
				if(res != FR_OK)
					  break;
			}
			else //������ļ��� ֤����������
			{
				printf("%s/%s\r\n", path, fn);
			}
		}
	}	
	return res;	
}










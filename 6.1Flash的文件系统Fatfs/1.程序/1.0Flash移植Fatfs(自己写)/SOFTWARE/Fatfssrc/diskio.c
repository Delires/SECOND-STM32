/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
/* (������FATFS�ʹ洢���ʵײ�IO��һЩ����������Ҫ�Լ�ʵ��)*/
/* ʹ��FATFS��ȫ������ײ�IO��*/

#include "diskio.h"		/* FatFs lower layer API */
#include "SPIFlash.h"
#include "integer.h"
/*������һЩ�ٷ�������û�õ���Щͷ�ļ���ע�͵�*/
//#include "usbdisk.h"	/*usb�洢 Example: Header file of existing USB MSD control module */
//#include "atadrive.h"	/*ada�洢 Example: Header file of existing ATA harddisk control module */
//#include "sdcard.h"		/*sd���洢 Example: Header file of existing MMC/SDC contorl module */

/* Definitions of physical drive number for each drive */

//����SD�� �� FLASH���
#define SD_CARD		   0	/* Example: SD_CARD to physical drive 0 */
#define SPI_FLASH	   1	/* Example: SPI_FLASH to physical drive 1 */




#define FLASH_SECTOR_SIZE 	512			  
//����W25Q128
//ǰ12M�ֽڸ�fatfs��,12M�ֽں�,���ڴ���ֿ�,�ֿ�ռ��3.09M.	ʣ�ಿ��,���ͻ��Լ���	 			    
u16	    FLASH_SECTOR_COUNT=2048*12;	//W25Q1218,ǰ12M�ֽڸ�FATFSռ��
#define FLASH_BLOCK_SIZE   	8     	//ÿ��BLOCK��8������


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)                   /* ͨ�����pdrv ��ȷ�������ĸ��洢����(�ñȵ���������ô���̸��ݱ�Ų����ĸ�) */
{
	DSTATUS stat= STA_NOINIT;   //����״̬λҪ����ֵ��Ȼ�����
	//int result;  //û�õ�ע��

	switch (pdrv) {
	case SD_CARD :         //pdrv��ATA����һ��ģ��Ǵ洢����š�switch(���)��ִ���ĸ��洢���ĵײ��״̬����
	   //result = ATA_disk_status(); //�ٷ����̵ĵײ�����û�õ���ע�͵�

		// translate the reslut code here
		break;
	case SPI_FLASH :
			if( SPI_FLASH_Read_ID() == W25Q128 )
			{
				//PROGRAM_PRINTF("\r\n ��ַ=%X \r\n", SPI_FLASH_Read_ID());
				stat &= ~STA_NOINIT;  //0��ʼ���ɹ�
			}
			else
			{
				stat = STA_NOINIT;
			}
		break;
		
	default:
		    stat = STA_NOINIT;
 

	}
	return stat; //δ����switch��� ֱ�ӷ��ش���
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)                           /* ͨ�����pdrv ��ȷ�������ĸ��洢����(�ñȵ���������ô���̸��ݱ�Ų����ĸ�) */
{
	u16 i;
	DSTATUS stat=STA_NOINIT;  //����״̬λҪ����ֵ��Ȼ�����
	//int result;  //û�õ�ע��
	switch (pdrv) {
	case SD_CARD :
		//result = ATA_disk_initialize(); //�ٷ����̵ĵײ�����û�õ���ע�͵�
		break;

	case SPI_FLASH :
			SPI_Communit_Init();  //SPI��ʼ��
			i=500;
			while(--i);
			Release_Power_Down_Flash(); //����flash����ֹ����͹���
			stat=disk_status(SPI_FLASH); //��ͨ������״̬λ��������ʼ���Ƿ�ɹ�
	        
	        
			//PROGRAM_PRINTF("\r\n ��ʼ��ֵ=%X \r\n",stat);
		break;
	
	default:
		    stat = STA_NOINIT;

	}
	return stat; //δ����switch��� ֱ�ӷ��ش���
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/* �������:pdrv(�豸��),*buff(���ݴ洢·��),sector(������ַ),	count(��ȡ����)  */
/* ���ز�����RES_OK (0)��RES_ERROR��RES_PARERR��RES_NOTRDY                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive �豸��*/
	BYTE *buff,		/* Data buffer to store read data ���ݴ洢��λ��*/
	DWORD sector,	/* Sector address in LBA  ������������ţ���Ҫ�ں����а�������ת��Ϊ��ַ*/
	UINT count		/* Number of sectors to read ��ȡ������*/
)
{	
	DRESULT res=RES_PARERR; //����״̬λҪ����ֵ��Ȼ�����
	
	if( !count ) //д���ֽ���Ϊ0 ֱ�ӽ���
	{
		return RES_PARERR;
	}

	switch (pdrv) {
	case SD_CARD :
			// translate the arguments here

			//result = ATA_disk_read(buff, sector, count); //�ײ�Ķ�ȡ��������
	     break;                                            //�ٷ����̵ĵײ�����û�õ���ע�͵�

		

	case SPI_FLASH :
		//sector+=770; //����Ҫ��ƫ�Ƶ�ַ�������޷������ļ�
	
			Read_Sector_Flash(sector*4096, buff, count*4096);
						//sector�������������(�ϲ㺯����С��λ������)��ÿ������4096byte�����ײ�SPI���������������ַ������Ҫ������*4096������ȷ�ĵ�ַ
						//count�Ƕ�ȡ�������������ײ�SPI����������Ƕ�ȡ�ֽ���������count*4906
			res = RES_OK;
	    break;
	
	default:
			res = RES_PARERR;
	
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res=RES_PARERR;  //����״̬λҪ����ֵ��Ȼ�����
	
	if( !count ) //д���ֽ���Ϊ0 ֱ�ӽ���
	{
		return RES_PARERR;
	}
	
	//int result;  //û�õ�ע��

	switch (pdrv) {
	case SD_CARD :
          //PROGRAM_PRINTF("\r\n д����ֵǰ�� \r\n");
		break;

	case SPI_FLASH : 
			//sector+=770; //����Ҫ��ƫ�Ƶ�ַ�������޷������ļ�
			Flash_Sector_Erase(sector*4096); //�ϲ㰲���������������ĵ�ַ*4096(һ������ֵ)
	                                      //����Ҫ�Ȳ�����д�룡������
			Write_More_256Byte_YeHuo(sector*4096, (u8 *)buff, count*4096);  //count*4096 == count<<12
			res = RES_OK;
	    
		break;
	
	default:
			res = RES_PARERR;

	}

	return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions(��Ҫ)һЩ�������洢�ռ���/��ʽ��/ʣ��ռ�  �˺������� */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..)  �豸�� */
	BYTE cmd,		/* Control code ����ָ��(ͨ����ͬ��ָ�������ƴ洢���ʷ���ʣ��ռ䣬��ʽ����)*/
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res=RES_PARERR;  //����״̬λҪ����ֵ��Ȼ�����
	//int result;  //û�õ�ע��

	switch (pdrv) {
	case SD_CARD :   //���caseû��break����Ϊ�˼��з���ֵ ִ����ֱ�ӽ���������

		// Process of the command for the ATA drive
		break;

	case SPI_FLASH :
		switch(cmd)
		{
			//�������sector��Ŀ
			case GET_SECTOR_COUNT:  //��Щ������Ҫ�Լ���ײ㺯��ʵ��
					*((DWORD*)buff) = 3326;  //4096   (256*16)-770=3326  ǰ770k���ֿ��ļ�ռ�� �������õ�Ҫ��ȥ770
							 //W25Q128��256��block��ÿ��block��16��sector
				 break;      //����ַ�ð�buffָ��ǿ��ת��ΪDWORDָ��
				
			//�������sector��С
			case GET_SECTOR_SIZE:
					*((WORD*)buff) = 4096;//���ֲ�W25Q1281��sector��4K=4*1024=4096BYTE
									   //����ַ��֪buff��WORDָ��
				break;
			
			//�������sector�����ٲ����ĸ���"ע��������������Ϊ��λ"
			case GET_BLOCK_SIZE:   
					*((DWORD*)buff) = 1; //���ֲ�W25Q1281���ٵ�erase��һ������
			
				break;
		//			case CTRL_TRIM:
		//				
		//			
		//			break;
			
				
		}
		res = RES_OK;
		break;

	default:
		res = RES_PARERR;

	}

	return res;
}
#endif



/*-----------------------------------------------------------------------*/
/* �õ��ļ��޸Ļ򴴽���ʱ�亯��     ���õ�RTC����ȡ��ǰʱ��                                     */
/*-----------------------------------------------------------------------*/

DWORD get_fattime(void)
{
	/* ���ص�ǰʱ��� */
	return	  ((DWORD)(2015 - 1980) << 25)	/* Year 2015 */
			| ((DWORD)1 << 21)				/* Month 1 */
			| ((DWORD)1 << 16)				/* Mday 1 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				  /* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}

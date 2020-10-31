/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
/* (连接了FATFS和存储介质底层IO的一些函数，函数要自己实现)*/
/* 使得FATFS完全独立与底层IO口*/

#include "diskio.h"		/* FatFs lower layer API */
#include "SPIFlash.h"
#include "integer.h"
/*下面是一些官方范例，没用到这些头文件就注释掉*/
//#include "usbdisk.h"	/*usb存储 Example: Header file of existing USB MSD control module */
//#include "atadrive.h"	/*ada存储 Example: Header file of existing ATA harddisk control module */
//#include "sdcard.h"		/*sd卡存储 Example: Header file of existing MMC/SDC contorl module */

/* Definitions of physical drive number for each drive */

//定义SD卡 和 FLASH标号
#define SD_CARD		   0	/* Example: SD_CARD to physical drive 0 */
#define SPI_FLASH	   1	/* Example: SPI_FLASH to physical drive 1 */




#define FLASH_SECTOR_SIZE 	512			  
//对于W25Q128
//前12M字节给fatfs用,12M字节后,用于存放字库,字库占用3.09M.	剩余部分,给客户自己用	 			    
u16	    FLASH_SECTOR_COUNT=2048*12;	//W25Q1218,前12M字节给FATFS占用
#define FLASH_BLOCK_SIZE   	8     	//每个BLOCK有8个扇区


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)                   /* 通过标号pdrv 来确定操作哪个存储介质(好比电脑里有那么多盘根据标号操作哪个) */
{
	DSTATUS stat= STA_NOINIT;   //定义状态位要赋初值不然会出错
	//int result;  //没用到注释

	switch (pdrv) {
	case SD_CARD :         //pdrv和ATA都是一类的，是存储器标号。switch(标号)就执行哪个存储器的底层读状态函数
	   //result = ATA_disk_status(); //官方例程的底层驱动没用到，注释掉

		// translate the reslut code here
		break;
	case SPI_FLASH :
			if( SPI_FLASH_Read_ID() == W25Q128 )
			{
				//PROGRAM_PRINTF("\r\n 地址=%X \r\n", SPI_FLASH_Read_ID());
				stat &= ~STA_NOINIT;  //0初始化成功
			}
			else
			{
				stat = STA_NOINIT;
			}
		break;
		
	default:
		    stat = STA_NOINIT;
 

	}
	return stat; //未进入switch语句 直接返回错误
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)                           /* 通过标号pdrv 来确定操作哪个存储介质(好比电脑里有那么多盘根据标号操作哪个) */
{
	u16 i;
	DSTATUS stat=STA_NOINIT;  //定义状态位要赋初值不然会出错
	//int result;  //没用到注释
	switch (pdrv) {
	case SD_CARD :
		//result = ATA_disk_initialize(); //官方例程的底层驱动没用到，注释掉
		break;

	case SPI_FLASH :
			SPI_Communit_Init();  //SPI初始化
			i=500;
			while(--i);
			Release_Power_Down_Flash(); //唤醒flash，防止进入低功耗
			stat=disk_status(SPI_FLASH); //再通过调用状态位函数看初始化是否成功
	        
	        
			//PROGRAM_PRINTF("\r\n 初始化值=%X \r\n",stat);
		break;
	
	default:
		    stat = STA_NOINIT;

	}
	return stat; //未进入switch语句 直接返回错误
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/* 输入参数:pdrv(设备号),*buff(数据存储路径),sector(扇区地址),	count(读取数量)  */
/* 返回参数：RES_OK (0)，RES_ERROR，RES_PARERR，RES_NOTRDY                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive 设备号*/
	BYTE *buff,		/* Data buffer to store read data 数据存储的位置*/
	DWORD sector,	/* Sector address in LBA  输入的是扇区号，需要在函数中把扇区号转换为地址*/
	UINT count		/* Number of sectors to read 读取扇区数*/
)
{	
	DRESULT res=RES_PARERR; //定义状态位要赋初值不然会出错
	
	if( !count ) //写入字节数为0 直接结束
	{
		return RES_PARERR;
	}

	switch (pdrv) {
	case SD_CARD :
			// translate the arguments here

			//result = ATA_disk_read(buff, sector, count); //底层的读取扇区函数
	     break;                                            //官方例程的底层驱动没用到，注释掉

		

	case SPI_FLASH :
		//sector+=770; //必须要加偏移地址，否则无法生成文件
	
			Read_Sector_Flash(sector*4096, buff, count*4096);
						//sector输入的是扇区号(上层函数最小单位是扇区)，每个扇区4096byte，但底层SPI函数输入的扇区地址，所以要扇区号*4096才是正确的地址
						//count是读取的扇区数，但底层SPI函数输入的是读取字节数。所以count*4906
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
	DRESULT res=RES_PARERR;  //定义状态位要赋初值不然会出错
	
	if( !count ) //写入字节数为0 直接结束
	{
		return RES_PARERR;
	}
	
	//int result;  //没用到注释

	switch (pdrv) {
	case SD_CARD :
          //PROGRAM_PRINTF("\r\n 写函数值前面 \r\n");
		break;

	case SPI_FLASH : 
			//sector+=770; //必须要加偏移地址，否则无法生成文件
			Flash_Sector_Erase(sector*4096); //上层安扇区擦，因此输入的地址*4096(一个扇区值)
	                                      //必须要先擦除再写入！！！！
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
/* Miscellaneous Functions(重要)一些杂项，比如存储空间多大/格式化/剩余空间  此函数复杂 */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..)  设备号 */
	BYTE cmd,		/* Control code 操作指令(通过不同的指令来控制存储介质发送剩余空间，格式化等)*/
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res=RES_PARERR;  //定义状态位要赋初值不然会出错
	//int result;  //没用到注释

	switch (pdrv) {
	case SD_CARD :   //这个case没有break是因为人家有返回值 执行完直接结束函数了

		// Process of the command for the ATA drive
		break;

	case SPI_FLASH :
		switch(cmd)
		{
			//该命令返回sector数目
			case GET_SECTOR_COUNT:  //这些命令需要自己编底层函数实现
					*((DWORD*)buff) = 3326;  //4096   (256*16)-770=3326  前770k被字库文件占了 真正能用的要减去770
							 //W25Q128有256个block，每个block有16个sector
				 break;      //查网址得把buff指针强制转化为DWORD指针
				
			//该命令返回sector大小
			case GET_SECTOR_SIZE:
					*((WORD*)buff) = 4096;//查手册W25Q1281个sector有4K=4*1024=4096BYTE
									   //查网址得知buff是WORD指针
				break;
			
			//该命令返回sector的最少擦除的个数"注意是以扇区个数为单位"
			case GET_BLOCK_SIZE:   
					*((DWORD*)buff) = 1; //查手册W25Q1281最少的erase是一个扇区
			
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
/* 得到文件修改或创建的时间函数     调用到RTC来获取当前时间                                     */
/*-----------------------------------------------------------------------*/

DWORD get_fattime(void)
{
	/* 返回当前时间戳 */
	return	  ((DWORD)(2015 - 1980) << 25)	/* Year 2015 */
			| ((DWORD)1 << 21)				/* Month 1 */
			| ((DWORD)1 << 16)				/* Mday 1 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				  /* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}

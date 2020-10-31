#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "SPIFlash.h"
#include "string.h"

/* FATFS头文件调用 */
#include "integer.h"
#include "ff.h"
#include "diskio.h"
#include "ffconf.h"
#include "fatfuction.h"

/**
blog
20200818：
      1.推测应该是硬件问题，应该是写函数的问题   ----  是杂项那个函数没返回值
20200819：
      1.f_open老返回10 写保护 为什么？  猜测还是底层原因   移植到野火的函数也不出问题
	     因为写diskio函数时，初始化DSTATUS stat没有赋初值，因此电脑会随机赋值导致返回值出错
         切记定义变量尽量赋一个初始值，这属于玄学问题
		 
20200824：1.flash按U盘打开没有新建文件夹，而野火却新建文件夹了  为什么
             还是怀疑底层有问题
          2.按野火的移植我程序文件夹有了 但没有内容
		     主函数按照野火 先open-write-close   再open-read-close读的顺序写就有值了
		  3.我的主函数，出现第二次打开失败。野火的函数第二次却能打开成功。
		     我写文件-----我再读   失败
			 野火写文件----野火读  成功
			 野火写----我读        成功          我的写文件就有问题！！！diskio写/SPI写有问题？
			 不是SPI写函数的问题，是diskio写接口的问题 
			 
20200923: FLASH开头没存系统文件，所以把sector+770注释掉，不需要偏移地址。、
          这样scan_files()函数才能扫描到
"1:"
		  
		  
**/
u8 WriteData_char[]="SPI普通写入数据实验检测";  //abcasdasdasdasd
u8 ReadData_char[80];
char path[100];  

FATFS FlashfsObject;  //FLASH--FAFTS文件系统结构体 定义为全局 定义到堆里 防止溢出
//FATFS FlashfsObject;  //每有一个存储介质，就要定义一个挂在的FAFTS文件系统结构体
FIL fp; //建立一个文件句柄
//调用FATFS文件系统写文件的话  相关存储数据的变量就必须要用FATFS文件系统中的"变量类型"定义变量   
//如果不按照FATFS文件系统中的"变量类型"定义变量，就会卡死了。分配的栈的大小不匹配就卡死了
//FATFS文件系统的函数是按照文件系统中的"变量类型"定义变量来操作，而定义的变量如果不匹配，那么函数操作的变量的栈大小 与 自己定义变量的栈大小不匹配，所以就卡死了，就会出现栈溢出
BYTE  Fatfs_WriteData_char[]="FATFS测试文件写入数据：加油！ 奥利给！！！";
BYTE  Fatfs_ReadData_char[800]="";    //必须要用FATFS文件系统中的"变量类型"定义变量   

UINT bw;
UINT br;


 int main(void)
 {	
	 u8 i;
	 FRESULT res; //定义文件系统操作返回的状态值
	 
  
	 
	 delay_init();
	 uart_init(115200);
	 LED_Init();

    //PROGRAM_PRINTF("\r\nFATFS内存值为: %d\r\n", sizeof(FATFS) );  //打印字符串 直接输数组名
	//drive_number = &j;
	// PROGRAM_PRINTF("\r\n%d   %d\r\n", (&drive_number),    (*drive_number)); 
	
	 


	 
/***************  挂载FATFS文件系统 **************************************/
/*存储介质挂载文件系统*/	
	  res=f_mount( &FlashfsObject, "1:", 1);  //必须以 “number：”这种形式输入存储介质标号。如“1：” “2：”  为啥我也不知道就是规定
	  PROGRAM_PRINTF("\r\n fmount_res=%d \r\n", res);
	 
/*格式化磁盘 重新加载FATFS   和字库文件无关*/	 
	  if(res == FR_NO_FILESYSTEM) //如果没文件系统 就格式化在重新挂载
	  {
		res = f_mkfs("1:", 0 ,0);
		PROGRAM_PRINTF("\r\n f_mkfs_res=%d \r\n", res);
		  
		//格式化后要取消挂载 再重新挂载
		f_mount( NULL, "1:", 1);  //取消挂载
		res = f_mount( &FlashfsObject, "1:", 1);//重新挂载	
        PROGRAM_PRINTF("\r\n f_mountsecond_res=%d \r\n", res);		  
		  
	  }

	  
/***************  FLASH的基本信息获取 **************************************/	 
		miscellaneous();
	  
/***************  读取FLASH文件的信息 **************************************/	 	  
	    file_check();
/***************  扫描存储介质所有文件 **************************************/	  
		printf("-----------------------------------------------------------------------------------------------\r\n");
		printf("-------------------- scan_files函数效果 ----------------------------------------------------\r\n");
		printf("-----------------------------------------------------------------------------------------------\r\n");
		strcpy(path,"1:"); //strcpy复制字符串，把存储设备1: 的名字复制给fpath
	             //必须要使用这种操作，不能直接输入"1:"，否则会出错
		scan_files(path);  //scan_files 应用层函数，自己写的   扫描fpath即扫描1:

	  
//	  
///*************** 对FATFS文件的基本操作 打开文件写操作**************************************/	 
//printf("\r\n\r\n*********** 对FATFS文件的基本操作 打开文件写操作 ***********\r\n");	  
///*打开一个文件*/
//	  //文件操作，先打开文件获得句柄，才能进行后来的操作
//	  res = f_open(&fp, "1:asf.txt",  FA_OPEN_EXISTING|FA_CREATE_ALWAYS|FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
//                                    //打开/写/读的权限都给上	   
///*写入一个文件*/
//	  //执行其他操作前必须先打开一个文件 获得句柄和权限
//		if(res == FR_OK)//文件打开成功
//		{ 
//			PROGRAM_PRINTF("\r\n f_open_res=%d \r\n", res);	
//			
//			res = f_write(&fp, Fatfs_WriteData_char,sizeof(Fatfs_WriteData_char), &bw);
//			PROGRAM_PRINTF("\r\n f_write_res=%d 写入字节数：bw=%d \r\n", res, bw);
//			if(res == FR_OK)//文件写入成功
//			{
//				/*读取一个文件*//*读取一个文件*/
//				f_lseek(&fp, 0); //让光标回到文件最开始;
//				res=f_read(&fp, Fatfs_ReadData_char, f_size(&fp), &br);  //fp的定义是  FIL fp;
//																	  //bw的定义是  UINT bw     它们都是按数据定义的，而传回的参数是指针，所以要写为指针
//				PROGRAM_PRINTF("\r\n f_read_res=%d  读取字节数：br=%d \r\n", res, br);
//				if(res == FR_OK)//文件读取成功
//				{
//					PROGRAM_PRINTF("\r\n 读取文件值为：%s \r\n", Fatfs_ReadData_char);
//				}
//				else
//				{
//					PROGRAM_DEBUG("\r\n 读取文件错误，错误值f_write_res=%d \r\n", res);
//				}

//		    }
//			else
//			{
//				PROGRAM_DEBUG("\r\n 文件写入失败，值为f_write_res=%d \r\n", res);
//			}
//		}
//		else
//		{
//		  PROGRAM_DEBUG("\r\n f_open文件打开错误，错误值f_open_res=%d \r\n", res);
//		}
///*文件关闭*/	
//		f_close(&fp);  //记得执行这步操作，否则值不存储到文件中
//		
//		
//		
//		//miscellaneous();
//		
///*删除一个文件*/		
//		res = f_unlink("1:mengdi.txt");//删除文件函数	  
//		if(res == FR_OK)
//		{
//			PROGRAM_PRINTF("\r\n f_unlink()函数删除文件成功 \r\n");
//		}
//		else
//		{
//			PROGRAM_DEBUG("\r\n 删除失败，错误值f_unlink_res=%d \r\n", res);
//		}

//	 
//	 
//	 
///***************  写字符串 **************************************/
//        /** 擦除Flash **/
//		Flash_Sector_Erase( 0x000000 );
//		printf("----------------往Flash写入字符串------------\r\n"); 
//		/** 写Flash **/
//		printf("\r\n\r\n\r\n\r\n\r\n\r\n"); 
//		printf("\r\n 往Flash存储的的数据为:\r\n%s\r\n", WriteData_char );  //打印字符串 直接输数组名
//     	Write_Less256byte_Flash(0x000000, WriteData_char, 80);

//		/*** 读取FLSH ***/ 
//		Read_Sector_Flash(0x000000, ReadData_char, 80);   //1k = 1024byte (不是位)
//		printf("\r\n Flash读取的数据为:\r\n%s\r\n", ReadData_char); 

//		

/************************ 基本部分 ************************/	
	 while(1)
	 {
		   i++;	
		  delay_ms(500);
		  LED1=!LED1;//显示函数正在运行
//		  if(i % 10 == 0)
//		  {
//			  i = 0;
//			   printf("\r\n 程序由NoMax(无极)所有 \r\n 供大家参考\r\n 这是一文件管理系统 SPI2--W25Q128-FAFFS\r\n ");

//		  }
	  

	 }	 
 }

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
// 
//FATFS FlashfsObject;  //FLASH--FAFTS文件系统结构体 定义为全局 定义到堆里 防止溢出
////FATFS FlashfsObject;  //每有一个存储介质，就要定义一个挂在的FAFTS文件系统结构体
//FIL fp; //建立一个文件句柄

//u8 WriteData_char[]="加油！ 奥利给！！！";
//u8 ReadData_char[80]="";


//BYTE Fatfs_WriteData_char[]="梦迪 我爱你";
//BYTE Fatfs_ReadData_char[800]="";
////数据要用常量const？？
//UINT bw;
//UINT br;





// int main(void)
// {	
//	 FRESULT res; //定义文件系统操作返回的状态值
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
////替换下文 这部分	 
//	 
///***************  FATFS文件系统操作 **************************************/
///*存储介质挂载文件系统*/	
//	  res=f_mount( &FlashfsObject, "1:", 1);  //必须以 “number：”这种形式输入存储介质标号。如“1：” “2：”  为啥我也不知道就是规定
//	  printf("\r\n fmount_res=%d \r\n", res);
//	 
///*格式化磁盘*/	 
//	 
//	  if(res == FR_NO_FILESYSTEM) //如果没文件系统 就格式化在重新挂载
//	  {
//		res = f_mkfs("1:", 0 ,0);
//		printf("\r\n f_mkfs_res=%d \r\n", res);
//		  
//		//格式化后要取消挂载 再重新挂载
//		f_mount( NULL, "1:", 1);  //取消挂载
//		res = f_mount( &FlashfsObject, "1:", 1);//重新挂载	
//        printf("\r\n f_mountsecond_res=%d \r\n", res);		  
//		  
//	  }
//	  
///*打开一个文件*/
//	  //文件操作，先打开文件获得句柄，才能进行后来的操作
//	  //res = f_open(&fp, "1:mengdi.txt",    FA_OPEN_EXISTING|FA_CREATE_ALWAYS|FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
//       res = f_open(&fp, "1:apx.txt",    FA_CREATE_ALWAYS | FA_WRITE);                             //打开/写/读的权限都给上	  
//	  printf("\r\n f_open_res=%d \r\n", res);	
///*写入一个文件*/
//	  //执行其他操作前必须先打开一个文件 获得句柄和权限
//	  if(res == FR_OK)
//	  { 
//		  res = f_write(&fp, Fatfs_WriteData_char,sizeof(Fatfs_WriteData_char), &bw);
//		  
//          if(res == FR_OK)
//		  {
//            printf("\r\n f_write_res=%d 写入字节数：bw=%d \r\n", res, bw);
//			 
//		  }			  
// 	  }
//	  else
//	  {
//		  printf("\r\n 写入失败 \r\n");
//	  }
//      res = f_close(&fp);
//	  printf("\r\n 关闭文件  f_close=%d  \r\n", res);

//	  
//	  
///*读取一个文件*//*读取一个文件*/
//	  
//	  res = f_open(&fp, "1:apx.txt" ,FA_OPEN_EXISTING | FA_READ); 
//		if(res == FR_OK)
//		{
//             res=f_read(&fp, Fatfs_ReadData_char, f_size(&fp), &br);
//			 printf("\r\n f_read_res=%d  读取字节数：br=%d \r\n", res, br);
//			 if(res == FR_OK)
//			 {
//				 printf("\r\n 文件值为：%s \r\n", Fatfs_ReadData_char);
//			 }
//			 else
//			 {
//				 printf("\r\n 读错误，错误值f_write_res=%d \r\n", res);
//			 }
//		 }
//		else
//		{
//			printf("\r\n 第二次打开文件失败 f_open_res=%d \r\n", res);
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


 


















//FATFS fs;													/* FatFs文件系统对象 */
//FIL fnew;													/* 文件对象 */
//FRESULT res_flash;                /* 文件操作结果 */
//UINT fnum;            					  /* 文件成功读写数量 */
//char fpath[100];                  /* 保存当前扫描路径 */
//char readbuffer[512];             
// 
///**
//  ******************************************************************************
//  *                                任务函数
//  ******************************************************************************
//  */
///* FatFs多项功能测试 */
//static FRESULT miscellaneous(void)
//{
//  DIR dir;
//  FATFS *pfs;
//  DWORD fre_clust, fre_sect, tot_sect;
//  
//  printf("\n*************** 设备信息获取 ***************\r\n");
//  /* 获取设备信息和空簇大小 */
//  res_flash = f_getfree("1:", &fre_clust, &pfs);

//  /* 计算得到总的扇区个数和空扇区个数 */
//  tot_sect = (pfs->n_fatent - 2) * pfs->csize;
//  fre_sect = fre_clust * pfs->csize;

//  /* 打印信息(4096 字节/扇区) */
//  printf("》设备总空间：%10lu KB。\n》可用空间：  %10lu KB。\n", tot_sect *4, fre_sect *4);
//  
//  printf("\n******** 文件定位和格式化写入功能测试 ********\r\n");
//  res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",
//                            FA_OPEN_ALWAYS|FA_WRITE|FA_READ );
//	if ( res_flash == FR_OK )
//	{
//    /*  文件定位 */
//    res_flash = f_lseek(&fnew,f_size(&fnew));
//    if (res_flash == FR_OK)
//    {
//      /* 格式化写入，参数格式类似printf函数 */
//      f_printf(&fnew,"\n在原来文件新添加一行内容\n");
//      f_printf(&fnew,"》设备总空间：%10lu KB。\n》可用空间：  %10lu KB。\n", tot_sect *4, fre_sect *4);
//      /*  文件定位到文件起始位置 */
//      res_flash = f_lseek(&fnew,0);
//      /* 读取文件所有内容到缓存区 */
//      res_flash = f_read(&fnew,readbuffer,f_size(&fnew),&fnum);
//      if(res_flash == FR_OK)
//      {
//        printf("》文件内容：\n%s\n",readbuffer);
//      }
//    }
//    f_close(&fnew);    
//    
//    printf("\n********** 目录创建和重命名功能测试 **********\r\n");
//    /* 尝试打开目录 */
//    res_flash=f_opendir(&dir,"1:TestDir");
//    if(res_flash!=FR_OK)
//    {
//      /* 打开目录失败，就创建目录 */
//      res_flash=f_mkdir("1:TestDir");
//    }
//    else
//    {
//      /* 如果目录已经存在，关闭它 */
//      res_flash=f_closedir(&dir);
//      /* 删除文件 */
//      f_unlink("1:TestDir/testdir.txt");
//    }
//    if(res_flash==FR_OK)
//    {
//      /* 重命名并移动文件 */
//      res_flash=f_rename("1:FatFs读写测试文件.txt","1:TestDir/testdir.txt");      
//    } 
//	}
//  else
//  {
//    printf("!! 打开文件失败：%d\n",res_flash);
//    printf("!! 或许需要再次运行“FatFs移植与读写测试”工程\n");
//  }
//  return res_flash;
//}

//  FILINFO fno;
///**
//  * 文件信息获取
//  */
//static FRESULT file_check(void)
//{

//  
//  /* 获取文件信息 */
//  res_flash=f_stat("1:TestDir/testdir.txt",&fno);
//  if(res_flash==FR_OK)
//  {
//    printf("“testdir.txt”文件信息：\n");
//    printf("》文件大小: %ld(字节)\n", fno.fsize);
//    printf("》时间戳: %u/%02u/%02u, %02u:%02u\n",
//           (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,fno.ftime >> 11, fno.ftime >> 5 & 63);
//    printf("》属性: %c%c%c%c%c\n\n",
//           (fno.fattrib & AM_DIR) ? 'D' : '-',      // 是一个目录
//           (fno.fattrib & AM_RDO) ? 'R' : '-',      // 只读文件
//           (fno.fattrib & AM_HID) ? 'H' : '-',      // 隐藏文件
//           (fno.fattrib & AM_SYS) ? 'S' : '-',      // 系统文件
//           (fno.fattrib & AM_ARC) ? 'A' : '-');     // 档案文件
//  }
//  return res_flash;
//}

///**
//  * @brief  scan_files 递归扫描FatFs内的文件
//  * @param  path:初始扫描路径
//  * @retval result:文件系统的返回值
//  */
//static FRESULT scan_files (char* path) 
//{ 
//  FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量	
//  FILINFO fno; 
//  DIR dir; 
//  int i;            
//  char *fn;        // 文件名	
//	
//#if _USE_LFN 
//  /* 长文件名支持 */
//  /* 简体中文需要2个字节保存一个“字”*/
//  static char lfn[_MAX_LFN*2 + 1]; 	
//  fno.lfname = lfn; 
//  fno.lfsize = sizeof(lfn); 
//#endif 
//  //打开目录
//  res = f_opendir(&dir, path); 
//  if (res == FR_OK) 
//	{ 
//    i = strlen(path); 
//    for (;;) 
//		{ 
//      //读取目录下的内容，再读会自动读下一个文件
//      res = f_readdir(&dir, &fno); 								
//      //为空时表示所有项目读取完毕，跳出
//      if (res != FR_OK || fno.fname[0] == 0) break; 	
//#if _USE_LFN 
//      fn = *fno.lfname ? fno.lfname : fno.fname; 
//#else 
//      fn = fno.fname; 
//#endif 
//      //点表示当前目录，跳过			
//      if (*fn == '.') continue; 	
//      //目录，递归读取      
//      if (fno.fattrib & AM_DIR)         
//			{ 			
//        //合成完整目录名        
//        sprintf(&path[i], "/%s", fn); 		
//        //递归遍历         
//        res = scan_files(path);	
//        path[i] = 0;         
//        //打开失败，跳出循环        
//        if (res != FR_OK) 
//					break; 
//      } 
//			else 
//			{ 
//				printf("%s/%s\r\n", path, fn);								//输出文件名	
//        /* 可以在这里提取特定格式的文件路径 */        
//      }//else
//    } //for
//  } 
//  return res; 
//}






///**
//  * @brief  主函数
//  * @param  无
//  * @retval 无
//  */
//int main(void)
//{    	
//	/* 初始化调试串口，一般为串口1 */
//	uart_init(115200);
//  printf("******** 这是一个SPI FLASH 文件系统实验 *******\r\n");
//  
//	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
//	res_flash = f_mount(&fs,"1:",1);
//  if(res_flash!=FR_OK)
//  {
//    printf("！！外部Flash挂载文件系统失败。(%d)\r\n",res_flash);
//    printf("！！可能原因：SPI Flash初始化不成功。\r\n");
//		while(1);
//  }
//  else
//  {
//    printf("》文件系统挂载成功，可以进行测试\r\n");    
//  }
//  
//  /* FatFs多项功能测试 */
//  res_flash = miscellaneous();

//  
//  printf("\n*************** 文件信息获取测试 **************\r\n");
//  res_flash = file_check();

//  
//  printf("***************** 文件扫描测试 ****************\r\n");
//  strcpy(fpath,"1:");
//  scan_files(fpath);
//  
//  
//	/* 不再使用文件系统，取消挂载文件系统 */
//	f_mount(NULL,"1:",1);
//  
//  /* 操作完成，停机 */
//	while(1)
//	{
//	}
//}

///************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

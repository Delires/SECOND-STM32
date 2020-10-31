#include "fatfuction.h"
#include "string.h" //strlen()函数头文件
/**********************************************************************************************
这个函数写一些FATF的相关函数1.存储介质大小获取  2.文件信息获取  3.文件扫描
这三个函数都是基于FATFS系统的基本函数构造出来的，属于顶层，官网上有参考写法

**********************************************************************************************/
FRESULT res_flash;                /* 文件操作结果返回值 */
FIL fnew;						  /* 文件对象句柄  调用部分函数要用 */
char readbuff[521];      //存储读到的数据
char fpath[100];         //存储文件路径

UINT f_br;    //fopen用的变量

FRESULT miscellaneous(void)
{
	FATFS *fps;
	DWORD fre_clust, fre_sector, tot_sector;
	DIR  dir; //f_opendir函数 文件夹句柄
    
	printf("-----------------------------------------------------------------------------------------------\r\n");
	printf("-------------------- miscellaneous函数效果 ----------------------------------------------------\r\n");
	printf("-----------------------------------------------------------------------------------------------\r\n");
/*************** 读取存储介质大小部分 ***************/
	printf("*********** 存储介质的内存大小 ***************************\r\n");
	
	res_flash = f_getfree("1:", &fre_clust, &fps); //f_getfree函数获取空余空间
	tot_sector = (fps->n_fatent-2) * fps->csize;
	fre_sector = fre_clust * fps->csize;  

    printf(" 文件总空间为%10lu kb \r\n", tot_sector*4);
	printf(" 文件剩余空间为%10lu kb \r\n", fre_sector*4);
	

/*************** 向存储介质写入一个文件 ***************/	
	printf("*********** 文件定位和格式化写入功能测试 *********************\r\n");
	res_flash = f_open( &fnew, "1:ceshi.txt", FA_OPEN_ALWAYS|FA_READ|FA_WRITE); //打开一个文件给各种权限
	if( res_flash == FR_OK ) //打开成功
	{
		res_flash = f_lseek(&fnew, f_size(&fnew) ); //先把光标放到文件的末尾，继续开始写。 fnew文件的光标偏移文件的大小
		if( res_flash == FR_OK ) //打开成功
		{
			f_printf( &fnew,"miscellaneous函数写测试，在文件中写入信息：文件总空间为%lu kb,  文件剩余空间为%lu kb \r\n", tot_sector*4,  fre_sector*4 );//直接向文件写入信息 不用f_write
		}

	    res_flash = f_lseek(&fnew, 0); //写完光标移动到文件开头
		
		res_flash = f_read( &fnew, readbuff, f_size(&fnew), &f_br);
		if( res_flash == FR_OK ) //读取成功
		{
			printf("读到的信息为：%s ", readbuff); //读取信息
		
		}
		f_close(&fnew); //关闭文件
	
	
/*************** 建一个文件夹，并把上面那个文件移进去  并且重命名上面那个文件 ***************/	
		printf("*********** 建立文件夹移动文件 和  重命名文件名 ***********\r\n");
		res_flash = f_opendir( &dir, "1:DirText");
		if(res_flash != FR_OK) //打开失败
		{
			res_flash = f_mkdir("1:DirText"); //建立一个文件夹
		}
		else
		{
			res_flash = f_closedir(&dir); //关闭文件夹
			f_unlink("1:DirText/shi.txt"); //删除ceshi.txt文件
			
		}
		if(res_flash == FR_OK) //建立一个文件夹 成功
		{
			res_flash = f_rename ("1:ceshi.txt", "1:DirText/shi.txt"); //重命名+ 用/移动文件
			if(res_flash == FR_OK) 
			{
				printf(" 重命名+ 用/移动文件 成功 ， FLASH改成U盘看看吧 \r\n");
			}
		}
	}
	else
	{
		printf(" 向存储介质写入文件失败 \r\n");
	}
	
	
	return res_flash;
}









/***********************************************
  文件信息获取函数  调用f_stat()函数
  功能:获取存储器存在的文件或文件夹的信息
  此函数是官网给的FATFS标准例程
***********************************************/
FILINFO fno;
FRESULT file_check(void)
{
	printf("-----------------------------------------------------------------------------------------------\r\n");
	printf("-------------------- miscellaneous函数效果 ----------------------------------------------------\r\n");
	printf("-----------------------------------------------------------------------------------------------\r\n");
	
	res_flash = f_stat ("1:DirText/shi.txt", &fno);
	if( res_flash == FR_OK )
	{
		printf("shi.txt的文件信息:\r\n");
		printf("文件大小为%ld(字节):\r\n",  fno.fsize);
		printf("文件操作时间: %u/%02u/%02u, %02u:%02u\n",
               (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,
               fno.ftime >> 11, fno.ftime >> 5 & 63);
		//有个规定时间从 1980/1/1 00:00开始算  
		
		 printf("Attributes: %c%c%c%c%c%c%c%c      ",           //文件属性
				(fno.fattrib & AM_RDO) ? 'R': '-' ,  //三位运算，如果是AM_DIR类型 就打印 D 不是打印-
				(fno.fattrib & AM_HID) ? 'H': '-' ,
				(fno.fattrib & AM_SYS) ? 'S': '-' , 
				(fno.fattrib & AM_VOL) ? 'V': '-' ,
				(fno.fattrib & AM_LFN) ? 'L': '-' ,
				(fno.fattrib & AM_DIR) ? 'D': '-' ,
				(fno.fattrib & AM_ARC) ? 'A': '-' ,
				(fno.fattrib & AM_MASK) ? 'M': '-'	    
                );	
          printf("文件名缩写字母代表，查询ff.h\r\n");	

          return res_flash;		
	}
	else
	{
		printf(" f_stat()函数读取存储介质里的文件夹失败 \r\n");
		  return res_flash;		
	}
}









/***********************************************
  扫描存储介质的所有文件  调运用 f_readdir()  f_opendir 函数
  功能:获取存储器存在的文件或文件夹的信息
  此函数是官网给的FATFS标准例程
  这是一个递归函数
  ***********************************************/
FRESULT scan_files (char* path)//输入的是一个指针，数组名
{ 
//递归函数必须局部变量，全局不会更改，但由于递归的次数多可能局部变量定义多导致溢出，所以栈空间要改大	
    FRESULT res; 	//存储文件函数操作返回状态
    FILINFO fno;    //f_readdir()函数需要的返回值得结构体
	DIR dir;          //文件夹句柄
	int i;          //存储目录长度
	char *fn;      //文件名
	

#if _USE_LFN 
/* 长文件名支持 */
	static char lfn[_MAX_LFN*2 +1]; //定义变量用于存储长文件名
	fno.lfname = lfn;            //文件名名
	lfn.lfsize = sizeof(lfn);   //文件名长度
#endif	
	
	res = f_opendir(&dir,  path); //f_opendir打开'1:'文件夹 就相当于直接打开存储设备
	if( res == FR_OK)
	{
		i = strlen( path ); //读取目录长度，用于后文拼接打印
		for(; ; ) //空循环
		{
			res = f_readdir(&dir, &fno);
			if( res!=FR_OK || fno.fname[0] == 0) break;
			     //一直循环读，读到读文件失败 或 文件名为空退出
#if _USE_LFN
			fn = *fno.lfname ? fno.lfname : fno.fname //fno.lfname有值fn就指向fno.lfname 否则指向 fno.name 
#else
			fn = fno.fname; //文件名赋值给fn  fno.fname是数组名是指针
#endif
			if( *fn == '.') continue; //如果读到文件就继续，文件名里有'.'
			
			if(fno.fattrib & AM_DIR) //文件属性是文件夹 递归就继续调用
			{
				sprintf(&path[i], "/%s", fn );  // sprintf(char *string, char *format [,argument,...]);把format拼接/替换到string   argument是format的值
			            //如果属性是文件夹  就把当前的文件名存储到path字符数组里
				        //i = strlen( path ); 在path的第几个数组后继续拼接跟着续要打印的值
				res = scan_files( path); //再继续递归
				
				path[i] = 0;  //递归结束把path[i]值赋0
				if(res != FR_OK)
					  break;
			}
			else //如果是文件名 证明读到底了
			{
				printf("%s/%s\r\n", path, fn);
			}
		}
	}	
	return res;	
}










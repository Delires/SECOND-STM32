/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SPI FLASH文件系统常用功能演示
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
#include "./usart/bsp_usart.h"
#include "ff.h"
#include "string.h"
/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
  
  
/**
日志
20200903：参考正点的更改discio的写函数偏移地址 读函数偏移  和  disk_ioctl() FLASH大小差不多正常
                                  有一个问题 写函数之前的擦出 和正点的略有问题，擦除问题？？

**/
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
char fpath[100];                  /* 保存当前扫描的所有路径 */
char readbuffer[512];             //保存读到的数据
 
/**
  ******************************************************************************
  *                                任务函数
  ******************************************************************************
  */
/* FatFs多项功能测试 */
static FRESULT miscellaneous(void)
{
  DIR dir;
  FATFS *pfs;
  DWORD fre_clust, fre_sect, tot_sect;
  
  printf("\n*************** 设备信息获取 ***************\r\n");
  /* 获取设备信息和空簇大小 */
  res_flash = f_getfree("1:", &fre_clust, &pfs);
                            //fre_clust 返回空余簇的个数
  /* 计算得到总的扇区个数和空扇区个数 */
  tot_sect = (pfs->n_fatent - 2) * pfs->csize; //总的扇区数 = 簇个数*每个簇扇区数 
	          //pfs->n_fatent 取结构体的值     pfs->csize取结构体的值
  fre_sect = fre_clust * pfs->csize; //空余的扇区数 = fre_clust 返回空余簇的个数 * 每个簇扇区数

  /* 打印信息(4096 字节/扇区) */
  printf("》设备总空间：%10lu KB。\n》可用空间：  %10lu KB。\n", tot_sect *4, fre_sect *4); //一个扇区4kb 再*4 算出kb
                       //10lu 输出长度为10的 lu无符号长整型
  
	
	
	
  printf("\n******** 文件定位和格式化写入功能测试 ********\r\n");
  res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",
                            FA_OPEN_ALWAYS|FA_WRITE|FA_READ );//给文件权限
	if ( res_flash == FR_OK )
	{
    /*  文件定位 */
    res_flash = f_lseek(&fnew,f_size(&fnew));//先把光标放到文件的末尾，继续开始写。 fnew文件的光标偏移文件的大小
    if (res_flash == FR_OK)
    {
      /* f_printf 格式化写入，参数格式类似printf函数 */
	//f_printf的操作就是类似 f_write 直接把数据打印到文件里
      f_printf(&fnew,"\n在原来文件新添加一行内容\n"); //把内容用f_printf打印到文件夹里
      f_printf(&fnew,"》设备总空间：%10lu KB。\n》可用空间：  %10lu KB。\n", tot_sect *4, fre_sect *4);
      /*  文件定位到文件起始位置 */
      res_flash = f_lseek(&fnew,0); //重新把光标放到文件开头
      /* 读取文件所有内容到缓存区 */
      res_flash = f_read(&fnew,readbuffer,f_size(&fnew),&fnum); //读整个文件的值，readbuffer缓冲区要足够大否则溢出值不对  
		                    //判断文件读取是否完成  btr != br 读的字节数不等于返回字节数  证明读完成了
      if(res_flash == FR_OK)
      {
        printf("》文件内容：\n%s\n",readbuffer);
      }
    }
    f_close(&fnew);    //关闭文件

//读刚才创建的 FatFs读写测试文件.txt 重命名	
    printf("\n********** 目录创建和重命名功能测试 **********\r\n");
    /* 尝试打开目录 */
    res_flash=f_opendir(&dir,"1:TestDir"); //f_opendir打开一个文件夹TestDir
    if(res_flash!=FR_OK) //判断是否打开成功
    {
      /* 打开目录失败，就创建目录 */
      res_flash=f_mkdir("1:TestDir"); //f_mkdir 创建一个文件夹(目录) TestDir
    }
    else
    {
      /* 如果目录已经存在，关闭它 */
      res_flash=f_closedir(&dir);
      /* 删除文件 */
      f_unlink("1:TestDir/testdir.txt"); //f_unlink 把TestDir文件夹里的testdir.txt删除，确保重新进行操作
    }
    if(res_flash==FR_OK)
    {
      /* 重命名并移动文件 */
      res_flash=f_rename("1:FatFs读写测试文件.txt","1:TestDir/testdir.txt");   //f_rename 重命名文件名 并且移动"带路径就自动移动了"   
    } 
	}
  else
  {
    printf("!! 打开文件失败：%d\n",res_flash);
    printf("!! 或许需要再次运行“FatFs移植与读写测试”工程\n");
  }
  return res_flash;
}






  FILINFO fno;
/**
  * 文件信息获取  调用f_stat()函数
  */
static FRESULT file_check(void)
{ 
  /* 获取文件信息 */
  res_flash=f_stat("1:TestDir/testdir.txt",&fno);//获取文件状态，状态信息返回给结构体fno
  if(res_flash==FR_OK)
  {
    printf("“testdir.txt”文件信息：\n");
    printf("》文件大小: %ld(字节)\n", fno.fsize);
    printf("》时间戳: %u/%02u/%02u, %02u:%02u\n",
           (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,fno.ftime >> 11, fno.ftime >> 5 & 63);
    printf("》属性: %c%c%c%c%c\n\n",
	       (fno.fattrib & AM_DIR) ? 'D' : '-',      // 是一个目录   x?x:x三目运算来判断文件的类型
           (fno.fattrib & AM_RDO) ? 'R' : '-',      // 只读文件
           (fno.fattrib & AM_HID) ? 'H' : '-',      // 隐藏文件
           (fno.fattrib & AM_SYS) ? 'S' : '-',      // 系统文件
           (fno.fattrib & AM_ARC) ? 'A' : '-');     // 档案文件
  }
  return res_flash;
}




/**
  * @brief  scan_files 递归扫描FatFs内的文件
  * @param  path:初始扫描路径
  * @retval result:文件系统的返回值   主要运用f_readdir()函数  参考官网的例子
     这是一个递归函数！！！！自己调用自己： 如果读到是文件夹名就打开文件继续递归，如果读到文件就结束递归直接输出
*/
static FRESULT scan_files (char* path) 
{ 
  FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量。递归函数必须局部变量，但由于递归的次数多可能局部变量定义多导致溢出，所以栈空间要改大	
  FILINFO fno;      
  DIR dir;          //包含目录信息的结构体
  int i;            //pathp[i]  
  char *fn;        // 文件名	
	
#if _USE_LFN 
  /* 长文件名支持 */
  /* 简体中文需要2个字节保存一个“字”*/
  static char lfn[_MAX_LFN*2 + 1]; 	//存储长文件名变量
  fno.lfname = lfn; 
  fno.lfsize = sizeof(lfn); 
#endif 
	
	printf("进入递归\r\n");
  //打开目录
  res = f_opendir(&dir, path); //f_opendir 打开文件1：
  if (res == FR_OK) 
	{ 
    i = strlen(path); //算出目录的长度，后面用于拼接字符串 打印函数
    for (;;)//没有限制的循环，不break跳出，就一直循环 
		{ 
      //读取目录下的内容，再读会自动读下一个文件
      res = f_readdir(&dir, &fno); 								
      //为空时表示所有项目读取完毕，跳出
      if (res != FR_OK || fno.fname[0] == 0) break; 	
#if _USE_LFN 
      fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
      fn = fno.fname; 
#endif 
      //点表示当前目录，跳过			
      if (*fn == '.') continue; 	
      //目录，递归读取      
      if (fno.fattrib & AM_DIR) //如果是目录  拼接 继续调用递归      
			{ 			
        //合成完整目录名        
        sprintf(&path[i], "/%s", fn);//sprintf()拼接函数 拼接文件名 		
        //递归遍历         
        res = scan_files(path);	
        path[i] = 0;         
        
        if (res != FR_OK)  //打开失败，跳出循环 来到return 结束递归  
					break; 
      } 
			else //如果不是目录，是文件名的话
			{ 
				printf("%s/%s\r\n", path, fn);	//直接输出打印出来
				//输出文件名	
				printf("进入输出打印\r\n");
        /* 可以在这里提取特定格式的文件路径 */        
      }//else
    } //for
  } 
  return res; 
}






/****************************************************************************************/
/********************************这是主函数***********************************************/
/****************************************************************************************/
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{    	
	/* 初始化调试串口，一般为串口1 */
	USART_Config();	
  printf("******** 这是一个SPI FLASH 文件系统实验 *******\r\n");
  
	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
	res_flash = f_mount(&fs,"1:",1); //一定要先挂载，这样后续才能继续开始
  if(res_flash!=FR_OK)
  {
    printf("！！外部Flash挂载文件系统失败。(%d)\r\n",res_flash);
    printf("！！可能原因：SPI Flash初始化不成功。\r\n");
		while(1);
  }
  else
  {
    printf("》文件系统挂载成功，可以进行测试\r\n");    
  }
  
  /* FatFs多项功能测试 */
  res_flash = miscellaneous();  //miscellaneous 应用层函数，自己写的

  
  printf("\n*************** 文件信息获取测试 **************\r\n");
  res_flash = file_check(); //file_check 应用层函数，自己写的

  //利用上函数file_check得到的文件属性 就可以进行如下的文件扫描
  printf("***************** 文件扫描测试 ****************\r\n");
  strcpy(fpath,"1:"); //strcpy复制字符串，把存储设备1: 的名字复制给fpath
  scan_files(fpath);  //scan_files 应用层函数，自己写的   扫描fpath即扫描1:
  
  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,"1:",1);
  
  /* 操作完成，停机 */
	while(1)
	{
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

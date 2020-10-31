#include "SPIFlash.h"
#include "usart.h"
#include "delay.h"
/**
stm32的硬件SPI的配置
用来读写FLASH 及其其他支持SPI通讯的芯片
要符合符合SPI通信规则
FLsh内部也有很多操作指令 因此要宏定义Flash的操作指令便于操作
编程步骤：
 1.初始化SPI
      1.0初始SPI的GPIO
      1.1配置SPI初始化
 2.根据SPI时序
      2.0使用byte write函数往发Flash写数据
	  2.1使用random read函数读数据+串口打印
 3.宏定义Flash的操作指令，便于读写操作
  
**/

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
//static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode); //超时处理

/*************************** 配置ISPI的GPIO *****************************/
/**
static 函数 只能被本函数调用
查询表 SPI不同的引脚配置mode不同
**/
static void SPI_GPIO_Config(void)
{
//配置三步走：打开时钟，配置，使能功能
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_SPI_APBxClock_Fun(GPIO_SPI_CLK, ENABLE);	

//NSS 软件控制做普通IO 直接推挽(不用复用)	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出(普通的输出 做IO用)
	GPIO_InitStruct.GPIO_Pin = SPI_NSS_CS_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStruct); 
	
//SCK
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //推挽复用输出
	GPIO_InitStruct.GPIO_Pin = SPI_SCK_PIN;
	GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStruct); 
//MISO
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_InitStruct.GPIO_Pin = SPI_MISO_PIN;
	GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStruct);
//MOSI
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //推挽复用输出
	GPIO_InitStruct.GPIO_Pin = SPI_MOSI_PIN;
	GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStruct);
	
	FLASH_SPI_CS_HIGH;  //NSS拉高中止SPI通信
}

/*************************** 配置 SPI的工作模式 *****************************/
/**

**/
static void SPI_Config(void)
{
//配置三步走：打开时钟，配置，使能功能
	SPI_InitTypeDef   SPI_InitStruct;
	SPI_APBxClock_Fun(SPI_CLK, ENABLE);
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; //72M/128=0.5Mhz 9250最高1M
	//模式0 也可以为3(和从设备匹配)
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge; //SPI_CPHA_2Edge;  //CPHA和CPOL结合配置SPI模式(和从设备匹配)
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;  //SPI_CPOL_High;  //网上是SPI 3
	SPI_InitStruct.SPI_CRCPolynomial = 0;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//(和从设备匹配)
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线全双工
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//(和9250设备匹配)
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master ;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft ;  //软件NSS使能 用GPIO
	SPI_Init(SPI_NUMBER, &SPI_InitStruct);  //操作 CR1 registeration
	
	//使能功能
    SPI_Cmd(SPI_NUMBER,ENABLE); 

}

/*************************** SPI初始化 *****************************/
/**
调用GPIO初始化 和 SPI初始化
**/
void SPI_Commit_Init(void)
{
	SPI_GPIO_Config();
	SPI_Config();
}







/*************************** SPI向Flash写1byte数据(由主机写)*****************************/
/**
1.发送Flash读取命令   //SPI是同步接收的，接收了立马就发。所以写和读都在一个函数里
2.按照SPI的通讯协议
3.读+写都在一个函数里，因为写的同时立马就会返回数据
4.必须写了才能读，发送数据之后能产生时钟
返回值为0 则通讯失败
**/
/***************************************************************************/
u8 SPI_FLASH_Send_Byte(u8 data)
{
	u16 value;
	SPITimeout=SPIT_FLAG_TIMEOUT;//linux防止超时
	/** 按照SPI的通讯协议去写 **/
	while( SPI_I2S_GetFlagStatus(SPI_NUMBER,SPI_I2S_FLAG_TXE) == RESET) //发送缓存为空 (根据SPI协议) 死循环等待 
    {
		if( (SPITimeout--) == 0)
		{
			FLASH_DEBUG("SPI 等待超时!errorCode = %d",0);
			return 0;
		}			
		
	}
	SPI_I2S_SendData(SPI_NUMBER,  data); //发送数据
	
	SPITimeout=SPIT_FLAG_TIMEOUT;
	//MOSI MISO 两根线同时进行
	//SPI是同步接收的，接收了立马就发。所以写和读都在一个函数里
	while( SPI_I2S_GetFlagStatus(SPI_NUMBER,SPI_I2S_FLAG_RXNE) == RESET) //接收缓存非空 (根据SPI协议) 死循环等待 
    {
       if( (SPITimeout--) == 0)
	   {
		   FLASH_DEBUG("SPI 等待超时!errorCode = %d",1);
		   return 0;
	   }
		
	}		
	value = SPI_I2S_ReceiveData(SPI_NUMBER);
	return value;
	//return (SPI_I2S_ReceiveData(SPI_NUMBER)) ;  //发送数据

}







/*************************** SPI向Flash读1byte数据*****************************/
/**
1.调用上文 ： SPI向Flash写1byte数据
2.因为只有SPI写了之后才能读，只有写了才能有CLK时钟 所以想读就得写   
3.定义一个DUMMY空指令，这样就不会打断上次的指令还能继续读数据
**/
/***************************************************************************/
u8 SPI_FLASH_Read_Byte(void)
{
	return SPI_FLASH_Send_Byte(DUMMY);//DUMMY空指令，这样就不会打断上次的指令还能继续读数据
}




/*******************************************************************************************************************************/
/*********************************  MPU9250部分  ******************************************************************************/
/*******************************************************************************************************************************/


/*************************** SPI向9250写数据*****************************/
/**
写数据 0+7位寄存器地址 = 寄存器地址
读数据 1+7位寄存器地址 = 寄存器地址|0x80
**/
/***************************************************************************/
u8 MPU9250_Write_Reg(u8 reg,u8 value)
{
 u8 status;
 FLASH_SPI_CS_LOW;
 status=SPI_FLASH_Send_Byte(reg); //发送reg地址
 SPI_FLASH_Send_Byte(value);//发送数据
 FLASH_SPI_CS_HIGH;// MPU9250_CS=1;  //失能MPU9250
 return(status);//
}





/*************************** SPI向9250读数据*****************************/
/**
写数据 0+7位寄存器地址 = 寄存器地址
读数据 1+7位寄存器地址 = 寄存器地址|0x80
**/
/***************************************************************************/
u8 MPU9250_Read_Reg(u8 reg)
{
   u8 reg_val;
   FLASH_SPI_CS_LOW;// MPU9250_CS=0;  //片选MPU9250
   SPI_FLASH_Send_Byte(reg|0x80); //reg地址+读命令
   reg_val=SPI_FLASH_Send_Byte(0xff);//任意数据
   FLASH_SPI_CS_HIGH;// MPU9250_CS=1;  //失能MPU9250
 return(reg_val);
}




/*************************** 初始化MPU9250  *****************************/
/**
写数据 0+7位寄存器地址 = 寄存器地址
读数据 1+7位寄存器地址 = 寄存器地址|0x80
**/
/***************************************************************************************************/
void Init_MPU9250(void)
{ 
	delay_ms(100); //初始化前先延时一段时间 否则重新复位上电会出错
/**配置9250**/
	//Single_Write(MPU9250_ADDR,USER_CTRL, 0x10);   //禁用IIC从模式
	
    MPU9250_Write_Reg(PWR_MGMT_1, 0x80);	//复位MPU9250 重置所有寄存器配置/解除休眠状态
	MPU9250_Write_Reg(PWR_MGMT_1, 0x00); //唤醒MPU9250，并选择陀螺仪x轴PLL为时钟源(自动选择最佳时钟源-否则使用外部振荡)
	                                                     //选啥都一样
	//Single_Write(MPU9250_ADDR_WRITE,PWR_MGMT_1, 0x68); //禁止加速度计
	//Single_Write(MPU9250_ADDR_WRITE,PWR_MGMT_2, 0x07); //禁止陀螺仪
	
	MPU9250_Write_Reg(INT_ENABLE, 0x00); //禁止中断INT
	//Single_Write(MPU9250_ADDR_WRITE,INT_ENABLE, 0x01); //使能数据更新中断INT 每读到新的数据就产生中断INT输出高电平


/**设置 gyro 陀螺仪量程**/	
	MPU9250_Write_Reg(CONFIG, 0x06);        //0x06=0000 0110d 结合G的配置 让G有5Hz带宽的滤波 Fs(Internal_Sample_Rate输出速率)=1Khz
	MPU9250_Write_Reg(SMPLRT_DIV, 0x07);    //用来配置陀螺仪采样频率为(详见手册2.4) 
	         //1000(陀螺仪输出速率)/(1+7)=125Hz (这个1k对应CONFIG寄存器的Fs) Internal_Sample_Rate
    MPU9250_Write_Reg(GYRO_CONFIG, 0x01);   //角速度范围+-2000
	
/**设置  acc 加速度计量程**/
    MPU9250_Write_Reg(ACCEL_CONFIG2, 0x00);   //加速度218.1Hz带宽  输出速率1Khz
    MPU9250_Write_Reg(ACCEL_CONFIG, 0x10); //加速度范围+-8g
  //----------------
//	Single_Write(MPU9250_ADDR_WRITE,0x6A,0x00);//close Master Mode	

/**设置  M 磁力计**/
    MPU9250_Write_Reg(INT_PIN_CFG,0x02);//turn on Bypass Mode 旁路模式 磁力计在里头
	delay_ms(10);                                       //每读一次数据或配置一次就要打开一次旁路模式
    MPU9250_Write_Reg(MAG_CNTL2, 0x01);  //复位磁力计
	MPU9250_Write_Reg(MAG_CNTL1, 0x01);  //设置为单次测量模式，否则读不出值
}








/*************************** 检测9250ID号 发送并接收(两根线同时进行)*****************************/
/**
看《W25Q128手册》P27  JEDEC
1.先把CS拉低发送指令 片选
2.发送相应读ID指令 0x75
3.再发送空指令 保证通讯继续，然后读设备地址  
**/
/***************************************************************************************************/
u8 SPI_FLASH_Read_ID(void)
{
	u8 flash_id;
	
	FLASH_SPI_CS_LOW;//片选
	SPI_FLASH_Send_Byte(WHO_AM_I|0x80); //reg地址+读命令
    flash_id = SPI_FLASH_Send_Byte(DUMMY);//发送DUMMY确保传输继续 读数据
	
	
	FLASH_SPI_CS_HIGH;
	
	return flash_id;
}



/********************************************************************/
/**
检测AK8963的内部磁力及是否存在  AKM的设备ID,它以一个字节和固定值（0x48）进行描述。
磁力计必须以IIC来读取，因为磁力计嵌入在设备中，必须要有芯片地址
**/
/********************************************************************/
bool Check_AK8963_Exist(void)
{
	
}





/*************************** 读加速度值  *****************************/
/**

**/
/***************************************************************************************************/





/*************************** 读罗盘值 *****************************/
/**

**/
/***************************************************************************************************/



////不能正确的打印出行，此函数直接被FLASH_DEBUG("SPI 等待超时!errorCode = %d",0);代替
///*************************** DEBUG部分 *****************************/
///**
//返回spi通讯错误的部分代码
//代码：0    发送一直不为空,上次还未发送完
//代码：1    接收一直为空,没读到数据
//**/
//static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
//{
//  /* 等待超时后的处理,输出错误信息 */
//  FLASH_DEBUG("SPI 等待超时!errorCode = %d",errorCode);
//  return 0;
//}
//   








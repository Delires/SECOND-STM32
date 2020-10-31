#include "SPIFlash.h"
#include "usart.h"
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
	GPIO_SPI_APBxClock_Fun(RCC_APB2Periph_GPIOB, ENABLE);	

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
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //此速度要在从设备接收范围内
	//模式0 也可以为3(和从设备匹配)
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; //SPI_CPHA_2Edge;  //CPHA和CPOL结合配置SPI模式(和从设备匹配)
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;  //SPI_CPOL_High;
	SPI_InitStruct.SPI_CRCPolynomial = 0;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//(和从设备匹配)
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线全双工
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//(和从设备匹配)
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
void SPI_Communit_Init(void)
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







/*************************** 检测ID号 发送并接收(两根线同时进行)*****************************/
/**
看《W25Q128手册》P27  JEDEC
1.先把CS拉低发送指令 片选
2.发送相应读ID指令 0x9F 和 0xAB,0X90 (看P24)
3.①SPI设置的是 SPI_DataSize_8b 一次传8位.而根据W25Q128手册传回来的字节有长有短,要做相应处理
  ②读ID要传回3byte的数据，所以首先要定义一个>3byte的数据存放传回来的值
  ③因为一次通讯只穿8bit，所以要通讯3次才能把数据传完。但想穿必须要先发，因此定义了一个DUMMY,来打开通讯的同时又不影响上次命令
4.>3byte的数据每接收一次SPI就要左移8bit，因为从设备是MSB
**/
/***************************************************************************************************/
u32 SPI_FLASH_Read_ID(void)
{
	u32 flash_id;
	
	FLASH_SPI_CS_LOW;//片选
    SPI_FLASH_Send_Byte(JEDEC_ID);//发送读ID指令
	
//根据SPI协议 读32位数据要左移
//从第二次通讯开始一直发送DUMMY,才开始接收传回的数据(原因看笔记   4.时序问题)
	flash_id = SPI_FLASH_Send_Byte(DUMMY);//发送DUMMY确保传输继续
	flash_id<<=8;
	flash_id |= SPI_FLASH_Send_Byte(DUMMY);
	flash_id<<=8;
	flash_id |= SPI_FLASH_Send_Byte(DUMMY); // |=不影响其他位
	
	FLASH_SPI_CS_HIGH;
	
	return flash_id;
}

/************************** 读取Flash的ID号 发送并接收(两根线同时进行)*****************************/
u32 SPI_FLASH_SHORT_ID(void)
{
	u32 flash_id;
	
	FLASH_SPI_CS_LOW;//片选
	/**按照手册发送指令**/
    SPI_FLASH_Send_Byte(Device_ID);//发送读ID指令
	SPI_FLASH_Send_Byte(DUMMY);
	SPI_FLASH_Send_Byte(DUMMY);
	SPI_FLASH_Send_Byte(DUMMY);
	
//根据SPI协议 读32位数据要左移
//从第二次通讯开始一直发送DUMMY,才开始接收传回的数据(原因看笔记   4.时序问题)
	flash_id = SPI_FLASH_Send_Byte(DUMMY);//发送DUMMY确保传输继续
	flash_id<<=8;
	flash_id |= SPI_FLASH_Send_Byte(DUMMY);

	
	FLASH_SPI_CS_HIGH;
	
	return flash_id;
}







/*************************** 写数据使能 *****************************/
/**
1.发送FLASH相应使能命令即可
2.擦除之前先使能
**/
/***************************************************************************/
void SPI_Flash_Write_Enable(void)
{

	FLASH_SPI_CS_LOW; //片选
 
	SPI_FLASH_Send_Byte(Write_Enable);
	
	FLASH_SPI_CS_HIGH; 
}







/*************************** 等待擦除/写完成*****************************/
/**
1.通过一直循环读取寄存器的位，来判断是否完成
2.一定要循环发送，循环读取
3.Read_Status_Register_1   位0
**/
/***************************************************************************/
void Wait_Erase_Write_End(void)
{
	u8 riges1_state=0;
	SPITimeout=SPIT_FLAG_TIMEOUT;//linux防止超时
	
	FLASH_SPI_CS_LOW; //片选
	SPI_FLASH_Send_Byte(Read_Status_Register_1);//读取标志位 要等待擦出完成	0位为0擦出完成

	do
	{
		riges1_state= SPI_FLASH_Send_Byte(DUMMY);
		if( (SPITimeout--) == 0)
		{
		   FLASH_DEBUG("Sector_Erase ERROR");
		
		}
	}
	while((riges1_state & 0x01) == 1);

	FLASH_SPI_CS_HIGH; 

}







/*************************** 扇区形式擦出FLASH*****************************/
/**
1.必须以扇区的形式来擦除FLASH(Flash的最小单位是扇区，存储是以扇区的形式来存储的)
2.写入之前必须先擦出( 因为FLASH的基本存储是全为1，只能把1变成0,不能变回来)
3.要有相关的擦出命令,有很多种擦出命令
4.根据FLASH协议 三个字节一同组成擦出的 扇区地址 A23-A00
5.擦除需要一定时间，所以要读取标志位 要等待擦出完成
4.擦除之前要写使能
**/
/***************************************************************************/
void Flash_Sector_Erase(u32 address)
{
	SPI_Flash_Write_Enable(); //擦除之前要写使能
	Wait_Erase_Write_End();
	
	FLASH_SPI_CS_LOW; //片选
	SPI_FLASH_Send_Byte(Sector_Erase);//擦除命令
//扇区的地址分割成1byte 分别发送
    //SPI_FLASH_Send_Byte( (u8)(address&0xFF0000) ); //错误写法，u8只会保留低8位
	SPI_FLASH_Send_Byte( (address>>16)&0xFF  ); //A23-A16  (address>>16)&0xFF 
	SPI_FLASH_Send_Byte( (address>>8)&0xFF  ); //A15-A8
    SPI_FLASH_Send_Byte( (address>>0)&0xFF  ); //A7-A0
	
	FLASH_SPI_CS_HIGH; 
	
	Wait_Erase_Write_End();
}







/*************************** 多字节读取FLASH值 *****************************/
/**
1.必须以扇区的形式来擦除FLASH(Flash存储是以扇区的形式来存储的)
2.写入之前必须先擦出( 因为FLASH的基本存储是全为1，只能把1变成0,不能变回来)
3.读取的值多用指针 不用返回值
4.指令+24bit地址+读数据
**/
/***************************************************************************/
void Read_Sector_Flash(u32 address, u8 *readBuffer, u32 numReadByte)
{                                   //读取的值多用指针 不用返回值
	FLASH_SPI_CS_LOW; //片选
	 
	SPI_FLASH_Send_Byte(Read_Data);//FLASH指令
//扇区的地址分割成1byte 分别发送
    //SPI_FLASH_Send_Byte( (u8)(address&0xFF0000) ); //错误写法，u8只会保留低8位
	SPI_FLASH_Send_Byte( (address>>16)&0xFF  ); //A23-A16  (address>>16)&0xFF 
	SPI_FLASH_Send_Byte( (address>>8)&0xFF  ); //A15-A8
    SPI_FLASH_Send_Byte( (address>>0)&0xFF  ); //A7-A0
	
	while(numReadByte--)
	{
		*readBuffer = SPI_FLASH_Send_Byte(DUMMY);
		readBuffer++;
	}
	
	FLASH_SPI_CS_HIGH; 
}







/*************************** 向FLASH写入数据 *****************************/
/**
1.必须以扇区的形式来擦除FLASH(Flash存储是以扇区的形式来存储的)
2.写入之前必须先擦出( 因为FLASH的基本存储是全为1，只能把1变成0,不能变回来)
3.写之前要写使能，等待写结束
4.指令+24bit地址+写数据
5.最多支持一次性写入256个
**/
/***************************************************************************/
u8 Write_Less256byte_Flash(u32 address, u8 *writeBuffer, u32 numWriteByte)
{    
     SPI_Flash_Write_Enable(); //擦除之前要写使能
	 Wait_Erase_Write_End();	
	
     FLASH_SPI_CS_LOW; //片选
  	 SPI_FLASH_Send_Byte(Page_Program); //写命令
	 
	 SPI_FLASH_Send_Byte( (address>>16)&0xFF ); //移位发送地址
	 SPI_FLASH_Send_Byte( (address>>8)&0xFF );
	 SPI_FLASH_Send_Byte( (address>>0)&0xFF );
	
	 if( numWriteByte > ONE_WRIT_MORENUMBER ) //用return 当超出256byte 直接结束
	 {
		  FLASH_DEBUG("SPI write bvte number than 256 byte");
		 return 0;
	 }
	 
	 while(numWriteByte--)
	 {
		SPI_FLASH_Send_Byte( *(writeBuffer) );
		writeBuffer++;
	 }
	 
	 FLASH_SPI_CS_HIGH; 
	 
	 Wait_Erase_Write_End();
	 
	 return 1;
}







/***************************大于256字节的多字节写入*****************************/
/**
1.考虑page对齐问题(256字节要对齐呢)
2.地址问题：(256倍数 和 单个 )每传输256byte，地址就要后移256，再重新调用写256byte
                            256倍数写完剩下的单个的地址也要计算再重新调用
3.字节问题：算出Pagenumber 和 singlenumer 配合地址后移
4.置于地址对齐(Flash好像不存在地址对齐问题)
**/
u8 Write_More_256Byte(u32 address, u8 *writeBuffer, u32 numWriteByte)
{    
	 u8 pageNumer, singleNumer;

	 if( numWriteByte > ONE_WRIT_MORENUMBER ) //用return 当超出256byte 直接结束
	 {
		 pageNumer = numWriteByte/ONE_WRIT_MORENUMBER;
		 singleNumer = numWriteByte%ONE_WRIT_MORENUMBER;
		 
		while(pageNumer --)//每256byte循环一次
		{
			Write_Less256byte_Flash(address, writeBuffer, ONE_WRIT_MORENUMBER);//按页数写
			address+=ONE_WRIT_MORENUMBER;
			writeBuffer+=ONE_WRIT_MORENUMBER;	
		}	
	   	    Write_Less256byte_Flash(address, writeBuffer, singleNumer);//写不足256byte的
		
	 }
	 else //小于256byte
	 {
		    Write_Less256byte_Flash(address, writeBuffer, numWriteByte);
	 }
	 return 1;
}




/********************************野火写入多字节*******************************************/
void Write_More_256Byte_YeHuo(u32 Maddress, u8 *MwriteBuffer, u32 numWriteByte)
{                                  
    u8 numPage=0,   numSingle=0; //计算字节问题
	u8 count=0, Addr=0,  temp=0; //计算地址问题
//地址问题	 
	Addr = Maddress % ONE_WRIT_MORENUMBER; //检测地址是否为256整数倍，首地址是否对齐
	count = ONE_WRIT_MORENUMBER - Addr;  //count=256-地址  一页剩下的字节数
//字节问题
    numPage = numWriteByte/ONE_WRIT_MORENUMBER;
	numSingle = numWriteByte%ONE_WRIT_MORENUMBER;
	
	if(Addr == 0)//地址对齐
	{
		if(numPage == 0)//少于1页 256byte
		{
			Write_Less256byte_Flash(Maddress, MwriteBuffer, numWriteByte);//直接调用
		}
		else //大于1页 256byte
		{
			while( numPage --)
			{
				Write_Less256byte_Flash(Maddress, MwriteBuffer, ONE_WRIT_MORENUMBER);//多次调用256倍数
				Maddress += ONE_WRIT_MORENUMBER;  //地址＋256
				MwriteBuffer += ONE_WRIT_MORENUMBER; // 数组＋256
			}
			
			Write_Less256byte_Flash(Maddress, MwriteBuffer, numSingle);//不足一页的写完
			
		}
	}
/*******地址不对齐的写 三步走******/	
	else  //地址不对齐 地址不是256的整数倍
	{
		if(numPage == 0)//少于1页 
		{
			if(numSingle>count) //单个写的数量 > 地址~256的值
			{
				temp = numSingle-count;
				Write_Less256byte_Flash(Maddress, MwriteBuffer, count); //先写满本页
				Maddress +=count; //数据 地址 递增count
				MwriteBuffer +=count;
				
				Write_Less256byte_Flash(Maddress, MwriteBuffer, temp);//再写完剩下的
			}
			else//单个写的数量 < 地址~256的值  直接写
			{
				Write_Less256byte_Flash(Maddress, MwriteBuffer, numSingle);
			}
		}
		else//大于1页 
		{
			numWriteByte -=count; //把多出的count减去进行处理，剩下的单独处理
			numPage = numWriteByte/ONE_WRIT_MORENUMBER;
   	        numSingle = numWriteByte%ONE_WRIT_MORENUMBER;
			
			Write_Less256byte_Flash(Maddress, MwriteBuffer, count);//先写count
			Maddress+=count;
			MwriteBuffer+=count;
			
			/** 除count外 单独处理，这下就对齐了**/
			while(numPage--)
			{
				Write_Less256byte_Flash(Maddress, MwriteBuffer, ONE_WRIT_MORENUMBER);
			    Maddress+=ONE_WRIT_MORENUMBER;
			    MwriteBuffer+=ONE_WRIT_MORENUMBER;
			}
			if( numSingle != 0 )
			{
				Write_Less256byte_Flash(Maddress, MwriteBuffer, numSingle); //单个的再写完
			}
		}
	}
}





/*************************** FLASH进入低功耗模式 *****************************/
/**
1.根据通讯规则写，写完之后NS要拉高
**/
/***************************************************************************/
void Power_Down_Flash(void)
{                                   //读取的值多用指针 不用返回值
	FLASH_SPI_CS_LOW; //片选
	
	SPI_FLASH_Send_Byte(Power_down); 
	
	FLASH_SPI_CS_HIGH;
	
}







/*************************** FLASH从低功耗模式唤醒出来 *****************************/
/**
1.根据通讯规则写
**/
/***************************************************************************/
void Release_Power_Down_Flash(void)
{                                   //读取的值多用指针 不用返回值
	FLASH_SPI_CS_LOW; //片选
	
	SPI_FLASH_Send_Byte(Release_Powerdown); 
	
	FLASH_SPI_CS_HIGH;
	 	
}


#include "EEpromI2C.h"
#include "usart.h"
/**
stm32的硬件IIC的配置
用来读写EEPROM
符合IIC通信
编程步骤：
 1.初始化
      1.0初始IIC的GPIO
      1.1配置IIC初始化
 2.根据IIC时序
      2.0使用byte write函数往EEPROM写数据
	  2.1使用random read函数读数据+串口打印
 3.升级函数(根据IIC时序)	  
      3.0编写page write
      3.1sequential read
 5.检验以上两个函数	  
	
**/

static __IO u32 TimeOut=I2C_Long_time; //静态 __IO 变量用来计时  这样这个值不会因编译优化 不会因函数调用改变
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode); 
/************ 1.初始化
              1.0初始IIC的GPIO
              1.1配置IIC初始化*******************************/
//有的变量是宏定义 注意
//三大步，使能时钟，配置功能，使能功能
	
static void I2C_EEPROM_config(void)
{
	
	I2C_InitTypeDef I2C_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	EEPROM_I2C_GPIO_APBxClkCmd(EEPROM_I2C_SCL_GPIO_CLK|EEPROM_I2C_SDA_GPIO_CLK, ENABLE); //使能GPIOB
	EEPROM_I2C_APBxClockcmd(EEPROM_I2C_CLK, ENABLE);     //使能I2C1                  

//IIC的SDA和SCL复用开漏 目的输出高阻态	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Pin=EEPROM_I2C_SCL_GPIO_PIN|EEPROM_I2C_SDA_GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStruct);

	
//配置IIC
    I2C_InitStruct.I2C_ClockSpeed = EEPROM_I2C_BAUDRATE;//宏定义配置时钟
	I2C_InitStruct.I2C_Mode =I2C_Mode_I2C;    //I2C模式
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2 ;//CCR寄存器14位占空比
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable; //使能应答
	I2C_InitStruct.I2C_AcknowledgedAddress =I2C_AcknowledgedAddress_7bit; //7位地址模式
	I2C_InitStruct.I2C_OwnAddress1= STM32_I2C_OWM_Address;          //I2C自己的地址
	I2C_Init(EEPROM_I2C, &I2C_InitStruct);
	
//一般配置完外设都有一个使能函数
    I2C_Cmd(EEPROM_I2C, ENABLE);	
}

void I2C_configation(void)
{
	I2C_EEPROM_config();
}


/******************************************************************************************/ 
/**写1byte函数  
			add_word 写地址 
			data   一字节的数据值  
    返回值0 1 2 3是错误信息
**/
/******************************************************************************************/
u32 EEPROM_Byte_Write(u8 add_word, u8 *data)
{
//产生起始信号
	I2C_GenerateSTART(EEPROM_I2C, ENABLE); //产生起始信号d  
	TimeOut=I2C_Short_time;
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT)== ERROR)//死循环一直等检测EV5事件
	{                              //应答不用管,从设备自己会应答，IIC的应答也不用管使能了自动应答
	  if((TimeOut--)==0)
	  {
		  return I2C_TIMEOUT_UserCallback(0);
	  }
	}	
	
//发送从设备地址
	TimeOut=I2C_Short_time;
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Transmitter);//发送8位地址，输入是从设备8位地址
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)== ERROR)//检测EV6事件
	{
	  if((TimeOut--)==0)
	  {
		  return I2C_TIMEOUT_UserCallback(1);
	  }		
	}
	
//发送存储地址
	TimeOut=I2C_Short_time;
	I2C_SendData(EEPROM_I2C,add_word);//发送第一个数据，被当作地址 
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)== ERROR)//检测EV8事件
	{
      if((TimeOut--)==0)
	  {
		  return I2C_TIMEOUT_UserCallback(2);
	  }	
	}
	
//发送数据
	TimeOut=I2C_Short_time;
	I2C_SendData(EEPROM_I2C,*data);//发送第一个数据
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)== ERROR)//检测EV8_2事件	
	{
      if((TimeOut--)==0)
	  {
		  return I2C_TIMEOUT_UserCallback(3);
	  }	
	}
	
	//产生结束信号
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
	return 1;  //成功
}








/******************************************************************************************/ 
/**写1page=8byte 函数 24c02一次性最多写8byte数 
			     add_word    写地址 
			     pBuffer     数组
                 NumberSize  <8
    返回值是4 5 6 7错误信息
**/
/******************************************************************************************/
u32 EEPROM_Page_Write(u8 add_word, u8 *Buff_data, u8 Writernumber)
{
//检测总线
	TimeOut=I2C_Long_time;
	while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY)==SET) //检测到IIC忙
	{
		if( (TimeOut--)==0 )
		{
			return I2C_TIMEOUT_UserCallback(4); //返回错误代码4
		}
		
	}

//产生起始信号	
	TimeOut=I2C_Long_time;
    I2C_GenerateSTART(EEPROM_I2C, ENABLE); //产生起始信号d  
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT)== ERROR)//死循环一直等检测EV5事件
	{
		if( (TimeOut--)==0 )
		{
			return I2C_TIMEOUT_UserCallback(5); //返回错误代码5 产生起始信号失败
		}		
		
	}
	
//发送从设备地址
	TimeOut=I2C_Long_time;
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Transmitter);//发送8位地址，输入是从设备8位地址
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)== ERROR)//检测写的EV6事件
	{
		if( (TimeOut--)==0 )
		{
			return I2C_TIMEOUT_UserCallback(6); //返回错误代码6  发送设备地址失败
		}			
	}
	                            
//发送存储地址
	TimeOut=I2C_Long_time;
	I2C_SendData(EEPROM_I2C,add_word);//发送第一个数据，被当作地址 
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)== ERROR)//检测EV8事件
	{
		if( (TimeOut--)==0 )
		{
			return I2C_TIMEOUT_UserCallback(7); //返回错误代码7  发送设备存储地址失败
		}			
		
	}

//发送数据	
	while(Writernumber)
	{

		I2C_SendData(EEPROM_I2C, *Buff_data);//*取值符
		   
		TimeOut=I2C_Long_time;
		while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)== ERROR)//检测EV8_2事件	
		{
			if( (TimeOut--)==0 )
			{
				return I2C_TIMEOUT_UserCallback(8); //返回错误代码8  page发送数据失败
			}					
		}
		
		Writernumber--;
		Buff_data++;
	}

	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
	return 1; //函数调用成功
	
}

/**********************************************************
地址自动对齐的写函数
EEPROM一次性写8byte的话必须对齐否则出错
该函数是把写的数据分为两种情况
                 1.地址不对齐:①不够8byte的部分----调用EEPROM_Byte_Write()函数
                              ②够8byte的部分----调EEPROM_Page_Write()函数
                              ③余下不够8byte的部分----调EEPROM_Byte_Write()函数
                
                 2.地址对齐:  ①够8byte的部分----调EEPROM_Page_Write()函数
                              ②余下不够8byte的部分----调EEPROM_Byte_Write()函数
输入参数：Buffer_Addr起始地址
          *Buff_data数组指针
          BufferNumber 写入位数<=256
**********************************************************/
u32 Buffer_Write(u8 Buffer_Addr, u8 *Buff_data, u8 BufferNumber)
{
	u8 addr=0,  NumberPage=0,  NumberSing=0,  NumberBegan=0;//变量初始化
//算出起始写的个数	x
	addr = Buffer_Addr%8;
	NumberBegan=8-addr;
	

//算出地址对齐时 8byte页数
	NumberPage=BufferNumber/8;
//算出地址对齐时 多余的地址个数
	NumberSing=BufferNumber%8;
	
/*******地址对齐的写 两步走******/
	if(addr==0)
	{
		if(NumberPage==0)//小于一页
		{
			EEPROM_Page_Write(Buffer_Addr, Buff_data, BufferNumber);
			EEPROM_WaitForWriteEnd();
		}
		else //大于一页
		{
			while(NumberPage--)
			{
				EEPROM_Page_Write(Buffer_Addr, Buff_data, 8);
				Buffer_Addr=Buffer_Addr+8; //地址+8到下一页
				Buff_data=Buff_data+8;//数据+8到下8个数据
				EEPROM_WaitForWriteEnd();
			}
			if(NumberSing!=0)//大于一页 剩下的余数
			{                   //上次page循环地址和数组都自动加到了
				EEPROM_Page_Write(Buffer_Addr, Buff_data, NumberSing);//发送不够8byte的数
				EEPROM_WaitForWriteEnd();	
			}
			
		}
	}
/*******地址不对齐的写 三步走******/	
	else
	{ 
		if(NumberPage==0) //地址不对齐 且<8
		{
			EEPROM_Page_Write(Buffer_Addr, Buff_data, BufferNumber);
			EEPROM_WaitForWriteEnd();
		}
		else //够一页
				 //x 8 8 8 余数 
			{    BufferNumber -= NumberBegan;
			     NumberPage = BufferNumber/8; //算8的倍数
				 NumberSing = BufferNumber%8; //算余数
				
				if(NumberBegan != 0)//写x
				{
					EEPROM_Page_Write(Buffer_Addr, Buff_data, NumberBegan);//写8byte
					EEPROM_WaitForWriteEnd();
					Buffer_Addr+=NumberBegan;
					Buff_data+=NumberBegan;
				}
				while(NumberPage--) //写page
				{
					EEPROM_Page_Write(Buffer_Addr, Buff_data, 8);
					EEPROM_WaitForWriteEnd();
					Buffer_Addr+=8;
					Buff_data+=8;						
				}
				if(NumberSing != 0)//写余数
				{
					EEPROM_Page_Write(Buffer_Addr, Buff_data, NumberSing);//写余数
					EEPROM_WaitForWriteEnd();
				}
				
			}
			
	}
}





/******************************************************************************************/ 
/**读EEPROM函数  
			add_word 读数地址  
			*data 数据存放数组  
			Readnumber读的位数
**/
/******************************************************************************************/ 
void EEPROM_Read(u8 add_word, u8 *data, u8 Readnumber) //读的多个数据必须写成指针否则不能修
{
    //产生起始信号
	I2C_GenerateSTART(EEPROM_I2C, ENABLE); //产生起始信号d  
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT)== ERROR);//死循环一直等检测EV5事件
	
	//发送从设备地址
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Transmitter);//发送8位地址，输入是从设备8位地址
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)== ERROR);//检测EV6事件
	
    //发送存储地址
	I2C_SendData(EEPROM_I2C,add_word);//发送第一个数据，被当作地址 
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)== ERROR);//检测EV8事件
	
	
	//发送第二次起始信号Sr 
	I2C_GenerateSTART(EEPROM_I2C, ENABLE); //产生起始信号d  
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT)== ERROR);//死循环一直等检测EV5事件
	
	//发送从设备地址 读 检测读的EV6事件
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Receiver);//发送8位地址，输入是从设备8位地址
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)== ERROR);//检测读的EV6事件

	while(Readnumber)
	{
		if(Readnumber == 1)
		{
		//读到最后一个字节
		 I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE); //应答不使能 不应答NACK 
		}
	     while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)== ERROR);//检测EV7数据读事件
		 *data=I2C_ReceiveData (EEPROM_I2C);   //读数据 
		
		 data++;//指针+1 即空出8位 1byte
		 Readnumber--;
	}
	 
	//产生结束信号
	 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
	
	 I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);//为不干扰下一次使用 重新使能ACK 
}


//检测写函数是否完成
//给函数重新起始，如果发送检测从设备地址成功，代表上次写字节的过程结束，EEPROM内部逻辑结束
u32 EEPROM_WaitForWriteEnd(void)
{
	TimeOut=I2C_Long_time;
	
	do
	{
		TimeOut--;
		//产生起始信号
		I2C_GenerateSTART(EEPROM_I2C, ENABLE); //产生起始信号 
		while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_SB)==RESET) //如果没产生标志位RESET 一直循环
		{   TimeOut=I2C_Long_time;
			TimeOut--;
			if((TimeOut--)==0)
			{
				 return I2C_TIMEOUT_UserCallback(9);
			}
		}
		
		//发送从设备地址
		I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Transmitter);//发送8位地址，输入是从设备8位地址
		
	}while( (I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_ADDR)==RESET) &&(TimeOut>0) ) ; //没产生标志位RESET 且 TimeOut未减至0
	
		
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
	return 1;
}









/******************************************************************************************/ 
/**检测IIC失败 返回错误值函数
    errorCode 发送1byte错误代码    errorCode=0 start信号错误
                                   errorCode=1 从设备地址发送错误
                                   errorCode=2 数据存储地址 发送错误
                                   errorCode=3 数据 发送错误

               发送page错误代码    errorCode=4 IIC总线忙
                                   errorCode=5 start信号错误
                                   errorCode=6 发送从设备地址错误
                                   errorCode=7 发送从设备存储数据地址错误
                                   errorCode=8 page发送数据失败
								  
			   等待错误代码		   errorCode=9 产生起始信号失败 上次写操作未完成  
			                       errorCode=10 发送地址失败 上次写操作未完成  
               
**/
/******************************************************************************************/ 
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  EEPROM__ERROR("IIC通信失败,错误代码:%d\r\n",errorCode);
  
  return 0;
}








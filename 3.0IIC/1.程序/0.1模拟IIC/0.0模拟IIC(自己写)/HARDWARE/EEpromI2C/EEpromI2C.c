#include "EEpromI2C.h"
#include "IIC_gpio.h"
#include "usart.h"
/**
stm32的硬件IIC的配置
用来读写EEPROM
符合IIC通信
编程步骤：
    1.IIC.gpio函数中已经将起始信号 停止信号 应答等模拟出来了。
	2.此函数就是通过函数将模拟的各种信号组合起来
	3.根据IIC手册写发送字节函数 读取字节函数
**/


/**********************************************************************/
/**
   检测从机是否存在函数
   调用了IIC_Check_Device() 
   存在此函数返回1 不存在返回0
//读数据 SCL时钟先准备好
**/
/**********************************************************************/
u8 ee_CheckOk(void)
{
	if(IIC_Check_Device(EEPROM_Address) == 0)
	{
		return 1;
	}
	else
	{
		IIC_Stop(); //检测失败后 停止IIC
		return 0;
	}
}





/**********************************************************************/
/**
  函数名：ee_ReadBytes
  作用：读EEPROM存储的数据
  形参：Address存储地址   ReadBuf读出的数据存储的函数  Size一次性读多少数据
  返回值：1读取成功       0读取失败
//根据IIC读字节时序 配合起始，停止，ack信号组成这个函数 配合goto语句随时返回错误值
**/
/**********************************************************************/
u8 ee_ReadBytes(u8 SaveAddress, u8 *ReadBuf, u16 Size)  //u16 是因为eeprom一共256个字节 Size最大是256=0x100  Address最大0xFF即255  u8最大255
{                        ////size是255 因为从0开始读 到255一共256个
	u16 i;//要是u8的i 和 u16的size比 
	      //如果u16_size的值比u8_i的最大值255还大，则u8_i
/****以下根据IIC读数据时序图******/
//1.产生起始信号	
	IIC_Start();
//2.发送8位设备地址  写0 
 	IIC_SendByte(EEPROM_Address |IIC_Write);
//3.等待从机应答
	if(IIC_Wait_Ack() != 0)
	{
		EEPROM_DEBUG("Read_DeviceAddressWrit_error");
		goto IIC_fail;
	}
//4.发送存储地址
	IIC_SendByte(SaveAddress);
//5.等待从机应答	
	if(IIC_Wait_Ack() != 0)
	{
		EEPROM_DEBUG("Read_SaveAddres_error");
		goto IIC_fail;
	}
//6.重新发送起始信号
	IIC_Start();
//7.重新发送地址，变成读
	IIC_SendByte(EEPROM_Address |IIC_Read);
//8.等待从机ACK
	if(IIC_Wait_Ack() != 0)
	{
		EEPROM_DEBUG("Read_DeviceAddressRead_error");
		goto IIC_fail;
	}
	
//9.读取数据 IIC不限制一次性读取数据的大小,每读一个数据 主机产生一次应答
	for(i=0; i<Size; i++)
	{
		ReadBuf[i] = IIC_ReadByte();
        
        if(i != (Size-1) ) //不是最后一个 就一直产生应答
		{
           IIC_Ack();
		}	
        else//最后一个 不产生应答
		{
           IIC_Nack(); 
		}			
	}
	IIC_Stop(); //信号间的延时，只用在写完之后读数据时搞个演示，其他的不用管，其他的演示在模拟信号中已经写过
	return 1;  //返回1读取成功
	
	
	IIC_fail:
	IIC_Stop();
	return 0;
}





/**********************************************************************/
/**
  函数名：ee_WriteBytes
  作用：向EEPROM写数据
  形参：Address存储地址   ReadBuf读出的数据存储的函数  Size一次性写多少数据
  返回值：1写数据成功       0写数据失败
//根据IIC写数据的时序 配合起始，停止，ack信号组成这个函数 配合goto语句随时返回错误值
**/
/**********************************************************************/
u8 ee_WriteBytes(u8 SaveAddress, u8 *WriteBuf, u16 Size)
{//eeprom写数据存在对齐不对齐的问题  一次性写>8byte的数据必须对齐否则出错
 //可以 每写一个字节都发送一次地址  这样效率低
 //或   把数据 按照Page_write 一次性8个字节的写
	u16 i, m;  //i是for循环用 m判断上次写操作是否完成
	u8 addr;
	
	addr = SaveAddress;
	for(i=0; i<Size; i++)
	{
		if( (i==0) || (addr & (EEPROM_PAGE_SIZE - 1))==0 )//第一个数 或 首地址 就从新开始信号
		{               //首地址 0 8 16 32.... (EEPROM_PAGE_SIZE-1)=7=111 首地址&111=0
			/**先停止IIC 以防上次的操作没完成，开始准备启动**/
				IIC_Stop();
			
			/**循环查询器件应答，如果有应答证明上次内部写操作完成了**/
			for(m=0; m<1000; m++) //不断循环发送起始信号
			{
			//1.产生起始信号
				IIC_Start(); 
			//2.发送地址
				IIC_SendByte(EEPROM_Address | IIC_Write); 
			//3.判断从设备是否有应答
				if(IIC_Wait_Ack()==0 ) 
				{
					break; //跳出for循环
				}
			}
			if(m == 1000 )
			{
				EEPROM_DEBUG("Write_DeviceAddress_error");
				goto IIC_fail; //GOTO语句结束
			}
			//4.从设备有应答 发送地址
			IIC_SendByte(addr);//地址最大0xFF=255 刚好u8
			                 //IIC_SendByte(SaveAddr);写成SaveAddr则关于addr的判断就无效了。所以会出错
			//5.等待从设备应答
			if(IIC_Wait_Ack()!=0 ) 
			{
				EEPROM_DEBUG("Write_SaveAddress_error");
				goto IIC_fail;
			}
	    }
		//6.循环发送数据 ，首地址的话
		IIC_SendByte(WriteBuf[i]); 
		//7.等待从设备的ack
		if(IIC_Wait_Ack()!=0 ) 
		{
			EEPROM_DEBUG("Write_Data_error");
			goto IIC_fail;
		}	
      	addr++;	
   }
	
   IIC_Stop();
   return 1;
   
IIC_fail:
  IIC_Stop();
  return 0;	
}



/**********************************************************************/
/**
  函数名：ee_Delay
  作用：延时函数
  形参：nCount 延时值32位
  一般采用这种延时 采用滴答定时器容易打断IIC通信
**/
/**********************************************************************/
void ee_Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}



#include "IIC_gpio.h"
/****************************
原理：  两根GPIO口模拟SDA SCL 产生起始信号，停止信号，ACK等信号。硬件IIC由于复用还有其他功能，会出现各种问题，所以多软件IIC。
二者区别：软件IIC没有各种检测位，只能简单的控制IO电平模拟IIC来读取。
          由于高低电平的变化会让GPIO的IDR寄存器的读取值不，通过读取IDR寄存器来读取值。

此函数写 ①配置GPIO 随便两个引脚 开漏输出
         ②根据IIC的时序图,产生起始信号，停止信号，ACK等信号
		 ③主机读/取 改变GPIO的输入/输出模式
		 ④ACK:主机应答模拟ACK信号，从机应答释放SDA线即可

*****************************/

/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

static void i2c_Delay(void)//经验值
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/                          //SCL最快400KHZ
	for (i = 0; i < 10; i++);
}



/**********************************************************************/
/**配置GPIO口----开漏输出GPIO_Mode_Out_OD  配置三法宝：时钟 配置 使能
**/
/**********************************************************************/
void IIC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD; //开漏� 涑�
	GPIO_InitStruct.GPIO_Pin = II2_SDA_PIN|II2_SCL_PIN;  //PB6-SCL PB7-SDA    
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStruct);
	
	GPIO_SetBits(IIC_PORT, II2_SDA_PIN|II2_SCL_PIN); //高电平 全部释放 iic低电平拿事
}


//发送数据(数据先准备好) 
//读数据(时钟先准备好) 
/**********************************************************************/
/**产生start起始信号                                                 
   SCL=1 时 SDA:1--0
   配置两个宏 直接寄存器控制IO电平
**/
/**********************************************************************/
void IIC_Start(void)
{//发送数据(数据先准备好)
	SDA_OUT();
	
	IIC_Analog_SDA_1();
	IIC_Analog_SCL_1();
	i2c_Delay();
	IIC_Analog_SDA_0();
	i2c_Delay();
	IIC_Analog_SCL_0();
	i2c_Delay();
}



/**********************************************************************/
/**产生Stop起始信号                                                 
   SCL=1 时 SDA:0--1
   配置两个宏 直接寄存器控制IO电平
//发送数据(数据先准备好) 
**/
/**********************************************************************/
void IIC_Stop(void)
{//发送数据(数据先准备好)
	SDA_OUT();
	
	IIC_Analog_SDA_0();
	IIC_Analog_SCL_1();
	i2c_Delay();
	IIC_Analog_SDA_1();
	//i2c_Delay();
}



/**********************************************************************/
/**
   IIC发送1byte的8bit数据
   参数：Byte一字节数据
//发送数据(数据先准备好)
**/
/**********************************************************************/
void IIC_SendByte(u8 _ucByte)
{//发送数据(数据先准备好)
	u8 i;
	//SDA_OUT();
	for(i=0; i<8; i++)
	{
		if(_ucByte & 0x80) //0x80=1000 0000 取最高位
		{
			IIC_Analog_SDA_1(); //最高位是1 SDA发高电平
		}
		else
		{
			IIC_Analog_SDA_0();
		}
		i2c_Delay(); //数据先准备好
		IIC_Analog_SCL_1();//在设置SCL
		i2c_Delay();
		IIC_Analog_SCL_0();
		if(i==7)
		{
			IIC_Analog_SDA_1();//释放SDA线，此时SCL=0数据无效
		}
		_ucByte<<=1;// <<按位操作
		i2c_Delay();
	}
}



/**********************************************************************/
/**
   IIC读1byte的8bit数据
   参数：void
   f返回 value
//读数据 SCL时钟先准备好
**/
/**********************************************************************/
u8 IIC_ReadByte(void)
{//发送数据(数据先准备好)
	u8 i, value;
	//SDA_IN();
	value=0;
	for(i=0; i<8; i++)
	{
		value <<=1; //MSB高位先 //①试着放后面看影响不
		IIC_Analog_SCL_1();//读数据 SCL时钟先准备好
		i2c_Delay();
		if(READ_SDA()==1)
		{
			value++;
		}
		IIC_Analog_SCL_0(); //因为是读数据 主机无法控制SDA 更谈不上释放总线
		i2c_Delay();
	}
	return value;
}



/**********************************************************************/
/**
   主机等待应答函数
   释放SDA总线 变成读 看SDA是否为0 0则应答成功 还要注意数据有效性
   返回值ack=0 从机应答成功
//读数据 SCL时钟先准备好
**/
/**********************************************************************/
u8 IIC_Wait_Ack(void)
{//发送数据(数据先准备好)
	u8 ack, Timeout=0;//超时处理
	//SDA_IN();
	
	IIC_Analog_SDA_1();//释放SDA线
	i2c_Delay();
	IIC_Analog_SCL_1(); //读数据 时钟先准备好
	i2c_Delay();
	if(READ_SDA()==1)
	{
		ack=1;
		Timeout++;     //超时处理
		if(Timeout>250)
		{
			ack=1;
			IIC_Stop();
		}
	}
	else
	{
	   ack=0;
	}
	IIC_Analog_SCL_0();
	i2c_Delay();
	return ack;
}



/**********************************************************************/
/**
   主机产生一个应答 ACK=0
//写数据 SDA先准备好
**/
/**********************************************************************/
void IIC_Ack(void)
{//写数据 SDA先准备好
	//SDA_OUT();
	
	IIC_Analog_SDA_0();
	i2c_Delay();
	IIC_Analog_SCL_1();
	i2c_Delay();
	IIC_Analog_SCL_0();
	i2c_Delay();	
	
	IIC_Analog_SDA_1();//释放SDA总线
}



/**********************************************************************/
/**
   主机产生一个应答 NACK=0
//写数据 SDA先准备好
**/
/**********************************************************************/
void IIC_Nack(void)
{//写数据 SDA先准备好
	//SDA_OUT();
	
	IIC_Analog_SDA_1();
	i2c_Delay();
	IIC_Analog_SCL_1();
	i2c_Delay();
	IIC_Analog_SCL_0();
	i2c_Delay();	
	
	//不用释放SDA总线 本来就是1
}



/**********************************************************************/
/**
   检测设备是否存在
   发送给8bit的设备从地址(不管发送啥 只要收到从设备都会回应一个ack) 等待应答  如果有应答返回0 代表设备存在可连接
//写数据 SDA先准备好
**/
/**********************************************************************/
u8 IIC_Check_Device(u8 addr)
{//写数据 SDA先准备好
	u8 cAck;
	
	IIC_GPIO_Config();//配置
	
	IIC_Start();

	IIC_SendByte(addr|IIC_Write); //发送7位地址加写
	cAck=IIC_Wait_Ack();
	
	IIC_Stop();
	
	return cAck;
}






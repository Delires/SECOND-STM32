#include "EEpromI2C.h"
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

/************ 1.初始化
              1.0初始IIC的GPIO
              1.1配置IIC初始化*******************************/
//有的变量是宏定义 注意
//三大步，使能时钟，配置功能，使能功能
void I2C_EEPROM_config(void)
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




/************  2.根据IIC时序
               2.0使用byte write函数往EEPROM写数据
	           2.1使用random read函数读数据+串口打印*******************************/
//写一个字节，由stm32的I2C发送时序图编
void EEPROM_Byte_Write(u8 add_word, u8 data)
{
	//产生起始信号
	I2C_GenerateSTART(EEPROM_I2C, ENABLE); //产生起始信号d  
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT)== ERROR);//死循环一直等检测EV5事件
	                  //应答不用管,从设备自己会应答，IIC的应答也不用管使能了自动应答
	//发送从设备地址
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Transmitter);//发送8位地址，输入是从设备8位地址
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)== ERROR);//检测EV6事件
	
	//发送存储地址
	I2C_SendData(EEPROM_I2C,add_word);//发送第一个数据，被当作地址 
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)== ERROR);//检测EV8事件
	
	//发送数据
	I2C_SendData(EEPROM_I2C,data);//发送第一个数据，被当作地址 
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)== ERROR);//检测EV8_2事件	
	
	//产生结束信号
	 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}



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
void EEPROM_WaitForWriteEnd(void)
{
	do
	{
		//产生起始信号
		I2C_GenerateSTART(EEPROM_I2C, ENABLE); //产生起始信号 
		while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_SB)==RESET); //如果没产生标志位RESET 一直循环
		
		//发送从设备地址
		I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Transmitter);//发送8位地址，输入是从设备8位地址
	}while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_ADDR)==RESET); //没产生标志位RESET 一直循环
	
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}





/************  3.升级函数(根据IIC时序)	  
               3.0编写page write
               3.1sequential read*******************************/
//EEPROM写多个字节，2Kflash每次最多写8个字节
void EEPROM_8Byte_Write(u8 add_word, u8 *data, u8 Writernumber)
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
	
	while(Writernumber)
	{

		I2C_SendData(EEPROM_I2C, *data);//*取值符
		if(Writernumber==1)
		{
			while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)== ERROR);//检测EV8_2事件	
		}
		if(Writernumber!=1)
		{
			while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)== ERROR);//检测EV8事件
		}
		Writernumber--;
		data++;
		
	}

	 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}













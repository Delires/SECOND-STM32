#ifndef __IIC_gpio_H
#define __IIC_gpio_H	 
#include "sys.h"

/****************IIC 读1 写0****************/
#define    IIC_Write                0
#define    IIC_Read                 1

/****************宏定义引脚****************/
#define    IIC_PORT               GPIOB
#define    II2_SCL_PIN            GPIO_Pin_6
#define    II2_SDA_PIN            GPIO_Pin_7



/********用寄存器配置两个宏 直接控制IO电平***/
#define    IIC_Analog_SCL_1()    GPIO_SetBits(IIC_PORT, II2_SCL_PIN)  //SCL PA6
#define    IIC_Analog_SCL_0()    GPIO_ResetBits(IIC_PORT, II2_SCL_PIN) 
 
#define    IIC_Analog_SDA_1()    GPIO_SetBits(IIC_PORT, II2_SDA_PIN)  //SDA PA7
#define    IIC_Analog_SDA_0()    GPIO_ResetBits(IIC_PORT, II2_SDA_PIN)


/*******************配置SDA****************/
                            //&=清除 高四位为0 其他是F不影响其他位    |=赋值 用左移不影响其他位
#define    SDA_IN()       {IIC_PORT->CRL&=0x0FFFFFFF; IIC_PORT->CRL|=(u32)8<<28;}  //PA7 上拉输入10输入00   1000=8
#define    SDA_OUT()      {IIC_PORT->CRL&=0x0FFFFFFF; IIC_PORT->CRL|=(u32)7<<28;}  //PA7 开漏01输出50mhz11  0111=7
#define    READ_SDA()     GPIO_ReadInputDataBit(IIC_PORT, II2_SDA_PIN)   //读PA7输入值 返回1或0
 



/**********模拟IIC的函数*************************/
void IIC_GPIO_Config(void); //配置IIC引脚
void IIC_Start(void);  //起始信号
void IIC_Stop(void);
void IIC_SendByte(u8 Byte);  //发送一字节数据
u8 IIC_ReadByte(void); //读一字节数 返回值是读的值
u8 IIC_Wait_Ack(void); //主机等待从机应答
void IIC_Ack(void);  //主机产生一个ack
void IIC_Nack(void); //主机不产生应答

u8 IIC_Check_Device(u8 addr); //检测设备存在


#endif


#ifndef __IIC_gpio_H
#define __IIC_gpio_H	 
#include "sys.h"

/****************IIC ��1 д0****************/
#define    IIC_Write                0
#define    IIC_Read                 1

/****************�궨������****************/
#define    IIC_PORT               GPIOB
#define    II2_SCL_PIN            GPIO_Pin_6
#define    II2_SDA_PIN            GPIO_Pin_7



/********�üĴ������������� ֱ�ӿ���IO��ƽ***/
#define    IIC_Analog_SCL_1()    GPIO_SetBits(IIC_PORT, II2_SCL_PIN)  //SCL PA6
#define    IIC_Analog_SCL_0()    GPIO_ResetBits(IIC_PORT, II2_SCL_PIN) 
 
#define    IIC_Analog_SDA_1()    GPIO_SetBits(IIC_PORT, II2_SDA_PIN)  //SDA PA7
#define    IIC_Analog_SDA_0()    GPIO_ResetBits(IIC_PORT, II2_SDA_PIN)


/*******************����SDA****************/
                            //&=��� ����λΪ0 ������F��Ӱ������λ    |=��ֵ �����Ʋ�Ӱ������λ
#define    SDA_IN()       {IIC_PORT->CRL&=0x0FFFFFFF; IIC_PORT->CRL|=(u32)8<<28;}  //PA7 ��������10����00   1000=8
#define    SDA_OUT()      {IIC_PORT->CRL&=0x0FFFFFFF; IIC_PORT->CRL|=(u32)7<<28;}  //PA7 ��©01���50mhz11  0111=7
#define    READ_SDA()     GPIO_ReadInputDataBit(IIC_PORT, II2_SDA_PIN)   //��PA7����ֵ ����1��0
 



/**********ģ��IIC�ĺ���*************************/
void IIC_GPIO_Config(void); //����IIC����
void IIC_Start(void);  //��ʼ�ź�
void IIC_Stop(void);
void IIC_SendByte(u8 Byte);  //����һ�ֽ�����
u8 IIC_ReadByte(void); //��һ�ֽ��� ����ֵ�Ƕ���ֵ
u8 IIC_Wait_Ack(void); //�����ȴ��ӻ�Ӧ��
void IIC_Ack(void);  //��������һ��ack
void IIC_Nack(void); //����������Ӧ��

u8 IIC_Check_Device(u8 addr); //����豸����


#endif


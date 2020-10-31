#include "EEpromI2C.h"
#include "IIC_gpio.h"
#include "usart.h"
/**
stm32��Ӳ��IIC������
������дEEPROM
����IICͨ��
��̲��裺
    1.IIC.gpio�������Ѿ�����ʼ�ź� ֹͣ�ź� Ӧ���ģ������ˡ�
	2.�˺�������ͨ��������ģ��ĸ����ź��������
	3.����IIC�ֲ�д�����ֽں��� ��ȡ�ֽں���
**/


/**********************************************************************/
/**
   ���ӻ��Ƿ���ں���
   ������IIC_Check_Device() 
   ���ڴ˺�������1 �����ڷ���0
//������ SCLʱ����׼����
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
		IIC_Stop(); //���ʧ�ܺ� ֹͣIIC
		return 0;
	}
}





/**********************************************************************/
/**
  ��������ee_ReadBytes
  ���ã���EEPROM�洢������
  �βΣ�Address�洢��ַ   ReadBuf���������ݴ洢�ĺ���  Sizeһ���Զ���������
  ����ֵ��1��ȡ�ɹ�       0��ȡʧ��
//����IIC���ֽ�ʱ�� �����ʼ��ֹͣ��ack�ź����������� ���goto�����ʱ���ش���ֵ
**/
/**********************************************************************/
u8 ee_ReadBytes(u8 SaveAddress, u8 *ReadBuf, u16 Size)  //u16 ����Ϊeepromһ��256���ֽ� Size�����256=0x100  Address���0xFF��255  u8���255
{                        ////size��255 ��Ϊ��0��ʼ�� ��255һ��256��
	u16 i;//Ҫ��u8��i �� u16��size�� 
	      //���u16_size��ֵ��u8_i�����ֵ255������u8_i
/****���¸���IIC������ʱ��ͼ******/
//1.������ʼ�ź�	
	IIC_Start();
//2.����8λ�豸��ַ  д0 
 	IIC_SendByte(EEPROM_Address |IIC_Write);
//3.�ȴ��ӻ�Ӧ��
	if(IIC_Wait_Ack() != 0)
	{
		EEPROM_DEBUG("Read_DeviceAddressWrit_error");
		goto IIC_fail;
	}
//4.���ʹ洢��ַ
	IIC_SendByte(SaveAddress);
//5.�ȴ��ӻ�Ӧ��	
	if(IIC_Wait_Ack() != 0)
	{
		EEPROM_DEBUG("Read_SaveAddres_error");
		goto IIC_fail;
	}
//6.���·�����ʼ�ź�
	IIC_Start();
//7.���·��͵�ַ����ɶ�
	IIC_SendByte(EEPROM_Address |IIC_Read);
//8.�ȴ��ӻ�ACK
	if(IIC_Wait_Ack() != 0)
	{
		EEPROM_DEBUG("Read_DeviceAddressRead_error");
		goto IIC_fail;
	}
	
//9.��ȡ���� IIC������һ���Զ�ȡ���ݵĴ�С,ÿ��һ������ ��������һ��Ӧ��
	for(i=0; i<Size; i++)
	{
		ReadBuf[i] = IIC_ReadByte();
        
        if(i != (Size-1) ) //�������һ�� ��һֱ����Ӧ��
		{
           IIC_Ack();
		}	
        else//���һ�� ������Ӧ��
		{
           IIC_Nack(); 
		}			
	}
	IIC_Stop(); //�źż����ʱ��ֻ����д��֮�������ʱ�����ʾ�������Ĳ��ùܣ���������ʾ��ģ���ź����Ѿ�д��
	return 1;  //����1��ȡ�ɹ�
	
	
	IIC_fail:
	IIC_Stop();
	return 0;
}





/**********************************************************************/
/**
  ��������ee_WriteBytes
  ���ã���EEPROMд����
  �βΣ�Address�洢��ַ   ReadBuf���������ݴ洢�ĺ���  Sizeһ����д��������
  ����ֵ��1д���ݳɹ�       0д����ʧ��
//����IICд���ݵ�ʱ�� �����ʼ��ֹͣ��ack�ź����������� ���goto�����ʱ���ش���ֵ
**/
/**********************************************************************/
u8 ee_WriteBytes(u8 SaveAddress, u8 *WriteBuf, u16 Size)
{//eepromд���ݴ��ڶ��벻���������  һ����д>8byte�����ݱ������������
 //���� ÿдһ���ֽڶ�����һ�ε�ַ  ����Ч�ʵ�
 //��   ������ ����Page_write һ����8���ֽڵ�д
	u16 i, m;  //i��forѭ���� m�ж��ϴ�д�����Ƿ����
	u8 addr;
	
	addr = SaveAddress;
	for(i=0; i<Size; i++)
	{
		if( (i==0) || (addr & (EEPROM_PAGE_SIZE - 1))==0 )//��һ���� �� �׵�ַ �ʹ��¿�ʼ�ź�
		{               //�׵�ַ 0 8 16 32.... (EEPROM_PAGE_SIZE-1)=7=111 �׵�ַ&111=0
			/**��ֹͣIIC �Է��ϴεĲ���û��ɣ���ʼ׼������**/
				IIC_Stop();
			
			/**ѭ����ѯ����Ӧ�������Ӧ��֤���ϴ��ڲ�д���������**/
			for(m=0; m<1000; m++) //����ѭ��������ʼ�ź�
			{
			//1.������ʼ�ź�
				IIC_Start(); 
			//2.���͵�ַ
				IIC_SendByte(EEPROM_Address | IIC_Write); 
			//3.�жϴ��豸�Ƿ���Ӧ��
				if(IIC_Wait_Ack()==0 ) 
				{
					break; //����forѭ��
				}
			}
			if(m == 1000 )
			{
				EEPROM_DEBUG("Write_DeviceAddress_error");
				goto IIC_fail; //GOTO������
			}
			//4.���豸��Ӧ�� ���͵�ַ
			IIC_SendByte(addr);//��ַ���0xFF=255 �պ�u8
			                 //IIC_SendByte(SaveAddr);д��SaveAddr�����addr���жϾ���Ч�ˡ����Ի����
			//5.�ȴ����豸Ӧ��
			if(IIC_Wait_Ack()!=0 ) 
			{
				EEPROM_DEBUG("Write_SaveAddress_error");
				goto IIC_fail;
			}
	    }
		//6.ѭ���������� ���׵�ַ�Ļ�
		IIC_SendByte(WriteBuf[i]); 
		//7.�ȴ����豸��ack
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
  ��������ee_Delay
  ���ã���ʱ����
  �βΣ�nCount ��ʱֵ32λ
  һ�����������ʱ ���õδ�ʱ�����״��IICͨ��
**/
/**********************************************************************/
void ee_Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}



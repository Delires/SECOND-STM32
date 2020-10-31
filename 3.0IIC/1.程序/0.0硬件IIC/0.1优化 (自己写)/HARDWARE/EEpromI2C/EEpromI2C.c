#include "EEpromI2C.h"
#include "usart.h"
/**
stm32��Ӳ��IIC������
������дEEPROM
����IICͨ��
��̲��裺
 1.��ʼ��
      1.0��ʼIIC��GPIO
      1.1����IIC��ʼ��
 2.����IICʱ��
      2.0ʹ��byte write������EEPROMд����
	  2.1ʹ��random read����������+���ڴ�ӡ
 3.��������(����IICʱ��)	  
      3.0��дpage write
      3.1sequential read
 5.����������������	  
	
**/

static __IO u32 TimeOut=I2C_Long_time; //��̬ __IO ����������ʱ  �������ֵ����������Ż� �����������øı�
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode); 
/************ 1.��ʼ��
              1.0��ʼIIC��GPIO
              1.1����IIC��ʼ��*******************************/
//�еı����Ǻ궨�� ע��
//���󲽣�ʹ��ʱ�ӣ����ù��ܣ�ʹ�ܹ���
	
static void I2C_EEPROM_config(void)
{
	
	I2C_InitTypeDef I2C_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	EEPROM_I2C_GPIO_APBxClkCmd(EEPROM_I2C_SCL_GPIO_CLK|EEPROM_I2C_SDA_GPIO_CLK, ENABLE); //ʹ��GPIOB
	EEPROM_I2C_APBxClockcmd(EEPROM_I2C_CLK, ENABLE);     //ʹ��I2C1                  

//IIC��SDA��SCL���ÿ�© Ŀ���������̬	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Pin=EEPROM_I2C_SCL_GPIO_PIN|EEPROM_I2C_SDA_GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStruct);

	
//����IIC
    I2C_InitStruct.I2C_ClockSpeed = EEPROM_I2C_BAUDRATE;//�궨������ʱ��
	I2C_InitStruct.I2C_Mode =I2C_Mode_I2C;    //I2Cģʽ
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2 ;//CCR�Ĵ���14λռ�ձ�
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable; //ʹ��Ӧ��
	I2C_InitStruct.I2C_AcknowledgedAddress =I2C_AcknowledgedAddress_7bit; //7λ��ַģʽ
	I2C_InitStruct.I2C_OwnAddress1= STM32_I2C_OWM_Address;          //I2C�Լ��ĵ�ַ
	I2C_Init(EEPROM_I2C, &I2C_InitStruct);
	
//һ�����������趼��һ��ʹ�ܺ���
    I2C_Cmd(EEPROM_I2C, ENABLE);	
}

void I2C_configation(void)
{
	I2C_EEPROM_config();
}


/******************************************************************************************/ 
/**д1byte����  
			add_word д��ַ 
			data   һ�ֽڵ�����ֵ  
    ����ֵ0 1 2 3�Ǵ�����Ϣ
**/
/******************************************************************************************/
u32 EEPROM_Byte_Write(u8 add_word, u8 *data)
{
//������ʼ�ź�
	I2C_GenerateSTART(EEPROM_I2C, ENABLE); //������ʼ�ź�d  
	TimeOut=I2C_Short_time;
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT)== ERROR)//��ѭ��һֱ�ȼ��EV5�¼�
	{                              //Ӧ���ù�,���豸�Լ���Ӧ��IIC��Ӧ��Ҳ���ù�ʹ�����Զ�Ӧ��
	  if((TimeOut--)==0)
	  {
		  return I2C_TIMEOUT_UserCallback(0);
	  }
	}	
	
//���ʹ��豸��ַ
	TimeOut=I2C_Short_time;
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Transmitter);//����8λ��ַ�������Ǵ��豸8λ��ַ
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)== ERROR)//���EV6�¼�
	{
	  if((TimeOut--)==0)
	  {
		  return I2C_TIMEOUT_UserCallback(1);
	  }		
	}
	
//���ʹ洢��ַ
	TimeOut=I2C_Short_time;
	I2C_SendData(EEPROM_I2C,add_word);//���͵�һ�����ݣ���������ַ 
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)== ERROR)//���EV8�¼�
	{
      if((TimeOut--)==0)
	  {
		  return I2C_TIMEOUT_UserCallback(2);
	  }	
	}
	
//��������
	TimeOut=I2C_Short_time;
	I2C_SendData(EEPROM_I2C,*data);//���͵�һ������
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)== ERROR)//���EV8_2�¼�	
	{
      if((TimeOut--)==0)
	  {
		  return I2C_TIMEOUT_UserCallback(3);
	  }	
	}
	
	//���������ź�
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
	return 1;  //�ɹ�
}








/******************************************************************************************/ 
/**д1page=8byte ���� 24c02һ�������д8byte�� 
			     add_word    д��ַ 
			     pBuffer     ����
                 NumberSize  <8
    ����ֵ��4 5 6 7������Ϣ
**/
/******************************************************************************************/
u32 EEPROM_Page_Write(u8 add_word, u8 *Buff_data, u8 Writernumber)
{
//�������
	TimeOut=I2C_Long_time;
	while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY)==SET) //��⵽IICæ
	{
		if( (TimeOut--)==0 )
		{
			return I2C_TIMEOUT_UserCallback(4); //���ش������4
		}
		
	}

//������ʼ�ź�	
	TimeOut=I2C_Long_time;
    I2C_GenerateSTART(EEPROM_I2C, ENABLE); //������ʼ�ź�d  
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT)== ERROR)//��ѭ��һֱ�ȼ��EV5�¼�
	{
		if( (TimeOut--)==0 )
		{
			return I2C_TIMEOUT_UserCallback(5); //���ش������5 ������ʼ�ź�ʧ��
		}		
		
	}
	
//���ʹ��豸��ַ
	TimeOut=I2C_Long_time;
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Transmitter);//����8λ��ַ�������Ǵ��豸8λ��ַ
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)== ERROR)//���д��EV6�¼�
	{
		if( (TimeOut--)==0 )
		{
			return I2C_TIMEOUT_UserCallback(6); //���ش������6  �����豸��ַʧ��
		}			
	}
	                            
//���ʹ洢��ַ
	TimeOut=I2C_Long_time;
	I2C_SendData(EEPROM_I2C,add_word);//���͵�һ�����ݣ���������ַ 
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)== ERROR)//���EV8�¼�
	{
		if( (TimeOut--)==0 )
		{
			return I2C_TIMEOUT_UserCallback(7); //���ش������7  �����豸�洢��ַʧ��
		}			
		
	}

//��������	
	while(Writernumber)
	{

		I2C_SendData(EEPROM_I2C, *Buff_data);//*ȡֵ��
		   
		TimeOut=I2C_Long_time;
		while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)== ERROR)//���EV8_2�¼�	
		{
			if( (TimeOut--)==0 )
			{
				return I2C_TIMEOUT_UserCallback(8); //���ش������8  page��������ʧ��
			}					
		}
		
		Writernumber--;
		Buff_data++;
	}

	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
	return 1; //�������óɹ�
	
}

/**********************************************************
��ַ�Զ������д����
EEPROMһ����д8byte�Ļ��������������
�ú����ǰ�д�����ݷ�Ϊ�������
                 1.��ַ������:�ٲ���8byte�Ĳ���----����EEPROM_Byte_Write()����
                              �ڹ�8byte�Ĳ���----��EEPROM_Page_Write()����
                              �����²���8byte�Ĳ���----��EEPROM_Byte_Write()����
                
                 2.��ַ����:  �ٹ�8byte�Ĳ���----��EEPROM_Page_Write()����
                              �����²���8byte�Ĳ���----��EEPROM_Byte_Write()����
���������Buffer_Addr��ʼ��ַ
          *Buff_data����ָ��
          BufferNumber д��λ��<=256
**********************************************************/
u32 Buffer_Write(u8 Buffer_Addr, u8 *Buff_data, u8 BufferNumber)
{
	u8 addr=0,  NumberPage=0,  NumberSing=0,  NumberBegan=0;//������ʼ��
//�����ʼд�ĸ���	x
	addr = Buffer_Addr%8;
	NumberBegan=8-addr;
	

//�����ַ����ʱ 8byteҳ��
	NumberPage=BufferNumber/8;
//�����ַ����ʱ ����ĵ�ַ����
	NumberSing=BufferNumber%8;
	
/*******��ַ�����д ������******/
	if(addr==0)
	{
		if(NumberPage==0)//С��һҳ
		{
			EEPROM_Page_Write(Buffer_Addr, Buff_data, BufferNumber);
			EEPROM_WaitForWriteEnd();
		}
		else //����һҳ
		{
			while(NumberPage--)
			{
				EEPROM_Page_Write(Buffer_Addr, Buff_data, 8);
				Buffer_Addr=Buffer_Addr+8; //��ַ+8����һҳ
				Buff_data=Buff_data+8;//����+8����8������
				EEPROM_WaitForWriteEnd();
			}
			if(NumberSing!=0)//����һҳ ʣ�µ�����
			{                   //�ϴ�pageѭ����ַ�����鶼�Զ��ӵ���
				EEPROM_Page_Write(Buffer_Addr, Buff_data, NumberSing);//���Ͳ���8byte����
				EEPROM_WaitForWriteEnd();	
			}
			
		}
	}
/*******��ַ�������д ������******/	
	else
	{ 
		if(NumberPage==0) //��ַ������ ��<8
		{
			EEPROM_Page_Write(Buffer_Addr, Buff_data, BufferNumber);
			EEPROM_WaitForWriteEnd();
		}
		else //��һҳ
				 //x 8 8 8 ���� 
			{    BufferNumber -= NumberBegan;
			     NumberPage = BufferNumber/8; //��8�ı���
				 NumberSing = BufferNumber%8; //������
				
				if(NumberBegan != 0)//дx
				{
					EEPROM_Page_Write(Buffer_Addr, Buff_data, NumberBegan);//д8byte
					EEPROM_WaitForWriteEnd();
					Buffer_Addr+=NumberBegan;
					Buff_data+=NumberBegan;
				}
				while(NumberPage--) //дpage
				{
					EEPROM_Page_Write(Buffer_Addr, Buff_data, 8);
					EEPROM_WaitForWriteEnd();
					Buffer_Addr+=8;
					Buff_data+=8;						
				}
				if(NumberSing != 0)//д����
				{
					EEPROM_Page_Write(Buffer_Addr, Buff_data, NumberSing);//д����
					EEPROM_WaitForWriteEnd();
				}
				
			}
			
	}
}





/******************************************************************************************/ 
/**��EEPROM����  
			add_word ������ַ  
			*data ���ݴ������  
			Readnumber����λ��
**/
/******************************************************************************************/ 
void EEPROM_Read(u8 add_word, u8 *data, u8 Readnumber) //���Ķ�����ݱ���д��ָ���������
{
    //������ʼ�ź�
	I2C_GenerateSTART(EEPROM_I2C, ENABLE); //������ʼ�ź�d  
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT)== ERROR);//��ѭ��һֱ�ȼ��EV5�¼�
	
	//���ʹ��豸��ַ
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Transmitter);//����8λ��ַ�������Ǵ��豸8λ��ַ
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)== ERROR);//���EV6�¼�
	
    //���ʹ洢��ַ
	I2C_SendData(EEPROM_I2C,add_word);//���͵�һ�����ݣ���������ַ 
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)== ERROR);//���EV8�¼�
	
	
	//���͵ڶ�����ʼ�ź�Sr 
	I2C_GenerateSTART(EEPROM_I2C, ENABLE); //������ʼ�ź�d  
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT)== ERROR);//��ѭ��һֱ�ȼ��EV5�¼�
	
	//���ʹ��豸��ַ �� ������EV6�¼�
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Receiver);//����8λ��ַ�������Ǵ��豸8λ��ַ
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)== ERROR);//������EV6�¼�

	while(Readnumber)
	{
		if(Readnumber == 1)
		{
		//�������һ���ֽ�
		 I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE); //Ӧ��ʹ�� ��Ӧ��NACK 
		}
	     while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)== ERROR);//���EV7���ݶ��¼�
		 *data=I2C_ReceiveData (EEPROM_I2C);   //������ 
		
		 data++;//ָ��+1 ���ճ�8λ 1byte
		 Readnumber--;
	}
	 
	//���������ź�
	 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
	
	 I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);//Ϊ��������һ��ʹ�� ����ʹ��ACK 
}


//���д�����Ƿ����
//������������ʼ��������ͼ����豸��ַ�ɹ��������ϴ�д�ֽڵĹ��̽�����EEPROM�ڲ��߼�����
u32 EEPROM_WaitForWriteEnd(void)
{
	TimeOut=I2C_Long_time;
	
	do
	{
		TimeOut--;
		//������ʼ�ź�
		I2C_GenerateSTART(EEPROM_I2C, ENABLE); //������ʼ�ź� 
		while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_SB)==RESET) //���û������־λRESET һֱѭ��
		{   TimeOut=I2C_Long_time;
			TimeOut--;
			if((TimeOut--)==0)
			{
				 return I2C_TIMEOUT_UserCallback(9);
			}
		}
		
		//���ʹ��豸��ַ
		I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Transmitter);//����8λ��ַ�������Ǵ��豸8λ��ַ
		
	}while( (I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_ADDR)==RESET) &&(TimeOut>0) ) ; //û������־λRESET �� TimeOutδ����0
	
		
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
	return 1;
}









/******************************************************************************************/ 
/**���IICʧ�� ���ش���ֵ����
    errorCode ����1byte�������    errorCode=0 start�źŴ���
                                   errorCode=1 ���豸��ַ���ʹ���
                                   errorCode=2 ���ݴ洢��ַ ���ʹ���
                                   errorCode=3 ���� ���ʹ���

               ����page�������    errorCode=4 IIC����æ
                                   errorCode=5 start�źŴ���
                                   errorCode=6 ���ʹ��豸��ַ����
                                   errorCode=7 ���ʹ��豸�洢���ݵ�ַ����
                                   errorCode=8 page��������ʧ��
								  
			   �ȴ��������		   errorCode=9 ������ʼ�ź�ʧ�� �ϴ�д����δ���  
			                       errorCode=10 ���͵�ַʧ�� �ϴ�д����δ���  
               
**/
/******************************************************************************************/ 
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  EEPROM__ERROR("IICͨ��ʧ��,�������:%d\r\n",errorCode);
  
  return 0;
}








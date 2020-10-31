#include "EEpromI2C.h"
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

/************ 1.��ʼ��
              1.0��ʼIIC��GPIO
              1.1����IIC��ʼ��*******************************/
//�еı����Ǻ궨�� ע��
//���󲽣�ʹ��ʱ�ӣ����ù��ܣ�ʹ�ܹ���
void I2C_EEPROM_config(void)
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




/************  2.����IICʱ��
               2.0ʹ��byte write������EEPROMд����
	           2.1ʹ��random read����������+���ڴ�ӡ*******************************/
//дһ���ֽڣ���stm32��I2C����ʱ��ͼ��
void EEPROM_Byte_Write(u8 add_word, u8 data)
{
	//������ʼ�ź�
	I2C_GenerateSTART(EEPROM_I2C, ENABLE); //������ʼ�ź�d  
	while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT)== ERROR);//��ѭ��һֱ�ȼ��EV5�¼�
	                  //Ӧ���ù�,���豸�Լ���Ӧ��IIC��Ӧ��Ҳ���ù�ʹ�����Զ�Ӧ��
	//���ʹ��豸��ַ
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Transmitter);//����8λ��ַ�������Ǵ��豸8λ��ַ
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)== ERROR);//���EV6�¼�
	
	//���ʹ洢��ַ
	I2C_SendData(EEPROM_I2C,add_word);//���͵�һ�����ݣ���������ַ 
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)== ERROR);//���EV8�¼�
	
	//��������
	I2C_SendData(EEPROM_I2C,data);//���͵�һ�����ݣ���������ַ 
    while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)== ERROR);//���EV8_2�¼�	
	
	//���������ź�
	 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}



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
void EEPROM_WaitForWriteEnd(void)
{
	do
	{
		//������ʼ�ź�
		I2C_GenerateSTART(EEPROM_I2C, ENABLE); //������ʼ�ź� 
		while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_SB)==RESET); //���û������־λRESET һֱѭ��
		
		//���ʹ��豸��ַ
		I2C_Send7bitAddress(EEPROM_I2C, EEPROM_Address, I2C_Direction_Transmitter);//����8λ��ַ�������Ǵ��豸8λ��ַ
	}while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_ADDR)==RESET); //û������־λRESET һֱѭ��
	
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}





/************  3.��������(����IICʱ��)	  
               3.0��дpage write
               3.1sequential read*******************************/
//EEPROMд����ֽڣ�2Kflashÿ�����д8���ֽ�
void EEPROM_8Byte_Write(u8 add_word, u8 *data, u8 Writernumber)
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
	
	while(Writernumber)
	{

		I2C_SendData(EEPROM_I2C, *data);//*ȡֵ��
		if(Writernumber==1)
		{
			while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)== ERROR);//���EV8_2�¼�	
		}
		if(Writernumber!=1)
		{
			while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)== ERROR);//���EV8�¼�
		}
		Writernumber--;
		data++;
		
	}

	 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}













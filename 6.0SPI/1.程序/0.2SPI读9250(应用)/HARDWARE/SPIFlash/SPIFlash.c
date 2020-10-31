#include "SPIFlash.h"
#include "usart.h"
#include "delay.h"
/**
stm32��Ӳ��SPI������
������дFLASH ��������֧��SPIͨѶ��оƬ
Ҫ���Ϸ���SPIͨ�Ź���
FLsh�ڲ�Ҳ�кܶ����ָ�� ���Ҫ�궨��Flash�Ĳ���ָ����ڲ���
��̲��裺
 1.��ʼ��SPI
      1.0��ʼSPI��GPIO
      1.1����SPI��ʼ��
 2.����SPIʱ��
      2.0ʹ��byte write��������Flashд����
	  2.1ʹ��random read����������+���ڴ�ӡ
 3.�궨��Flash�Ĳ���ָ����ڶ�д����
  
**/

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
//static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode); //��ʱ����

/*************************** ����ISPI��GPIO *****************************/
/**
static ���� ֻ�ܱ�����������
��ѯ�� SPI��ͬ����������mode��ͬ
**/
static void SPI_GPIO_Config(void)
{
//���������ߣ���ʱ�ӣ����ã�ʹ�ܹ���
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_SPI_APBxClock_Fun(GPIO_SPI_CLK, ENABLE);	

//NSS �����������ͨIO ֱ������(���ø���)	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //�������(��ͨ����� ��IO��)
	GPIO_InitStruct.GPIO_Pin = SPI_NSS_CS_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStruct); 
	
//SCK
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //���츴�����
	GPIO_InitStruct.GPIO_Pin = SPI_SCK_PIN;
	GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStruct); 
//MISO
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_InitStruct.GPIO_Pin = SPI_MISO_PIN;
	GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStruct);
//MOSI
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //���츴�����
	GPIO_InitStruct.GPIO_Pin = SPI_MOSI_PIN;
	GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStruct);
	
	FLASH_SPI_CS_HIGH;  //NSS������ֹSPIͨ��
}

/*************************** ���� SPI�Ĺ���ģʽ *****************************/
/**

**/
static void SPI_Config(void)
{
//���������ߣ���ʱ�ӣ����ã�ʹ�ܹ���
	SPI_InitTypeDef   SPI_InitStruct;
	SPI_APBxClock_Fun(SPI_CLK, ENABLE);
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; //72M/128=0.5Mhz 9250���1M
	//ģʽ0 Ҳ����Ϊ3(�ʹ��豸ƥ��)
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge; //SPI_CPHA_2Edge;  //CPHA��CPOL�������SPIģʽ(�ʹ��豸ƥ��)
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;  //SPI_CPOL_High;  //������SPI 3
	SPI_InitStruct.SPI_CRCPolynomial = 0;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//(�ʹ��豸ƥ��)
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫��ȫ˫��
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//(��9250�豸ƥ��)
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master ;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft ;  //���NSSʹ�� ��GPIO
	SPI_Init(SPI_NUMBER, &SPI_InitStruct);  //���� CR1 registeration
	
	//ʹ�ܹ���
    SPI_Cmd(SPI_NUMBER,ENABLE); 

}

/*************************** SPI��ʼ�� *****************************/
/**
����GPIO��ʼ�� �� SPI��ʼ��
**/
void SPI_Commit_Init(void)
{
	SPI_GPIO_Config();
	SPI_Config();
}







/*************************** SPI��Flashд1byte����(������д)*****************************/
/**
1.����Flash��ȡ����   //SPI��ͬ�����յģ�����������ͷ�������д�Ͷ�����һ��������
2.����SPI��ͨѶЭ��
3.��+д����һ���������Ϊд��ͬʱ����ͻ᷵������
4.����д�˲��ܶ�����������֮���ܲ���ʱ��
����ֵΪ0 ��ͨѶʧ��
**/
/***************************************************************************/
u8 SPI_FLASH_Send_Byte(u8 data)
{
	u16 value;
	SPITimeout=SPIT_FLAG_TIMEOUT;//linux��ֹ��ʱ
	/** ����SPI��ͨѶЭ��ȥд **/
	while( SPI_I2S_GetFlagStatus(SPI_NUMBER,SPI_I2S_FLAG_TXE) == RESET) //���ͻ���Ϊ�� (����SPIЭ��) ��ѭ���ȴ� 
    {
		if( (SPITimeout--) == 0)
		{
			FLASH_DEBUG("SPI �ȴ���ʱ!errorCode = %d",0);
			return 0;
		}			
		
	}
	SPI_I2S_SendData(SPI_NUMBER,  data); //��������
	
	SPITimeout=SPIT_FLAG_TIMEOUT;
	//MOSI MISO ������ͬʱ����
	//SPI��ͬ�����յģ�����������ͷ�������д�Ͷ�����һ��������
	while( SPI_I2S_GetFlagStatus(SPI_NUMBER,SPI_I2S_FLAG_RXNE) == RESET) //���ջ���ǿ� (����SPIЭ��) ��ѭ���ȴ� 
    {
       if( (SPITimeout--) == 0)
	   {
		   FLASH_DEBUG("SPI �ȴ���ʱ!errorCode = %d",1);
		   return 0;
	   }
		
	}		
	value = SPI_I2S_ReceiveData(SPI_NUMBER);
	return value;
	//return (SPI_I2S_ReceiveData(SPI_NUMBER)) ;  //��������

}







/*************************** SPI��Flash��1byte����*****************************/
/**
1.�������� �� SPI��Flashд1byte����
2.��Ϊֻ��SPIд��֮����ܶ���ֻ��д�˲�����CLKʱ�� ��������͵�д   
3.����һ��DUMMY��ָ������Ͳ������ϴε�ָ��ܼ���������
**/
/***************************************************************************/
u8 SPI_FLASH_Read_Byte(void)
{
	return SPI_FLASH_Send_Byte(DUMMY);//DUMMY��ָ������Ͳ������ϴε�ָ��ܼ���������
}




/*******************************************************************************************************************************/
/*********************************  MPU9250����  ******************************************************************************/
/*******************************************************************************************************************************/


/*************************** SPI��9250д����*****************************/
/**
д���� 0+7λ�Ĵ�����ַ = �Ĵ�����ַ
������ 1+7λ�Ĵ�����ַ = �Ĵ�����ַ|0x80
**/
/***************************************************************************/
u8 MPU9250_Write_Reg(u8 reg,u8 value)
{
 u8 status;
 FLASH_SPI_CS_LOW;
 status=SPI_FLASH_Send_Byte(reg); //����reg��ַ
 SPI_FLASH_Send_Byte(value);//��������
 FLASH_SPI_CS_HIGH;// MPU9250_CS=1;  //ʧ��MPU9250
 return(status);//
}





/*************************** SPI��9250������*****************************/
/**
д���� 0+7λ�Ĵ�����ַ = �Ĵ�����ַ
������ 1+7λ�Ĵ�����ַ = �Ĵ�����ַ|0x80
**/
/***************************************************************************/
u8 MPU9250_Read_Reg(u8 reg)
{
   u8 reg_val;
   FLASH_SPI_CS_LOW;// MPU9250_CS=0;  //ƬѡMPU9250
   SPI_FLASH_Send_Byte(reg|0x80); //reg��ַ+������
   reg_val=SPI_FLASH_Send_Byte(0xff);//��������
   FLASH_SPI_CS_HIGH;// MPU9250_CS=1;  //ʧ��MPU9250
 return(reg_val);
}




/*************************** ��ʼ��MPU9250  *****************************/
/**
д���� 0+7λ�Ĵ�����ַ = �Ĵ�����ַ
������ 1+7λ�Ĵ�����ַ = �Ĵ�����ַ|0x80
**/
/***************************************************************************************************/
void Init_MPU9250(void)
{ 
	delay_ms(100); //��ʼ��ǰ����ʱһ��ʱ�� �������¸�λ�ϵ�����
/**����9250**/
	//Single_Write(MPU9250_ADDR,USER_CTRL, 0x10);   //����IIC��ģʽ
	
    MPU9250_Write_Reg(PWR_MGMT_1, 0x80);	//��λMPU9250 �������мĴ�������/�������״̬
	MPU9250_Write_Reg(PWR_MGMT_1, 0x00); //����MPU9250����ѡ��������x��PLLΪʱ��Դ(�Զ�ѡ�����ʱ��Դ-����ʹ���ⲿ��)
	                                                     //ѡɶ��һ��
	//Single_Write(MPU9250_ADDR_WRITE,PWR_MGMT_1, 0x68); //��ֹ���ٶȼ�
	//Single_Write(MPU9250_ADDR_WRITE,PWR_MGMT_2, 0x07); //��ֹ������
	
	MPU9250_Write_Reg(INT_ENABLE, 0x00); //��ֹ�ж�INT
	//Single_Write(MPU9250_ADDR_WRITE,INT_ENABLE, 0x01); //ʹ�����ݸ����ж�INT ÿ�����µ����ݾͲ����ж�INT����ߵ�ƽ


/**���� gyro ����������**/	
	MPU9250_Write_Reg(CONFIG, 0x06);        //0x06=0000 0110d ���G������ ��G��5Hz������˲� Fs(Internal_Sample_Rate�������)=1Khz
	MPU9250_Write_Reg(SMPLRT_DIV, 0x07);    //�������������ǲ���Ƶ��Ϊ(����ֲ�2.4) 
	         //1000(�������������)/(1+7)=125Hz (���1k��ӦCONFIG�Ĵ�����Fs) Internal_Sample_Rate
    MPU9250_Write_Reg(GYRO_CONFIG, 0x01);   //���ٶȷ�Χ+-2000
	
/**����  acc ���ٶȼ�����**/
    MPU9250_Write_Reg(ACCEL_CONFIG2, 0x00);   //���ٶ�218.1Hz����  �������1Khz
    MPU9250_Write_Reg(ACCEL_CONFIG, 0x10); //���ٶȷ�Χ+-8g
  //----------------
//	Single_Write(MPU9250_ADDR_WRITE,0x6A,0x00);//close Master Mode	

/**����  M ������**/
    MPU9250_Write_Reg(INT_PIN_CFG,0x02);//turn on Bypass Mode ��·ģʽ ����������ͷ
	delay_ms(10);                                       //ÿ��һ�����ݻ�����һ�ξ�Ҫ��һ����·ģʽ
    MPU9250_Write_Reg(MAG_CNTL2, 0x01);  //��λ������
	MPU9250_Write_Reg(MAG_CNTL1, 0x01);  //����Ϊ���β���ģʽ�����������ֵ
}








/*************************** ���9250ID�� ���Ͳ�����(������ͬʱ����)*****************************/
/**
����W25Q128�ֲᡷP27  JEDEC
1.�Ȱ�CS���ͷ���ָ�� Ƭѡ
2.������Ӧ��IDָ�� 0x75
3.�ٷ��Ϳ�ָ�� ��֤ͨѶ������Ȼ����豸��ַ  
**/
/***************************************************************************************************/
u8 SPI_FLASH_Read_ID(void)
{
	u8 flash_id;
	
	FLASH_SPI_CS_LOW;//Ƭѡ
	SPI_FLASH_Send_Byte(WHO_AM_I|0x80); //reg��ַ+������
    flash_id = SPI_FLASH_Send_Byte(DUMMY);//����DUMMYȷ��������� ������
	
	
	FLASH_SPI_CS_HIGH;
	
	return flash_id;
}



/********************************************************************/
/**
���AK8963���ڲ��������Ƿ����  AKM���豸ID,����һ���ֽں͹̶�ֵ��0x48������������
�����Ʊ�����IIC����ȡ����Ϊ������Ƕ�����豸�У�����Ҫ��оƬ��ַ
**/
/********************************************************************/
bool Check_AK8963_Exist(void)
{
	
}





/*************************** �����ٶ�ֵ  *****************************/
/**

**/
/***************************************************************************************************/





/*************************** ������ֵ *****************************/
/**

**/
/***************************************************************************************************/



////������ȷ�Ĵ�ӡ���У��˺���ֱ�ӱ�FLASH_DEBUG("SPI �ȴ���ʱ!errorCode = %d",0);����
///*************************** DEBUG���� *****************************/
///**
//����spiͨѶ����Ĳ��ִ���
//���룺0    ����һֱ��Ϊ��,�ϴλ�δ������
//���룺1    ����һֱΪ��,û��������
//**/
//static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
//{
//  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
//  FLASH_DEBUG("SPI �ȴ���ʱ!errorCode = %d",errorCode);
//  return 0;
//}
//   








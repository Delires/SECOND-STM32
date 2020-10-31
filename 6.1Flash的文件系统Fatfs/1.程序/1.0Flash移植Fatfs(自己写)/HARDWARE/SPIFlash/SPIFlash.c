#include "SPIFlash.h"
#include "usart.h"
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
	GPIO_SPI_APBxClock_Fun(RCC_APB2Periph_GPIOB, ENABLE);	

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
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //���ٶ�Ҫ�ڴ��豸���շ�Χ��
	//ģʽ0 Ҳ����Ϊ3(�ʹ��豸ƥ��)
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; //SPI_CPHA_2Edge;  //CPHA��CPOL�������SPIģʽ(�ʹ��豸ƥ��)
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;  //SPI_CPOL_High;
	SPI_InitStruct.SPI_CRCPolynomial = 0;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//(�ʹ��豸ƥ��)
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫��ȫ˫��
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//(�ʹ��豸ƥ��)
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
void SPI_Communit_Init(void)
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







/*************************** ���ID�� ���Ͳ�����(������ͬʱ����)*****************************/
/**
����W25Q128�ֲᡷP27  JEDEC
1.�Ȱ�CS���ͷ���ָ�� Ƭѡ
2.������Ӧ��IDָ�� 0x9F �� 0xAB,0X90 (��P24)
3.��SPI���õ��� SPI_DataSize_8b һ�δ�8λ.������W25Q128�ֲᴫ�������ֽ��г��ж�,Ҫ����Ӧ����
  �ڶ�IDҪ����3byte�����ݣ���������Ҫ����һ��>3byte�����ݴ�Ŵ�������ֵ
  ����Ϊһ��ͨѶֻ��8bit������ҪͨѶ3�β��ܰ����ݴ��ꡣ���봩����Ҫ�ȷ�����˶�����һ��DUMMY,����ͨѶ��ͬʱ�ֲ�Ӱ���ϴ�����
4.>3byte������ÿ����һ��SPI��Ҫ����8bit����Ϊ���豸��MSB
**/
/***************************************************************************************************/
u32 SPI_FLASH_Read_ID(void)
{
	u32 flash_id;
	
	FLASH_SPI_CS_LOW;//Ƭѡ
    SPI_FLASH_Send_Byte(JEDEC_ID);//���Ͷ�IDָ��
	
//����SPIЭ�� ��32λ����Ҫ����
//�ӵڶ���ͨѶ��ʼһֱ����DUMMY,�ſ�ʼ���մ��ص�����(ԭ�򿴱ʼ�   4.ʱ������)
	flash_id = SPI_FLASH_Send_Byte(DUMMY);//����DUMMYȷ���������
	flash_id<<=8;
	flash_id |= SPI_FLASH_Send_Byte(DUMMY);
	flash_id<<=8;
	flash_id |= SPI_FLASH_Send_Byte(DUMMY); // |=��Ӱ������λ
	
	FLASH_SPI_CS_HIGH;
	
	return flash_id;
}

/************************** ��ȡFlash��ID�� ���Ͳ�����(������ͬʱ����)*****************************/
u32 SPI_FLASH_SHORT_ID(void)
{
	u32 flash_id;
	
	FLASH_SPI_CS_LOW;//Ƭѡ
	/**�����ֲᷢ��ָ��**/
    SPI_FLASH_Send_Byte(Device_ID);//���Ͷ�IDָ��
	SPI_FLASH_Send_Byte(DUMMY);
	SPI_FLASH_Send_Byte(DUMMY);
	SPI_FLASH_Send_Byte(DUMMY);
	
//����SPIЭ�� ��32λ����Ҫ����
//�ӵڶ���ͨѶ��ʼһֱ����DUMMY,�ſ�ʼ���մ��ص�����(ԭ�򿴱ʼ�   4.ʱ������)
	flash_id = SPI_FLASH_Send_Byte(DUMMY);//����DUMMYȷ���������
	flash_id<<=8;
	flash_id |= SPI_FLASH_Send_Byte(DUMMY);

	
	FLASH_SPI_CS_HIGH;
	
	return flash_id;
}







/*************************** д����ʹ�� *****************************/
/**
1.����FLASH��Ӧʹ�������
2.����֮ǰ��ʹ��
**/
/***************************************************************************/
void SPI_Flash_Write_Enable(void)
{

	FLASH_SPI_CS_LOW; //Ƭѡ
 
	SPI_FLASH_Send_Byte(Write_Enable);
	
	FLASH_SPI_CS_HIGH; 
}







/*************************** �ȴ�����/д���*****************************/
/**
1.ͨ��һֱѭ����ȡ�Ĵ�����λ�����ж��Ƿ����
2.һ��Ҫѭ�����ͣ�ѭ����ȡ
3.Read_Status_Register_1   λ0
**/
/***************************************************************************/
void Wait_Erase_Write_End(void)
{
	u8 riges1_state=0;
	SPITimeout=SPIT_FLAG_TIMEOUT;//linux��ֹ��ʱ
	
	FLASH_SPI_CS_LOW; //Ƭѡ
	SPI_FLASH_Send_Byte(Read_Status_Register_1);//��ȡ��־λ Ҫ�ȴ��������	0λΪ0�������

	do
	{
		riges1_state= SPI_FLASH_Send_Byte(DUMMY);
		if( (SPITimeout--) == 0)
		{
		   FLASH_DEBUG("Sector_Erase ERROR");
		
		}
	}
	while((riges1_state & 0x01) == 1);

	FLASH_SPI_CS_HIGH; 

}







/*************************** ������ʽ����FLASH*****************************/
/**
1.��������������ʽ������FLASH(Flash����С��λ���������洢������������ʽ���洢��)
2.д��֮ǰ�����Ȳ���( ��ΪFLASH�Ļ����洢��ȫΪ1��ֻ�ܰ�1���0,���ܱ����)
3.Ҫ����صĲ�������,�кܶ��ֲ�������
4.����FLASHЭ�� �����ֽ�һͬ��ɲ����� ������ַ A23-A00
5.������Ҫһ��ʱ�䣬����Ҫ��ȡ��־λ Ҫ�ȴ��������
4.����֮ǰҪдʹ��
**/
/***************************************************************************/
void Flash_Sector_Erase(u32 address)
{
	SPI_Flash_Write_Enable(); //����֮ǰҪдʹ��
	Wait_Erase_Write_End();
	
	FLASH_SPI_CS_LOW; //Ƭѡ
	SPI_FLASH_Send_Byte(Sector_Erase);//��������
//�����ĵ�ַ�ָ��1byte �ֱ���
    //SPI_FLASH_Send_Byte( (u8)(address&0xFF0000) ); //����д����u8ֻ�ᱣ����8λ
	SPI_FLASH_Send_Byte( (address>>16)&0xFF  ); //A23-A16  (address>>16)&0xFF 
	SPI_FLASH_Send_Byte( (address>>8)&0xFF  ); //A15-A8
    SPI_FLASH_Send_Byte( (address>>0)&0xFF  ); //A7-A0
	
	FLASH_SPI_CS_HIGH; 
	
	Wait_Erase_Write_End();
}







/*************************** ���ֽڶ�ȡFLASHֵ *****************************/
/**
1.��������������ʽ������FLASH(Flash�洢������������ʽ���洢��)
2.д��֮ǰ�����Ȳ���( ��ΪFLASH�Ļ����洢��ȫΪ1��ֻ�ܰ�1���0,���ܱ����)
3.��ȡ��ֵ����ָ�� ���÷���ֵ
4.ָ��+24bit��ַ+������
**/
/***************************************************************************/
void Read_Sector_Flash(u32 address, u8 *readBuffer, u32 numReadByte)
{                                   //��ȡ��ֵ����ָ�� ���÷���ֵ
	FLASH_SPI_CS_LOW; //Ƭѡ
	 
	SPI_FLASH_Send_Byte(Read_Data);//FLASHָ��
//�����ĵ�ַ�ָ��1byte �ֱ���
    //SPI_FLASH_Send_Byte( (u8)(address&0xFF0000) ); //����д����u8ֻ�ᱣ����8λ
	SPI_FLASH_Send_Byte( (address>>16)&0xFF  ); //A23-A16  (address>>16)&0xFF 
	SPI_FLASH_Send_Byte( (address>>8)&0xFF  ); //A15-A8
    SPI_FLASH_Send_Byte( (address>>0)&0xFF  ); //A7-A0
	
	while(numReadByte--)
	{
		*readBuffer = SPI_FLASH_Send_Byte(DUMMY);
		readBuffer++;
	}
	
	FLASH_SPI_CS_HIGH; 
}







/*************************** ��FLASHд������ *****************************/
/**
1.��������������ʽ������FLASH(Flash�洢������������ʽ���洢��)
2.д��֮ǰ�����Ȳ���( ��ΪFLASH�Ļ����洢��ȫΪ1��ֻ�ܰ�1���0,���ܱ����)
3.д֮ǰҪдʹ�ܣ��ȴ�д����
4.ָ��+24bit��ַ+д����
5.���֧��һ����д��256��
**/
/***************************************************************************/
u8 Write_Less256byte_Flash(u32 address, u8 *writeBuffer, u32 numWriteByte)
{    
     SPI_Flash_Write_Enable(); //����֮ǰҪдʹ��
	 Wait_Erase_Write_End();	
	
     FLASH_SPI_CS_LOW; //Ƭѡ
  	 SPI_FLASH_Send_Byte(Page_Program); //д����
	 
	 SPI_FLASH_Send_Byte( (address>>16)&0xFF ); //��λ���͵�ַ
	 SPI_FLASH_Send_Byte( (address>>8)&0xFF );
	 SPI_FLASH_Send_Byte( (address>>0)&0xFF );
	
	 if( numWriteByte > ONE_WRIT_MORENUMBER ) //��return ������256byte ֱ�ӽ���
	 {
		  FLASH_DEBUG("SPI write bvte number than 256 byte");
		 return 0;
	 }
	 
	 while(numWriteByte--)
	 {
		SPI_FLASH_Send_Byte( *(writeBuffer) );
		writeBuffer++;
	 }
	 
	 FLASH_SPI_CS_HIGH; 
	 
	 Wait_Erase_Write_End();
	 
	 return 1;
}







/***************************����256�ֽڵĶ��ֽ�д��*****************************/
/**
1.����page��������(256�ֽ�Ҫ������)
2.��ַ���⣺(256���� �� ���� )ÿ����256byte����ַ��Ҫ����256�������µ���д256byte
                            256����д��ʣ�µĵ����ĵ�ַҲҪ���������µ���
3.�ֽ����⣺���Pagenumber �� singlenumer ��ϵ�ַ����
4.���ڵ�ַ����(Flash���񲻴��ڵ�ַ��������)
**/
u8 Write_More_256Byte(u32 address, u8 *writeBuffer, u32 numWriteByte)
{    
	 u8 pageNumer, singleNumer;

	 if( numWriteByte > ONE_WRIT_MORENUMBER ) //��return ������256byte ֱ�ӽ���
	 {
		 pageNumer = numWriteByte/ONE_WRIT_MORENUMBER;
		 singleNumer = numWriteByte%ONE_WRIT_MORENUMBER;
		 
		while(pageNumer --)//ÿ256byteѭ��һ��
		{
			Write_Less256byte_Flash(address, writeBuffer, ONE_WRIT_MORENUMBER);//��ҳ��д
			address+=ONE_WRIT_MORENUMBER;
			writeBuffer+=ONE_WRIT_MORENUMBER;	
		}	
	   	    Write_Less256byte_Flash(address, writeBuffer, singleNumer);//д����256byte��
		
	 }
	 else //С��256byte
	 {
		    Write_Less256byte_Flash(address, writeBuffer, numWriteByte);
	 }
	 return 1;
}




/********************************Ұ��д����ֽ�*******************************************/
void Write_More_256Byte_YeHuo(u32 Maddress, u8 *MwriteBuffer, u32 numWriteByte)
{                                  
    u8 numPage=0,   numSingle=0; //�����ֽ�����
	u8 count=0, Addr=0,  temp=0; //�����ַ����
//��ַ����	 
	Addr = Maddress % ONE_WRIT_MORENUMBER; //����ַ�Ƿ�Ϊ256���������׵�ַ�Ƿ����
	count = ONE_WRIT_MORENUMBER - Addr;  //count=256-��ַ  һҳʣ�µ��ֽ���
//�ֽ�����
    numPage = numWriteByte/ONE_WRIT_MORENUMBER;
	numSingle = numWriteByte%ONE_WRIT_MORENUMBER;
	
	if(Addr == 0)//��ַ����
	{
		if(numPage == 0)//����1ҳ 256byte
		{
			Write_Less256byte_Flash(Maddress, MwriteBuffer, numWriteByte);//ֱ�ӵ���
		}
		else //����1ҳ 256byte
		{
			while( numPage --)
			{
				Write_Less256byte_Flash(Maddress, MwriteBuffer, ONE_WRIT_MORENUMBER);//��ε���256����
				Maddress += ONE_WRIT_MORENUMBER;  //��ַ��256
				MwriteBuffer += ONE_WRIT_MORENUMBER; // ���飫256
			}
			
			Write_Less256byte_Flash(Maddress, MwriteBuffer, numSingle);//����һҳ��д��
			
		}
	}
/*******��ַ�������д ������******/	
	else  //��ַ������ ��ַ����256��������
	{
		if(numPage == 0)//����1ҳ 
		{
			if(numSingle>count) //����д������ > ��ַ~256��ֵ
			{
				temp = numSingle-count;
				Write_Less256byte_Flash(Maddress, MwriteBuffer, count); //��д����ҳ
				Maddress +=count; //���� ��ַ ����count
				MwriteBuffer +=count;
				
				Write_Less256byte_Flash(Maddress, MwriteBuffer, temp);//��д��ʣ�µ�
			}
			else//����д������ < ��ַ~256��ֵ  ֱ��д
			{
				Write_Less256byte_Flash(Maddress, MwriteBuffer, numSingle);
			}
		}
		else//����1ҳ 
		{
			numWriteByte -=count; //�Ѷ����count��ȥ���д���ʣ�µĵ�������
			numPage = numWriteByte/ONE_WRIT_MORENUMBER;
   	        numSingle = numWriteByte%ONE_WRIT_MORENUMBER;
			
			Write_Less256byte_Flash(Maddress, MwriteBuffer, count);//��дcount
			Maddress+=count;
			MwriteBuffer+=count;
			
			/** ��count�� �����������¾Ͷ�����**/
			while(numPage--)
			{
				Write_Less256byte_Flash(Maddress, MwriteBuffer, ONE_WRIT_MORENUMBER);
			    Maddress+=ONE_WRIT_MORENUMBER;
			    MwriteBuffer+=ONE_WRIT_MORENUMBER;
			}
			if( numSingle != 0 )
			{
				Write_Less256byte_Flash(Maddress, MwriteBuffer, numSingle); //��������д��
			}
		}
	}
}





/*************************** FLASH����͹���ģʽ *****************************/
/**
1.����ͨѶ����д��д��֮��NSҪ����
**/
/***************************************************************************/
void Power_Down_Flash(void)
{                                   //��ȡ��ֵ����ָ�� ���÷���ֵ
	FLASH_SPI_CS_LOW; //Ƭѡ
	
	SPI_FLASH_Send_Byte(Power_down); 
	
	FLASH_SPI_CS_HIGH;
	
}







/*************************** FLASH�ӵ͹���ģʽ���ѳ��� *****************************/
/**
1.����ͨѶ����д
**/
/***************************************************************************/
void Release_Power_Down_Flash(void)
{                                   //��ȡ��ֵ����ָ�� ���÷���ֵ
	FLASH_SPI_CS_LOW; //Ƭѡ
	
	SPI_FLASH_Send_Byte(Release_Powerdown); 
	
	FLASH_SPI_CS_HIGH;
	 	
}


#include "IIC_gpio.h"
/****************************
Ô­Àí£º  Á½¸ùGPIO¿ÚÄ£ÄâSDA SCL ²úÉúÆðÊ¼ÐÅºÅ£¬Í£Ö¹ÐÅºÅ£¬ACKµÈÐÅºÅ¡£Ó²¼þIICÓÉÓÚ¸´ÓÃ»¹ÓÐÆäËû¹¦ÄÜ£¬»á³öÏÖ¸÷ÖÖÎÊÌâ£¬ËùÒÔ¶àÈí¼þIIC¡£
¶þÕßÇø±ð£ºÈí¼þIICÃ»ÓÐ¸÷ÖÖ¼ì²âÎ»£¬Ö»ÄÜ¼òµ¥µÄ¿ØÖÆIOµçÆ½Ä£ÄâIICÀ´¶ÁÈ¡¡£
          ÓÉÓÚ¸ßµÍµçÆ½µÄ±ä»¯»áÈÃGPIOµÄIDR¼Ä´æÆ÷µÄ¶ÁÈ¡Öµ²»£¬Í¨¹ý¶ÁÈ¡IDR¼Ä´æÆ÷À´¶ÁÈ¡Öµ¡£

´Ëº¯ÊýÐ´ ¢ÙÅäÖÃGPIO Ëæ±ãÁ½¸öÒý½Å ¿ªÂ©Êä³ö
         ¢Ú¸ù¾ÝIICµÄÊ±ÐòÍ¼,²úÉúÆðÊ¼ÐÅºÅ£¬Í£Ö¹ÐÅºÅ£¬ACKµÈÐÅºÅ
		 ¢ÛÖ÷»ú¶Á/È¡ ¸Ä±äGPIOµÄÊäÈë/Êä³öÄ£Ê½
		 ¢ÜACK:Ö÷»úÓ¦´ðÄ£ÄâACKÐÅºÅ£¬´Ó»úÓ¦´ðÊÍ·ÅSDAÏß¼´¿É

*****************************/

/*
*********************************************************************************************************
*	º¯ Êý Ãû: i2c_Delay
*	¹¦ÄÜËµÃ÷: I2C×ÜÏßÎ»ÑÓ³Ù£¬×î¿ì400KHz
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/

static void i2c_Delay(void)//¾­ÑéÖµ
{
	uint8_t i;

	/*¡¡
	 	ÏÂÃæµÄÊ±¼äÊÇÍ¨¹ýÂß¼­·ÖÎöÒÇ²âÊÔµÃµ½µÄ¡£
    ¹¤×÷Ìõ¼þ£ºCPUÖ÷Æµ72MHz £¬MDK±àÒë»·¾³£¬1¼¶ÓÅ»¯
  
		Ñ­»·´ÎÊýÎª10Ê±£¬SCLÆµÂÊ = 205KHz 
		Ñ­»·´ÎÊýÎª7Ê±£¬SCLÆµÂÊ = 347KHz£¬ SCL¸ßµçÆ½Ê±¼ä1.5us£¬SCLµÍµçÆ½Ê±¼ä2.87us 
	 	Ñ­»·´ÎÊýÎª5Ê±£¬SCLÆµÂÊ = 421KHz£¬ SCL¸ßµçÆ½Ê±¼ä1.25us£¬SCLµÍµçÆ½Ê±¼ä2.375us 
	*/                          //SCL×î¿ì400KHZ
	for (i = 0; i < 10; i++);
}



/**********************************************************************/
/**ÅäÖÃGPIO¿Ú----¿ªÂ©Êä³öGPIO_Mode_Out_OD  ÅäÖÃÈý·¨±¦£ºÊ±ÖÓ ÅäÖÃ Ê¹ÄÜ
**/
/**********************************************************************/
void IIC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD; //¿ªÂ©Ê ä³ö
	GPIO_InitStruct.GPIO_Pin = II2_SDA_PIN|II2_SCL_PIN;  //PB6-SCL PB7-SDA    
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStruct);
	
	GPIO_SetBits(IIC_PORT, II2_SDA_PIN|II2_SCL_PIN); //¸ßµçÆ½ È«²¿ÊÍ·Å iicµÍµçÆ½ÄÃÊÂ
}


//·¢ËÍÊý¾Ý(Êý¾ÝÏÈ×¼±¸ºÃ) 
//¶ÁÊý¾Ý(Ê±ÖÓÏÈ×¼±¸ºÃ) 
/**********************************************************************/
/**²úÉústartÆðÊ¼ÐÅºÅ                                                 
   SCL=1 Ê± SDA:1--0
   ÅäÖÃÁ½¸öºê Ö±½Ó¼Ä´æÆ÷¿ØÖÆIOµçÆ½
**/
/**********************************************************************/
void IIC_Start(void)
{//·¢ËÍÊý¾Ý(Êý¾ÝÏÈ×¼±¸ºÃ)
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
/**²úÉúStopÆðÊ¼ÐÅºÅ                                                 
   SCL=1 Ê± SDA:0--1
   ÅäÖÃÁ½¸öºê Ö±½Ó¼Ä´æÆ÷¿ØÖÆIOµçÆ½
//·¢ËÍÊý¾Ý(Êý¾ÝÏÈ×¼±¸ºÃ) 
**/
/**********************************************************************/
void IIC_Stop(void)
{//·¢ËÍÊý¾Ý(Êý¾ÝÏÈ×¼±¸ºÃ)
	SDA_OUT();
	
	IIC_Analog_SDA_0();
	IIC_Analog_SCL_1();
	i2c_Delay();
	IIC_Analog_SDA_1();
	//i2c_Delay();
}



/**********************************************************************/
/**
   IIC·¢ËÍ1byteµÄ8bitÊý¾Ý
   ²ÎÊý£ºByteÒ»×Ö½ÚÊý¾Ý
//·¢ËÍÊý¾Ý(Êý¾ÝÏÈ×¼±¸ºÃ)
**/
/**********************************************************************/
void IIC_SendByte(u8 _ucByte)
{//·¢ËÍÊý¾Ý(Êý¾ÝÏÈ×¼±¸ºÃ)
	u8 i;
	//SDA_OUT();
	for(i=0; i<8; i++)
	{
		if(_ucByte & 0x80) //0x80=1000 0000 È¡×î¸ßÎ»
		{
			IIC_Analog_SDA_1(); //×î¸ßÎ»ÊÇ1 SDA·¢¸ßµçÆ½
		}
		else
		{
			IIC_Analog_SDA_0();
		}
		i2c_Delay(); //Êý¾ÝÏÈ×¼±¸ºÃ
		IIC_Analog_SCL_1();//ÔÚÉèÖÃSCL
		i2c_Delay();
		IIC_Analog_SCL_0();
		if(i==7)
		{
			IIC_Analog_SDA_1();//ÊÍ·ÅSDAÏß£¬´ËÊ±SCL=0Êý¾ÝÎÞÐ§
		}
		_ucByte<<=1;// <<°´Î»²Ù×÷
		i2c_Delay();
	}
}



/**********************************************************************/
/**
   IIC¶Á1byteµÄ8bitÊý¾Ý
   ²ÎÊý£ºvoid
   f·µ»Ø value
//¶ÁÊý¾Ý SCLÊ±ÖÓÏÈ×¼±¸ºÃ
**/
/**********************************************************************/
u8 IIC_ReadByte(void)
{//·¢ËÍÊý¾Ý(Êý¾ÝÏÈ×¼±¸ºÃ)
	u8 i, value;
	//SDA_IN();
	value=0;
	for(i=0; i<8; i++)
	{
		value <<=1; //MSB¸ßÎ»ÏÈ //¢ÙÊÔ×Å·ÅºóÃæ¿´Ó°Ïì²»
		IIC_Analog_SCL_1();//¶ÁÊý¾Ý SCLÊ±ÖÓÏÈ×¼±¸ºÃ
		i2c_Delay();
		if(READ_SDA()==1)
		{
			value++;
		}
		IIC_Analog_SCL_0(); //ÒòÎªÊÇ¶ÁÊý¾Ý Ö÷»úÎÞ·¨¿ØÖÆSDA ¸üÌ¸²»ÉÏÊÍ·Å×ÜÏß
		i2c_Delay();
	}
	return value;
}



/**********************************************************************/
/**
   Ö÷»úµÈ´ýÓ¦´ðº¯Êý
   ÊÍ·ÅSDA×ÜÏß ±ä³É¶Á ¿´SDAÊÇ·ñÎª0 0ÔòÓ¦´ð³É¹¦ »¹Òª×¢ÒâÊý¾ÝÓÐÐ§ÐÔ
   ·µ»ØÖµack=0 ´Ó»úÓ¦´ð³É¹¦
//¶ÁÊý¾Ý SCLÊ±ÖÓÏÈ×¼±¸ºÃ
**/
/**********************************************************************/
u8 IIC_Wait_Ack(void)
{//·¢ËÍÊý¾Ý(Êý¾ÝÏÈ×¼±¸ºÃ)
	u8 ack, Timeout=0;//³¬Ê±´¦Àí
	//SDA_IN();
	
	IIC_Analog_SDA_1();//ÊÍ·ÅSDAÏß
	i2c_Delay();
	IIC_Analog_SCL_1(); //¶ÁÊý¾Ý Ê±ÖÓÏÈ×¼±¸ºÃ
	i2c_Delay();
	if(READ_SDA()==1)
	{
		ack=1;
		Timeout++;     //³¬Ê±´¦Àí
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
   Ö÷»ú²úÉúÒ»¸öÓ¦´ð ACK=0
//Ð´Êý¾Ý SDAÏÈ×¼±¸ºÃ
**/
/**********************************************************************/
void IIC_Ack(void)
{//Ð´Êý¾Ý SDAÏÈ×¼±¸ºÃ
	//SDA_OUT();
	
	IIC_Analog_SDA_0();
	i2c_Delay();
	IIC_Analog_SCL_1();
	i2c_Delay();
	IIC_Analog_SCL_0();
	i2c_Delay();	
	
	IIC_Analog_SDA_1();//ÊÍ·ÅSDA×ÜÏß
}



/**********************************************************************/
/**
   Ö÷»ú²úÉúÒ»¸öÓ¦´ð NACK=0
//Ð´Êý¾Ý SDAÏÈ×¼±¸ºÃ
**/
/**********************************************************************/
void IIC_Nack(void)
{//Ð´Êý¾Ý SDAÏÈ×¼±¸ºÃ
	//SDA_OUT();
	
	IIC_Analog_SDA_1();
	i2c_Delay();
	IIC_Analog_SCL_1();
	i2c_Delay();
	IIC_Analog_SCL_0();
	i2c_Delay();	
	
	//²»ÓÃÊÍ·ÅSDA×ÜÏß ±¾À´¾ÍÊÇ1
}



/**********************************************************************/
/**
   ¼ì²âÉè±¸ÊÇ·ñ´æÔÚ
   ·¢ËÍ¸ø8bitµÄÉè±¸´ÓµØÖ·(²»¹Ü·¢ËÍÉ¶ Ö»ÒªÊÕµ½´ÓÉè±¸¶¼»á»ØÓ¦Ò»¸öack) µÈ´ýÓ¦´ð  Èç¹ûÓÐÓ¦´ð·µ»Ø0 ´ú±íÉè±¸´æÔÚ¿ÉÁ¬½Ó
//Ð´Êý¾Ý SDAÏÈ×¼±¸ºÃ
**/
/**********************************************************************/
u8 IIC_Check_Device(u8 addr)
{//Ð´Êý¾Ý SDAÏÈ×¼±¸ºÃ
	u8 cAck;
	
	IIC_GPIO_Config();//ÅäÖÃ
	
	IIC_Start();

	IIC_SendByte(addr|IIC_Write); //·¢ËÍ7Î»µØÖ·¼ÓÐ´
	cAck=IIC_Wait_Ack();
	
	IIC_Stop();
	
	return cAck;
}






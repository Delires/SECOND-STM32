//��.c���Ǹ��ļ���ת�����ÿ⺯�� ��������
#include "RCC_chaopin.h"
/*�Լ�д��*/

u8 ZZZ_ABC[25];
u8 i;




void HSE_SetClock(uint32_t RCC_PLLMul_x) //����ֵΪ��Ƶֵ
{
	__IO ErrorStatus HSEstatus; //����HSE��־λ
	__IO uint8_t PLLstatus; //PLL����Ϊϵͳʱ�ӱ�־λ
 
//��ʱ�ӵ�����ȫ����λ
	RCC_DeInit(); 
	
    RCC_HSEConfig(RCC_HSE_ON); //ʹ��HSE
	HSEstatus=RCC_WaitForHSEStartUp();
	if(HSEstatus==SUCCESS)
	{
	//��STM32������ˮ�߲���
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//Ԥ����
		FLASH_SetLatency(FLASH_Latency_2);  //��ʱ����״̬����Ƶ��̫��FLASH������Ԥ����
		
	//����HCLK PLCK1 PCLK2Sʱ�� ��ϵͳʱ��system�Ĺ�ϵ
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
	
	//����OLLC���໷
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_x); //��Ƶ��
	    RCC_PLLCmd(ENABLE); //ʹ��PLL
		
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)!=SET)
		{
			
		}
		
	//����ϵͳsystemʱ��
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	    PLLstatus=RCC_GetSYSCLKSource();//��ϵͳ���óɹ�����ֵ
		while((PLLstatus & RCC_CFGR_SWS) != (uint32_t)0x08)//����δ�ɹ�
		{
		}	
	}
	
	else//������ʱ��������Ԥ������
	{
		//���HSE����ʧ�ܣ��ڴ˴�д��Ӧ������
	}
	for(i=0; i<25; i++)
	{
		ZZZ_ABC[i]=i;
	}
}
	


void HSI_SetClock(uint32_t RCC_PLLMul_x) //����ֵΪ��Ƶֵ
{
    __IO  uint32_t HSIStatus, StartUpCounter;
	__IO uint8_t PLLstatus;
	
//��ʱ�ӵ�����ȫ����λ
	RCC_DeInit(); 

//ʹ��HSI	
    RCC_HSICmd(ENABLE); //ʹ��HSi
	HSIStatus = RCC->CR & RCC_CR_HSIRDY;  //�� RCC_GetFlagStatus(RCC_FLAG_HSIRDY);
	                     //RCC_CR_HSIRDY=0x02 �����°�ʹ��λ��û
	if(HSIStatus==RCC_CR_HSIRDY) //�����ɹ�
	{
	//��STM32������ˮ�߲���
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//Ԥ����
		FLASH_SetLatency(FLASH_Latency_2);  //��ʱ����״̬����Ƶ��̫��FLASH������Ԥ����
		
	//����HCLK PLCK1 PCLK2Sʱ�� ��ϵͳʱ��system�Ĺ�ϵ
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
	
	//����OLLC���໷  ��ΪHSI
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_x); //��ʱ����֪��PLL��HSI�Ļ�ֻ��/2  ����PLL��� 4*16=64M
	    RCC_PLLCmd(ENABLE); //ʹ��PLL
		
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)!=SET)
		{
			
		}
		
	//����ϵͳsystemʱ��
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	    PLLstatus=RCC_GetSYSCLKSource();//��ϵͳ���óɹ�����ֵ
		while((PLLstatus & RCC_CFGR_SWS) != (uint32_t)0x08)//����δ�ɹ�
		{
		}	
	}
	
	else//������ʱ��������Ԥ������
	{
		//���HSI����ʧ�ܣ��ڴ˴�д��Ӧ������
	}
}
	


//����PA8�鿴MCO����  ��Դ��system������
void MCO_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		    //���츴�ã���ΪҪ���ʱ�Ӳ���
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5

}





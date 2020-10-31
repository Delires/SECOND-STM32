//把.c的那个文件在转换成用库函数 来回运用
#include "RCC_chaopin.h"
/*自己写的*/

u8 ZZZ_ABC[25];
u8 i;




void HSE_SetClock(uint32_t RCC_PLLMul_x) //输入值为倍频值
{
	__IO ErrorStatus HSEstatus; //返回HSE标志位
	__IO uint8_t PLLstatus; //PLL配置为系统时钟标志位
 
//把时钟的配置全部复位
	RCC_DeInit(); 
	
    RCC_HSEConfig(RCC_HSE_ON); //使能HSE
	HSEstatus=RCC_WaitForHSEStartUp();
	if(HSEstatus==SUCCESS)
	{
	//让STM32三步流水线操作
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//预处理
		FLASH_SetLatency(FLASH_Latency_2);  //延时两个状态，怕频率太快FLASH来不及预处理
		
	//配置HCLK PLCK1 PCLK2S时钟 和系统时钟system的关系
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
	
	//配置OLLC锁相环
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_x); //倍频数
	    RCC_PLLCmd(ENABLE); //使能PLL
		
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)!=SET)
		{
			
		}
		
	//配置系统system时钟
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	    PLLstatus=RCC_GetSYSCLKSource();//读系统配置成功返回值
		while((PLLstatus & RCC_CFGR_SWS) != (uint32_t)0x08)//配置未成功
		{
		}	
	}
	
	else//开启超时，做错误预备处理
	{
		//如果HSE配置失败，在此处写相应处理函数
	}
	for(i=0; i<25; i++)
	{
		ZZZ_ABC[i]=i;
	}
}
	


void HSI_SetClock(uint32_t RCC_PLLMul_x) //输入值为倍频值
{
    __IO  uint32_t HSIStatus, StartUpCounter;
	__IO uint8_t PLLstatus;
	
//把时钟的配置全部复位
	RCC_DeInit(); 

//使能HSI	
    RCC_HSICmd(ENABLE); //使能HSi
	HSIStatus = RCC->CR & RCC_CR_HSIRDY;  //或 RCC_GetFlagStatus(RCC_FLAG_HSIRDY);
	                     //RCC_CR_HSIRDY=0x02 不用怕把使能位与没
	if(HSIStatus==RCC_CR_HSIRDY) //开启成功
	{
	//让STM32三步流水线操作
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//预处理
		FLASH_SetLatency(FLASH_Latency_2);  //延时两个状态，怕频率太快FLASH来不及预处理
		
	//配置HCLK PLCK1 PCLK2S时钟 和系统时钟system的关系
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
	
	//配置OLLC锁相环  改为HSI
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_x); //有时钟树知，PLL用HSI的话只能/2  所以PLL最大 4*16=64M
	    RCC_PLLCmd(ENABLE); //使能PLL
		
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)!=SET)
		{
			
		}
		
	//配置系统system时钟
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	    PLLstatus=RCC_GetSYSCLKSource();//读系统配置成功返回值
		while((PLLstatus & RCC_CFGR_SWS) != (uint32_t)0x08)//配置未成功
		{
		}	
	}
	
	else//开启超时，做错误预备处理
	{
		//如果HSI配置失败，在此处写相应处理函数
	}
}
	


//复用PA8查看MCO波形  来源于system的配置
void MCO_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		    //推挽复用，因为要输出时钟波形
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5

}





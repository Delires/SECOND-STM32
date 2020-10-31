#include "dac.h"
#include "dma.h"
#include "tim.h"

void GPIOA_4(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //使能PORTA通道时钟
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //模拟输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4)	;//PA.4 输出高
}


void Dac1_Init(void) //DAC初始化无普通输出
{
 	DAC_InitTypeDef DAC_InitStruct;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //使能DAC通道时钟 


//	DAC_InitStruct.DAC_Trigger=DAC_Trigger_None;			
	DAC_InitStruct.DAC_Trigger=DAC_Trigger_T2_TRGO;	//定时器2触发   databuf定义的是整个数组长度，则把所数组所有数都搬到DA_DOR里
	DAC_InitStruct.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitStruct.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitStruct);	 //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC1
	DAC_DMACmd(DAC_Channel_1, ENABLE);//使能
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值   初始值为0

}



//设置通道1输出电压
void ConstantVol(float vol)
{
	float temp = vol;
	u16 i;
	for(i=0;i<=n; i++)
	{
		data[i]=temp*4095.0/3.3;       //DA12位 2^12=4096 平分给3.3V
		dma_data[i]=data[i];
	}
}


//三角电压
void TriangleVol(void)  
{
	u16 i, j=n/2; //三角波一半一半
	for(i=0; i<=n; i++)
	{
		if(i<j) //波形增
		{
			data[i]=(i*((1.0/f)/n))*(4095.0/(0.5/f));   //前者是把数组平分给周期   后者是把DA值平分给半个周期
                                                	//前者数组随时间变化率   后者幅值变化率
      dma_data[i]=data[i];
		} 
    else//波形减
    {
      data[i]=(4096.0*2.0)-(i*((1.0/f)/n))*(4095.0/(0.5/f));
			dma_data[i]=data[i];
		}			
		
	}
}



//方波
void SquareVol(void)
{
  u16 i;
	for(i=0; i<n; i++)
	{
		if(i<n*k) //波形增
		{
			data[i]=4095; 
      dma_data[i]=data[i];
		} 
    else//波形减
    {
      data[i]=0;
			dma_data[i]=data[i];
		}
  }	
}
	



//锯齿波
void SawVol(void)
{
	u16 i;
	for(i=0; i<=n; i++)
	{
		 data[i]=(i*((1.0/f)/n))*(4095.0/(1.0/f));  //锯齿 DA数据平分给一个周期
		 dma_data[i]=data[i];
	}
}





//float data[n];    //放数字量数据
//u16 dma_data[n];


















































#include "dac.h"
#include "dma.h"
#include "tim.h"

void GPIOA_4(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //ʹ��PORTAͨ��ʱ��
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4)	;//PA.4 �����
}


void Dac1_Init(void) //DAC��ʼ������ͨ���
{
 	DAC_InitTypeDef DAC_InitStruct;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //ʹ��DACͨ��ʱ�� 


//	DAC_InitStruct.DAC_Trigger=DAC_Trigger_None;			
	DAC_InitStruct.DAC_Trigger=DAC_Trigger_T2_TRGO;	//��ʱ��2����   databuf��������������鳤�ȣ�������������������ᵽDA_DOR��
	DAC_InitStruct.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitStruct.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitStruct);	 //��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DAC1
	DAC_DMACmd(DAC_Channel_1, ENABLE);//ʹ��
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ   ��ʼֵΪ0

}



//����ͨ��1�����ѹ
void ConstantVol(float vol)
{
	float temp = vol;
	u16 i;
	for(i=0;i<=n; i++)
	{
		data[i]=temp*4095.0/3.3;       //DA12λ 2^12=4096 ƽ�ָ�3.3V
		dma_data[i]=data[i];
	}
}


//���ǵ�ѹ
void TriangleVol(void)  
{
	u16 i, j=n/2; //���ǲ�һ��һ��
	for(i=0; i<=n; i++)
	{
		if(i<j) //������
		{
			data[i]=(i*((1.0/f)/n))*(4095.0/(0.5/f));   //ǰ���ǰ�����ƽ�ָ�����   �����ǰ�DAֵƽ�ָ��������
                                                	//ǰ��������ʱ��仯��   ���߷�ֵ�仯��
      dma_data[i]=data[i];
		} 
    else//���μ�
    {
      data[i]=(4096.0*2.0)-(i*((1.0/f)/n))*(4095.0/(0.5/f));
			dma_data[i]=data[i];
		}			
		
	}
}



//����
void SquareVol(void)
{
  u16 i;
	for(i=0; i<n; i++)
	{
		if(i<n*k) //������
		{
			data[i]=4095; 
      dma_data[i]=data[i];
		} 
    else//���μ�
    {
      data[i]=0;
			dma_data[i]=data[i];
		}
  }	
}
	



//��ݲ�
void SawVol(void)
{
	u16 i;
	for(i=0; i<=n; i++)
	{
		 data[i]=(i*((1.0/f)/n))*(4095.0/(1.0/f));  //��� DA����ƽ�ָ�һ������
		 dma_data[i]=data[i];
	}
}





//float data[n];    //������������
//u16 dma_data[n];


















































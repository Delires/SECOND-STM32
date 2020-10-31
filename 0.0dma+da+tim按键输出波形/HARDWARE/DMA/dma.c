#include "dma.h"
#include "math.h"
#include "dac.h"

float data[n];    //������������
u16 dma_data[n];

void dma_config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	//Դ   Ŀ���ַ
	DMA_InitStruct.DMA_PeripheralBaseAddr= 0x40007408;  // (u32)&DAC->DHR12R1;
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&dma_data;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST; //���䷽��
	DMA_InitStruct.DMA_BufferSize = n;   //�����С
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;  //Ŀ�ĵ�ַ������
	DMA_InitStruct.DMA_MemoryInc= DMA_MemoryInc_Enable;  //Դ��ַ����
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
                                                     //DA������12λ �������ô�����Ϊ16λ	
	DMA_InitStruct.DMA_MemoryDataSize= DMA_MemoryDataSize_HalfWord; 
	DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;  //ѭ��
	DMA_InitStruct.DMA_Priority=DMA_Priority_High; //���ȼ���
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;   //���ڴ浽�ڴ�
	DMA_Init(DMA2_Channel3,&DMA_InitStruct);
	
	DMA_Cmd(DMA2_Channel3, ENABLE);
}

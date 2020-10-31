#include "dma.h"
#include "math.h"
#include "dac.h"

float data[n];    //放数字量数据
u16 dma_data[n];

void dma_config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	//源   目标地址
	DMA_InitStruct.DMA_PeripheralBaseAddr= 0x40007408;  // (u32)&DAC->DHR12R1;
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&dma_data;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST; //传输方向
	DMA_InitStruct.DMA_BufferSize = n;   //缓存大小
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;  //目的地址不增加
	DMA_InitStruct.DMA_MemoryInc= DMA_MemoryInc_Enable;  //源地址增加
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
                                                     //DA数据是12位 所以设置传输宽度为16位	
	DMA_InitStruct.DMA_MemoryDataSize= DMA_MemoryDataSize_HalfWord; 
	DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;  //循环
	DMA_InitStruct.DMA_Priority=DMA_Priority_High; //优先级高
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;   //非内存到内存
	DMA_Init(DMA2_Channel3,&DMA_InitStruct);
	
	DMA_Cmd(DMA2_Channel3, ENABLE);
}

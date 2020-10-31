#ifndef __DMA_H
#define __DMA_H	 
#include "sys.h"	    

#define n 100 //数组长度
#define f 5 //频率
#define k 0.7 //占空比
   
	 
extern float data[n];    //放数字量数据
extern u16 dma_data[n];
//#define DAC_DHR12L1   0x40007408  //右对齐寄存器地址
void dma_config(void); 



#endif


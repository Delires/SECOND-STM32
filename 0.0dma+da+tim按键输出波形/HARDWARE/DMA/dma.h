#ifndef __DMA_H
#define __DMA_H	 
#include "sys.h"	    

#define n 100 //���鳤��
#define f 5 //Ƶ��
#define k 0.7 //ռ�ձ�
   
	 
extern float data[n];    //������������
extern u16 dma_data[n];
//#define DAC_DHR12L1   0x40007408  //�Ҷ���Ĵ�����ַ
void dma_config(void); 



#endif


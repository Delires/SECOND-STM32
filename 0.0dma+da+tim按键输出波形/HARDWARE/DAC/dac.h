#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	    

void GPIOA_4(void); //DAC1 PA4���ų�ʼ��	 	 
void Dac1_Init(void);       //DA��ʼ��

void ConstantVol(float vol);  //���30����ѹ  ����0~3.3v
void TriangleVol(void);   //���ǲ�
void SawVol(void);        //��ݲ�
void SquareVol(void);        //����





#endif


















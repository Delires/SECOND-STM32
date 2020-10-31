#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	    

void GPIOA_4(void); //DAC1 PA4引脚初始化	 	 
void Dac1_Init(void);       //DA初始化

void ConstantVol(float vol);  //输出30个恒压  输入0~3.3v
void TriangleVol(void);   //三角波
void SawVol(void);        //锯齿波
void SquareVol(void);        //方波





#endif


















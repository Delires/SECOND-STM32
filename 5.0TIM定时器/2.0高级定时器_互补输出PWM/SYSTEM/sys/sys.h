#ifndef __sys_h__
#define __sys_h__
#include "stm32f10x.h"

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0X2000000+((addr & 0XFFFFF)<<5)+(bitnum<<2))
//把“位带地址+位序号”转换成 位带别名地址的宏
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口输出地址映射
//通过某种算法，进制寄存器位的映射，映射成一种地址。

//IO口输出地址映射
#define GPIOA_ODR_Addr        (GPIOA_BASE+12)             //0X4001080C
#define GPIOB_ODR_Addr        (GPIOB_BASE+12)    
#define GPIOC_ODR_Addr        (GPIOC_BASE+12)  
#define GPIOD_ODR_Addr        (GPIOD_BASE+12)  
#define GPIOE_ODR_Addr        (GPIOE_BASE+12)    
#define GPIOF_ODR_Addr        (GPIOF_BASE+12)    
#define GPIOG_ODR_Addr        (GPIOG_BASE+12)

//IO口输入地址映射
#define GPIOA_IDR_Addr        (GPIOA_BASE+8)
#define GPIOB_IDR_Addr        (GPIOB_BASE+8)
#define GPIOC_IDR_Addr        (GPIOC_BASE+8)
#define GPIOD_IDR_Addr        (GPIOD_BASE+8)
#define GPIOE_IDR_Addr        (GPIOE_BASE+8)
#define GPIOF_IDR_Addr        (GPIOF_BASE+8)
#define GPIOG_IDR_Addr        (GPIOG_BASE+8)

//IO口输入输出操作，只对单一引脚，n的取值范围为0~15
#define PAout(n)              BIT_ADDR(GPIOA_ODR_Addr, n) //A输出
#define PAin(n)               BIT_ADDR(GPIOA_IDR_Addr, n) //A输入

#define PBout(n)              BIT_ADDR(GPIOB_ODR_Addr, n) //B输出
#define PBin(n)               BIT_ADDR(GPIOB_IDR_Addr, n) //B输入

#define PCout(n)              BIT_ADDR(GPIOC_ODR_Addr, n) //C输出
#define PCin(n)               BIT_ADDR(GPIOC_IDR_Addr, n) //C输入

#define PDout(n)              BIT_ADDR(GPIOD_ODR_Addr, n) //D输出
#define PDin(n)               BIT_ADDR(GPIOD_IDR_Addr, n) //D输入

#define PEout(n)              BIT_ADDR(GPIOE_ODR_Addr, n) //E输出
#define PEin(n)               BIT_ADDR(GPIOE_IDR_Addr, n) //E输入

#define PFout(n)              BIT_ADDR(GPIOF_ODR_Addr, n) //F输出
#define PFin(n)               BIT_ADDR(GPIOF_IDR_Addr, n) //F输入

#define PGout(n)              BIT_ADDR(GPIOG_ODR_Addr, n) //G输出
#define PGin(n)               BIT_ADDR(GPIOG_IDR_Addr, n) //G输入


#endif


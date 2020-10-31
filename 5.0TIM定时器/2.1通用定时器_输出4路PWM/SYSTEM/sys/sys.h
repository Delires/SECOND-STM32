#ifndef __sys_h__
#define __sys_h__
#include "stm32f10x.h"

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0X2000000+((addr & 0XFFFFF)<<5)+(bitnum<<2))
//�ѡ�λ����ַ+λ��š�ת���� λ��������ַ�ĺ�
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�������ַӳ��
//ͨ��ĳ���㷨�����ƼĴ���λ��ӳ�䣬ӳ���һ�ֵ�ַ��

//IO�������ַӳ��
#define GPIOA_ODR_Addr        (GPIOA_BASE+12)             //0X4001080C
#define GPIOB_ODR_Addr        (GPIOB_BASE+12)    
#define GPIOC_ODR_Addr        (GPIOC_BASE+12)  
#define GPIOD_ODR_Addr        (GPIOD_BASE+12)  
#define GPIOE_ODR_Addr        (GPIOE_BASE+12)    
#define GPIOF_ODR_Addr        (GPIOF_BASE+12)    
#define GPIOG_ODR_Addr        (GPIOG_BASE+12)

//IO�������ַӳ��
#define GPIOA_IDR_Addr        (GPIOA_BASE+8)
#define GPIOB_IDR_Addr        (GPIOB_BASE+8)
#define GPIOC_IDR_Addr        (GPIOC_BASE+8)
#define GPIOD_IDR_Addr        (GPIOD_BASE+8)
#define GPIOE_IDR_Addr        (GPIOE_BASE+8)
#define GPIOF_IDR_Addr        (GPIOF_BASE+8)
#define GPIOG_IDR_Addr        (GPIOG_BASE+8)

//IO���������������ֻ�Ե�һ���ţ�n��ȡֵ��ΧΪ0~15
#define PAout(n)              BIT_ADDR(GPIOA_ODR_Addr, n) //A���
#define PAin(n)               BIT_ADDR(GPIOA_IDR_Addr, n) //A����

#define PBout(n)              BIT_ADDR(GPIOB_ODR_Addr, n) //B���
#define PBin(n)               BIT_ADDR(GPIOB_IDR_Addr, n) //B����

#define PCout(n)              BIT_ADDR(GPIOC_ODR_Addr, n) //C���
#define PCin(n)               BIT_ADDR(GPIOC_IDR_Addr, n) //C����

#define PDout(n)              BIT_ADDR(GPIOD_ODR_Addr, n) //D���
#define PDin(n)               BIT_ADDR(GPIOD_IDR_Addr, n) //D����

#define PEout(n)              BIT_ADDR(GPIOE_ODR_Addr, n) //E���
#define PEin(n)               BIT_ADDR(GPIOE_IDR_Addr, n) //E����

#define PFout(n)              BIT_ADDR(GPIOF_ODR_Addr, n) //F���
#define PFin(n)               BIT_ADDR(GPIOF_IDR_Addr, n) //F����

#define PGout(n)              BIT_ADDR(GPIOG_ODR_Addr, n) //G���
#define PGin(n)               BIT_ADDR(GPIOG_IDR_Addr, n) //G����


#endif


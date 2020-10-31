#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h"

//0,��֧��ucos
//1,֧��ucos
#define SYSTEM_SUPPORT_OS		0		//����ϵͳ�ļ����Ƿ�֧��UCOS
																	    
	 
//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0x00FFFFF)<<5)+(bitnum<<2))   //��λ����ת��Ϊλ����������ֵ ��������
                            
/*
 *addr & 0xF0000000��ȡ��ַ�ĸ�4λ��������2����4����������SRAM�������ַ��
 *�����2��+0x02000000��=0X2200 0000������SRAM�������4��+0x02000000��=0X4200 0000����������
 *
 *addr & 0x000FFFFFF�����ε�����λ���൱��-0X2000 0000����-0X4000 0000�������ʾƫ��λ�������ٸ��ֽ�
 *<<5  ����*8*4����Ϊλ����һ����ַ��ʾһ���ֽڣ�һ���ֽ���8��bit��һ��bit�������ͳ�һ���֣���4���ֽ�
 *<<2 ����*4����Ϊһ��λ�������ͳ�һ���֣���4���ֽ�
 *
 *�ֽ��������ʽӦ�þ���������
 *SRAMλ��������ַ
 *AliasAddr= 0x22000000+((A-0x20000000)*8+n)*4 =0x22000000+ (A-0x20000000)*8*4 +n*4
 *����λ��������ַ
 *AliasAddr= 0x22000000+((A-0x20000000)*8+n)*4 =0x22000000+ (A-0x20000000)*8*4 +n*4
 */

#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))  //��addr�����ֵת��Ϊ ָ���ȡֵ
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
                                  //BITBAND(addr, bitnum) ��λ����ת��Ϊλ����������ֵ
                                  //MEM_ADDR(xxx)�ٰ��������ֵת��Ϊ �����Ż��޷��ŵ�32λ��ָ������

//IO�ڵ�ַӳ�� 12d��ODR��ƫ�Ƶ�ַ0x0Ch
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C   //12d��ODR��ƫ�Ƶ�ַ0x0Ch
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C   
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808    //8d��ODR��ƫ�Ƶ�ַ0x08h
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� //BIT_ADDR(GPIOA_ODR_Addr,n) = *((volatile unsigned long*)((GPIOA_ODR_Addr & 0xF0000000)+0x2000000+((GPIOA_ODR_Addr &0x00FFFFF)<<5)+(bitnum<<2))
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ

#endif

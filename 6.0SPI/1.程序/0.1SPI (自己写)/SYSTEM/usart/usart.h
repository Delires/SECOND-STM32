#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define _TXET                1
	 
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);

/**
#define DEBUG(format, ...) printf (format, ##__VA_ARGS__)
   ��' ## '����˼�ǣ�����ɱ���������Ի�Ϊ�գ���ʹԤ�������� preprocessor ��ȥ������ǰ����Ǹ����š���
**/
#ifdef   _TXET
#define  TXET(message, arg...)  printf(" �ļ���:[%s] ����:[%d] ����[%s] ʱ��[%s]"message" <<Good>>",__FILE__,__LINE__,__DATE__,__TIME__,##arg) //TXET(format, arg...)  printf("FILE: "__FILE__", LINE: %d: "format"\n", __LINE__, ##arg)  
                                                                                       //##arg��ʾ�궨���������ֵ
																					   //format��ʾ�궨�嶺��ǰ���ݵĲ���
																					   //"FILE: "__FILE__", LINE: %d: "��ʾ����ֵ
          
#else
#define  TXET(info)  
#endif


#endif



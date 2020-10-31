#ifndef __EEpromI2C_H
#define __EEpromI2C_H	 
#include "sys.h"

/**Ϊ������ֲ��  �ú궨��  �ı䴮��ֱ�Ӹı�꼴�� **/
//I2C����
#define           EEPROM_Address                0xA0  //8λ��ַ
#define           STM32_I2C_OWM_Address         0x5f//����һ��32�Լ��ĵ�ַ(7λ) ���EEPROMһ��

/********** eeprom һҳ��С �����ڴ�****************/
#define           EEPROM_PAGE_SIZE              8
#define           EEPROM_SIZE                   256



/********debug ���� ���������Ϣ**********************/
#define EEPROM_DEBUG(message,agr...)  printf("\r\nDebug: file:[%s] line:[%d] Message:"message" \r\n",__FILE__, __LINE__, ##agr)

//printf�궨�� ��ӡ������Ϣ
#define EEPROM__ERROR(message,agr...)    printf("<I2C ERROR>"message"\r\n",##agr);



/********����������**********************/
u8 ee_CheckOk(void);  //����豸���ں���

u8 ee_ReadBytes(u8 SaveAddress, u8 *ReadBuf, u16 Size); //�����ݺ���
u8 ee_WriteBytes(u8 SaveAddress, u8 *WriteBuf, u16 Size); //д���ݺ���


void ee_Delay(__IO uint32_t nCount); //��ʱ����

#endif





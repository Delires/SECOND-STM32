#ifndef __key_h__
#define __key_h__

//�궨�� ����KEYΪ��ȡ�˿�ֵ
#define KEY1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) //GPIO_ReadInputDataBit��ȡ����ֵ����
#define KEY2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)
#define KEY3 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define KEY4 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)

void KEY_Init(void);  //������ʼ������

int KEY_Scan(void); //�����ֵ����  u8����Ϊ�˺����з���ֵ
                   //              void��ʾ�κ���ʹ��ʱ���ø�ֵ


#endif

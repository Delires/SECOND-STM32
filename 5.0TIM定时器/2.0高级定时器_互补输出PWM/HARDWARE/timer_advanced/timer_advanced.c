#include "timer_advanced.h"
/******************************************************************/
/**********�˺�������TIM61/8�߼���ʱ�����������PWM��**************/
/**********������ʱ��  ��ɲ�����Ƶ�**************/
/******************************************************************/
/**********�˺�������TIM61/8�߼���ʱ�����������PWM��**************/
/**
���� TIN1_CH1--PA8  TIN1_CH1N--PB13�������PWM   ɲ��BKIN---PB12  
                       ����PB13 PB12������������оƬ����PWM�����ë��(оƬ����������)
ԭ��:CNT������CRR���Ƹߵ�ƽʱ��(��ӦCRRx�Ĵ���)
              ARR����PWM����(��ӦARR�Ĵ���)
			  ʱ��������ʱһ��ʱ��(PSC�Ĵ���)

���·��:     ʹ��(��Ͳ��ù� ʱ��ʹ�ܣ�����ʹ��)
              ����GPIO(��ΪGPIO���PWM)   GPIO_Init()
              ��ʼ����ʱ������ʱ��        TIM_TimeBaseInit() 
			  ��ʼ������/�������         TIM_OC1Init()
                             			ʹ��RCC  TIM_OC1PreloadConfig() 

			  ��ʼ��ɲ������              TIM_BDTRConfig()     Bakeup DeadTime
			  �ж����ȼ�����              NVIC_Init()
**/

/**
����GPIO���  TIN1_CH1--PA8  1
              TIN1_CH1N--PB13�������PWM   
			  ɲ��BKIN---PB12 
��Ϊǣ����GPIO��������������Ҫʹ�ø��ù���
**/
static void TIM_Advanced_GPIO_Config(void)   //���������static��ֻ������������ܵ���
{
	GPIO_InitTypeDef GPIO_InitStruct; //����һ���ṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   //����GPIOA�Ļ� ֱ��ʹ����Ӧ���ܼ���	

//����TIN1_CH1--PA8 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  //�������� ���쾢��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);  //ȡ�ṹ���ַ 

//TIN1_CH1N--PB13 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  //�������� ���쾢��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);  //ȡ�ṹ���ַ 
	
//ɲ��BKIN---PB12   ɲ��ΪëҲ���ó������(ɲ��ҲҪ���ó����)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  //�������� ���쾢��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);  //ȡ�ṹ���ַ 

//ɲ��������Ϊ�͵�ƽ ����ʲô��ƽ��ɲ����mode��������� �������õ��Ǹߵ�ƽɲ��
	GPIO_ResetBits(GPIOB,GPIO_Pin_12); 

//	
}







/**
���ø߼���ʱ���� ģʽ�� ɲ������
����ʱ��
arr��װ��ֵ, pscԤ����, high�ߵ�ƽ����(�ߵ�ƽ��װ��ֵ)
**/
static void TIM_Advanced_Mode_Config(u16 arr, u16 psc, u16 high)
{
	 TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;   //ʱ�����ýṹ��
     TIM_OCInitTypeDef TIM_OCInitStruct;               //PWM���ýṹ��
     TIM_BDTRInitTypeDef TIM_BDTRInitStruct;	       //ɲ��+����ʱ�����ýṹ��

/*********************����PWMƵ��(TIMʱ��)*************************/
//����ʱ��  ʱ������PWMƵ��   
//psc=8   rcc=7  PWM����=(1/(72/psc))*arr=(psc*arr)/72M    ����PWMƵ��=72M/(psc*arr)
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//��������ʱ��Ҫ�õ�Tds=CKD*Tck_int
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; //��ʹ���ظ��Ĵ���
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct); 

/*********************����TIM1��OC1����·���PWM1  PWMռ�ձ�*************************/	
// ����ΪPWMģʽ1
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
// ���ͨ�� ���е�ƽ�������� �� ��Ч��ƽ��ƽ��������
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set; //PWM��ɲ������ ��ʹ��ɲ��PWMֹͣ�����ֹͣ���ʱCHx����ʱ������ߵ�ƽ
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;   //CHx�ߵ�ƽ��Ч ��CNT<RCC����ߵ�ƽ
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//���ͨ��ʹ��
	
// �������ͨ�� ���е�ƽ�������� �� �����Ч��ƽ�ļ�������
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //ʹ��ɲ��PWMֹͣ�����ֹͣ���ʱCHxN����ʱ������ߵ�ƽ
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;  //CHxN�ߵ�ƽ��Ч ��CNT<RCC����ߵ�ƽ
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;//�������ʹ��
	
	TIM_OCInitStruct.TIM_Pulse = high;// (psc/high);       //���ֵ/arr����ռ�ձ� ��Ϊ�õ���ͬһʱ�� arr���ܵļ�������
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);

//Ԥ�����Ƿ�ʹ��,��д��ARR�Ƿ�������Ч
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
/************************ ��ʼ��ɲ������ ����ʱ��**************************/	
    TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_1;                 //�����ܸñ�ɲ��������   �� ɲ�����ƽ���״̬ 
    TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Enable;  //����ģʽ�� ��ʱ��������ʱ ������е�ƽ 
	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Enable;  //���ģʽ�� ��ʱ��������ʱ �����Ч��ƽ
//����ʱ�� 
	TIM_BDTRInitStruct.TIM_DeadTime = 11;  //��������ʱ�� 11d=0000 1011b ���ֲ�֪DTG[7:5]=0xx => DT=DTG[7:0]��Tdtg��Tdtg = TDTS��   TDTS=CKD*TCK_INT(��CR1�Ĵ���) 
	                                        //11*Tdtg=11/72M=0.152ns   ����ʱ����ݰ���/ȫ��MOS�ܵ�����ȥȷ�����ʱ��
//ɲ��
    TIM_BDTRInitStruct.TIM_Break = TIM_Break_Enable; //ɲ��ʹ��
	TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_High;  //ɲ�����Ըߵ�ƽ ��PB12Ϊ��ʱPWMֹͣ���
	 TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable; //�Զ����ʹ��  ���ɲ����Ч��һ�������¼��Զ����
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStruct);

/************** ������ ʹ��ʱ��+����+ʹ�ܹ��� ****************************/
//ʹ�ܶ�ʱ��
   TIM_Cmd(TIM1, ENABLE);
//�߼���ʱ�����е� ����OCx ��OCxN����� �������     ͨ�ö�ʱ������Ҫ���ʹ�� MOEλ
   TIM_CtrlPWMOutputs(TIM1, ENABLE); 

}


//218





/**
��ʱ����ʼ��Init������ǰ��������
ע��xxx_Config��xxx_Init��һ��  ǰ���������������   ��������������ٵ���ǰ��ȥ��ʼ��Init
**/
void TIM_Advanced_Mode_Init(void)
{
	TIM_Advanced_GPIO_Config(); 
	TIM_Advanced_Mode_Config(8, 7, 4);
	                      //�������u16 arr, u16 psc, u16 high
	                      //arr��װ��ֵ  high�ߵ�ƽʱ��   (arr+1)/high=PWMռ�ձ�
	                      //pscԤ����ϵ��   PWMƵ��=72M/((arr+1)*(psc+1))
	//TIM_Advanced_Mode_Config(500, 600, 150);  //��������PWM��Ƶ��
}
//89 79 PWM 0.1ms 10khz    ����ʱ��0.152us  ��Ȼ������        
//8 7 pwmһ������1us  �������ܿ�������ʱ��
//�����嵽������������ �嵽P20��  ע��PWMƵ��̫��̫С�˶�������
//TIM_Advanced_Mode_Config(8, 7, 4); ���ú����������Σ�����ĻḲ��ǰ��� ���������Ե�

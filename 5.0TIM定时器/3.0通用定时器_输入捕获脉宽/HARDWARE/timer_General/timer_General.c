#include "timer_General.h"
/******************************************************************************************************/
/**********������ʱ��TIM2 ����������*****************************************************************/
/**********���˼·:TIM2��CH1����PA0,PA0����WK_UP�����պ������һ������ߵ�ƽ**************************/
/**********�������WK_UP���� ����TIM2��CH1Ϊ���벶��***************************************************/
/******************************************************************************************************/
/******************************************************************************************************/

/**
����GPIO  PA0---CH1 PA0���벶��   ���ó�ʲô��ѯ�ֲ�8.1.11
**/
static void TIM_General_GPIO_Config(void)   //���������static��ֻ������������ܵ���
 {
    GPIO_InitTypeDef GPIO_InitStruct;  
 
//ʹ��ʱ��+����+��������������(gpio���ò����ڿ�������)
	 /**������Ĵ���  **/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;  //�ĳ��������� ��ΪҪ��ȡ�ߵ�ƽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	 
	GPIO_ResetBits(GPIOA, GPIO_Pin_0); //wk_up���������������أ��������ǰ����
}
  

/**
����TIM2 ʱ��
RCC����װ��ֵ   PSCԤ����ϵ��
**/
static void TIM_General_Base_Config(u16 arr, u16 psc)   //���������static��ֻ������������ܵ���
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //��ʹ��һ��TIM2  
//����TIM2ʱ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //�����ú�Tdts�й� CR1 CKDλ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);  
//ʹ�ܹ���
	TIM_Cmd(TIM2, ENABLE); 
	
}


/**
����TIM2��CH1Ϊ���벶��
**/
static void TIM_General_Capture_Config()   
{
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct; 

	//�������벶��
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1; //TIM2��CH1
	TIM_ICInitStruct.TIM_ICFilter = 0x00;       //���벶����� 
	              //����������ź�Ϊ1M���ִ��ڸ�Ƶ���źŸ��ţ���ô��ʱ�ͺ��б�Ҫ�����˲������ǿ������ò���Ƶ��Ϊ2M�������ڱ�֤��������Ч�źŵĻ����ϰѸ���2M �ĸ�Ƶ�����źŹ��˵���
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; //���������أ��ж���Ҫ��
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
                        	//����ÿ���ٴδ����¼����㲶��һ��
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;//���벶��ͨ��ICx����������TIx��ӳ���ϵ ֱ�� PWMҪ��ֱ��
    TIM_ICInit(TIM2, &TIM_ICInitStruct);  
	
                      
//�����ж����ȼ�
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&NVIC_InitStruct);  

    TIM_ClearFlag(TIM2, TIM_FLAG_CC1|TIM_FLAG_Update); //�����һ���жϱ�־  TIM_ClearITPendingBit()�������Ҳ����
                                                  //����жϷ�ֹ���
/**���˳���ȱ�ⲿ��**/
    TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_CC1, ENABLE);  //ʹ�ܲ���͸����ж� 

    TIM_Cmd(TIM2, ENABLE); 
}



/**
�жϷ����� 
��ȡ����ʱ��  ע��:��Ϊֻ�ж�ʱ���жϣ��������������½����ж�
                   ���Ҫ�ü�����־λ���ж�����/�½����ж�  �жϲ������
 debug�� printf("2"); 
**/
/***************************����˼·***********************************/
#if( Myslef_Open )
//����һ���ṹ��  �˴��Ƿ����ڴ�� ����ROM�ľ�̬��
GPIO_Genral_Capture Capture_value={0, 0, 0, 0};
/**�ж϶�����ȥΪʲô�أ� RCC TIM2����������������ʹ�ܾͺ���**/

void TIM2_IRQHandler(void)
{
//�ж� if�ж�+��ʲô+����ж�
	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update)==1) //����ߵ�ƽ�����ж�
	{
		//printf("1");     //debug��
		
        Capture_value.Period_Value ++;  
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	
	}
	
	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_CC1)==1) //�����/�͵�ƽ�����ж�
	{
		//printf("2");     //debug��

	   if(Capture_value.CaptureStart_Bit == 0) //û�в���������
	   {
		   Capture_value.CaptureStart_Bit = 1;  //�����ر�־λ����Ϊ1
	       Capture_value.CaptureStop_Bit = 0;  //������־λΪ0
		   
		   TIM_SetCounter(TIM2, 0);  //���ü�ʱ��Ϊ0
		   Capture_value.Period_Value = 0;
		   Capture_value.Time_Value = 0;
		   
		   TIM_OC1PolarityConfig(TIM2, TIM_OCPolarity_Low); //����Ϊ�͵�ƽ����	   
	   }
	   else //if�ж�ʱ�Ѱ�CaptureStart_Bit��Ϊ1��
	   {
		   //Capture_value.Time_Value =TIM_GetCounter(TIM2);  //�������Ǹ�һ����
		   Capture_value.Time_Value=TIM_GetCapture1(TIM2);
		 
		   TIM_OC1PolarityConfig(TIM2, TIM_OCPolarity_High);//����Ϊ�ߵ�ƽ����
		   
		   Capture_value.CaptureStart_Bit = 0;  //�����ر�Ϊ0�ȴ��´β���
		   Capture_value.CaptureStop_Bit=1;//������־λ
	   }
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);	
	}
	
}
#endif




/***************************����ԭ�� û�˱����ĸ��㣡������*************************************/
/**
    ������������¼����ط����ٳ�����ִ��else ��ִ��if
                            ����λ��ֵ����ֵ����ô˵�ء�����һ���� �����ĵ�7λ����Ϊ��ʼ���ж�λ
                                                                   �����ĵ�6λ����Ϊ����������ж�λ
                                                                   ������5~0λ����Ϊ�������ڼ�������λ
                            ��֪�������ǹ����㻹�������Ĵ����Ҫ�Ѵ���д�ÿ���̫��
**/
#if( ZhengDian_Open )
u8 TIM2CH1_CAPTURE_STA=0;
//TIM2CH1_CAPTURE_STA=0X04������������   0x08���������  5~0λ�������ڵĴ���
u8 TIM2CH1_CAPTURE_VAL=0;
void TIM2_IRQHandler(void)
{
//�ж� if�ж�+��ʲô+����ж�
	if( (TIM2CH1_CAPTURE_STA&0x80)==0 ) //�жϲ����Ƿ�ɹ� ����δ���������ж�
	{
		if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update)==1)//�����ж�
		{
			if( TIM2CH1_CAPTURE_STA&0x40 )//������������أ���֤�����峬��һ����ʱ����
		     {
		 	      if( (TIM2CH1_CAPTURE_STA&0x3F)==0x3F ) //������峤�ȳ���ʱ�� ֱ��ǿ�����
				  {
					  TIM2CH1_CAPTURE_VAL|=0xFFFF;
					  TIM2CH1_CAPTURE_STA|=0x80;  //ǿ�Ʋ������
				  }
				  else
				  {
					  TIM2CH1_CAPTURE_STA++;  //һֱ�ӵ�0x3F����
				  }
		     }
		}

		if( TIM_GetFlagStatus(TIM2, TIM_FLAG_CC1)==1  )//�����ж�
		{
		 if( TIM2CH1_CAPTURE_STA&0x40) //����������
			{
				 TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2);
				 TIM2CH1_CAPTURE_STA|=0x80; //���Ϊ�������
				
				 TIM_OC1PolarityConfig(TIM2, TIM_OCPolarity_High);//����Ϊ�ߵ�ƽ����
				
			}
			else//û�в���������
			{
				 TIM2CH1_CAPTURE_STA=0;//��һ�β��񵽵�ƽ����ֵ��0
                 TIM2CH1_CAPTURE_VAL=0;
				
				 TIM_SetCounter(TIM2, 0);//�����ʱ��
				 TIM_OC1PolarityConfig(TIM2, TIM_OCPolarity_Low); //����Ϊ�͵�ƽ����
				 TIM2CH1_CAPTURE_STA|=0x40;//״̬λ�����
			}
		}
		
	}
	
	
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //����ж�
	
}
#endif













/********************��ʼ����ʱ�� ���� ʱ�� ���벶������*********************/
void TIM_General_Capture_Init(void)
{
	TIM_General_GPIO_Config();
	TIM_General_Base_Config(65535, 71); //pscԽСԽ��ȷ  ��ʱ����6553us=6.5ms   ��ʱһ��1ms
	TIM_General_Capture_Config();  
}


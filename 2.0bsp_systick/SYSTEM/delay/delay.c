#include "delay.h"
/***********
���stm32����osϵͳ(ϵͳҪsysytick�ṩʱ��Ƭ)������systick��ʱ����ժȡ����ժȡsystick�ı仯�������м�ʱ
���û��os����ֱ�Ӷ�systick�������������������Ӷ����м�ʱ
*****************/

//�����Ҫʹ��OS,����������ͷ�ļ�����.
//ʹ��os�ĳ�ʼ��
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

static u8  fac_us=0;							//us��ʱ������,����ʱ��ʱ��1usת����sysҪ����������			   
static u16 fac_ms=0;							//��ʱ��msת����sysҪ����������
	
	
#if SYSTEM_SUPPORT_OS							//���SYSTEM_SUPPORT_OS������,˵��Ҫ֧��OS��(������UCOS).
//��delay_us/delay_ms��Ҫ֧��OS��ʱ����Ҫ������OS��صĺ궨��ͺ�����֧��
//������3���궨��:
//    delay_osrunning:���ڱ�ʾOS��ǰ�Ƿ���������,�Ծ����Ƿ����ʹ����غ���
//delay_ostickspersec:���ڱ�ʾOS�趨��ʱ�ӽ���,delay_init�����������������ʼ��systick
// delay_osintnesting:���ڱ�ʾOS�ж�Ƕ�׼���,��Ϊ�ж����治���Ե���,delay_msʹ�øò����������������
//Ȼ����3������:
//  delay_osschedlock:��������OS�������,��ֹ����
//delay_osschedunlock:���ڽ���OS�������,���¿�������
//    delay_ostimedly:����OS��ʱ,���������������.

//�����̽���UCOSII��UCOSIII��֧��,����OS,�����вο�����ֲ
//֧��UCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD������,˵��Ҫ֧��UCOSII				
#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNesting		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif


//֧��UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD������,˵��Ҫ֧��UCOSIII	
#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNestingCtr		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif


//us����ʱʱ,�ر��������(��ֹ���us���ӳ�)
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   				//ʹ��UCOSIII
	OS_ERR err; 
	OSSchedLock(&err);							//UCOSIII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#else											//����UCOSII
	OSSchedLock();								//UCOSII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#endif
}

//us����ʱʱ,�ָ��������
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   				//ʹ��UCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);						//UCOSIII�ķ�ʽ,�ָ�����
#else											//����UCOSII
	OSSchedUnlock();							//UCOSII�ķ�ʽ,�ָ�����
#endif
}

//����OS�Դ�����ʱ������ʱ
//ticks:��ʱ�Ľ�����
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);	//UCOSIII��ʱ��������ģʽ
#else
	OSTimeDly(ticks);							//UCOSII��ʱ
#endif 
}
 
//systick�жϷ�����,ʹ��ucosʱ�õ�
void SysTick_Handler(void)
{	
	if(delay_osrunning==1)						//OS��ʼ����,��ִ�������ĵ��ȴ���
	{
		OSIntEnter();							//�����ж�
		OSTimeTick();       					//����ucos��ʱ�ӷ������               
		OSIntExit();       	 					//���������л����ж�
	}
}
#endif




//systick��ʼ�� os�ĳ�ʼ�� �� ��ͨ��ʼ��
void delay_init(void)
{

#if SYSTEM_SUPPORT_OS  //�ж�����os
	u32 reload; //����װ��ֵ
#endif
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //��sysһ��ʱ��9mhz ���ʱһ��1/9us
	fac_us=SystemCoreClock/8000000; //��fac_us�Ļ�׼��Ҫ��ʱ9�� �ŵ���1ms

#if SYSTEM_SUPPORT_OS  //�ж�����os
	reload = SystemCoreClock/8000000; //1us������Ĵ�ʱ
	reload*=1000000/delay_ostickspersec;		//����delay_ostickspersec�趨���ʱ��
												//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
	fac_ms=1000/delay_ostickspersec;			//����OS������ʱ�����ٵ�λ	 
  
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;//����systick�쳣���� λ1
	SysTick->LOAD |= reload;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //����ʹ��

#else //��os��
	fac_ms = (u16)fac_us*1000; //msʱ����Ҫ�ļ�ʱ����
#endif	

}


#if SYSTEM_SUPPORT_OS  							//�����Ҫ֧��OS.
//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD��ֵ	    	 
	ticks=nus*fac_us; 							//��Ҫ�Ľ�����	  		 
	tcnt=0;
	delay_osschedlock();						//��ֹOS���ȣ���ֹ���us��ʱ
	told=SysTick->VAL;        					//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
	delay_osschedunlock();						//�ָ�OS����									    
}
//��ʱnms
//nms:Ҫ��ʱ��ms��
void delay_ms(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)	//���OS�Ѿ�������,���Ҳ������ж�����(�ж����治���������)	    
	{		 
		if(nms>=fac_ms)							//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			delay_ostimedly(nms/fac_ms);		//OS��ʱ
		}
		nms%=fac_ms;							//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((u32)(nms*1000));					//��ͨ��ʽ��ʱ  
}





#else //����OSʱ
//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{
	u32 temp; //����һ����ʱ�������ڼ��sys�Ĵ����ı仯
	SysTick->LOAD = nus*fac_us;//���ؼ�����ֵ=��ʱ������*1us��ʱ����ʱ����
	SysTick->VAL = 0x00;   //sys�Ĵ���ֵ��0
  SysTick->CTRL |= 0x01; //SysTick_CTRL_ENABLE_Msk; //�Ĵ���ʹ��  sys��һ��������24λ�Ĵ���
	do   
	{
		temp = SysTick->CTRL;//�ѵ�ǰ�Ĵ���״̬��ֵ���� temp ͨ�����temp���Ĵ���״̬
	}while( (temp&0x01) && !(temp&(1<<16)) );  //(temp&0x01)��ʾsys�ٴ�  temp&(1<<16)û������һֱ��0
	
	SysTick->CTRL &= ~0x01; //�򿪼�ʱ����λȡ�� Ȼ����0 �رռ�����
	SysTick->VAL = 0x00; //sys�Ĵ�����0
	
}

void delay_ms(u16 nms) //������ʱ����
{
	u32 temp;
	SysTick->LOAD = (u32)nms*fac_ms;  //����ʱ1msӦ�õļ�ʱ����
	SysTick->VAL = 0x00; //ֵ����
	SysTick->CTRL |= 0x01;  //ʹ�ܼ�ʱ��
	do
	{
		temp = SysTick->CTRL;
	}while( (temp&0x01) && !(temp&(1<<16)) );  //sys�� && sys����δ��0
	
	SysTick->CTRL &= ~0x01; //�رռ�ʱ��
	SysTick->VAL=0x00; //����
	
}
#endif 



#include "bsp_systic.h"

#if(0)
/***�ο�����**/
//static+������ �����˺������ܱ����� ʹ��ֻ���ڶ�������������ʹ��
//��static�������ں����У���ֻ����������ܷ����������ں������ֻ������ļ��ܷ�������
//��ͬ������static����������� ��Ϊû�н���ȫ�����ֿռ�

//������ __INLINE(inline) ���庯��Ϊ���� ʹ�ú������Ե���
//�ٸ�����  ���ú���:��ջ����,��ȡ�� ����ѭ���ý���ջ�ö��
//          inline�Ļ�  ���ú����Ļ� �൱�ں궨��������дһ���������  �����ظ�����ջ
//           ����������ʹ��������  ��inline����д�ں�������ǰ�����Ǻ�������ǰ
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{ 
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
                             //д���ֵ����reload��ֵ24λ��Υ��ֵ ����1                                    
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
	                        //��reloadֵ���ؽ��Ĵ��� Ҫ��һλ���Լ�1 
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
	                        //�������ȼ�������Ϊ16-1=15=1111b  Ȼ�����ȼ�����ֻ�ֳܷ�2��  ��ռ�������ȷֱ�Ϊ3��3  
	                        //��ֻ������һ����������NVIC����ռ�ͱȽ����ȼ�������ȥ�����������������ȼ������Ƕ���
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
                            //д�����ֵ
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
	            //����ʱ��72M
                   SysTick_CTRL_TICKINT_Msk   |
                //ʹ���ж�	
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
                //ʹ��sysytick
	return (0);                                                  /* Function successful */
}
#endif
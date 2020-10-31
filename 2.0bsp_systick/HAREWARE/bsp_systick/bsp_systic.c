#include "bsp_systic.h"

#if(0)
/***参考函数**/
//static+函数名 表明此函数不能被调用 使用只能在定义的这个函数内使用
//即static变量放在函数中，就只有这个函数能访问它；放在函数外就只有这个文件能访问它。
//不同函数的static定义可以重名 因为没有进入全局名字空间

//但由于 __INLINE(inline) 定义函数为内联 使得函数可以调用
//举个例子  调用函数:进栈调用,在取出 若是循环得进出栈好多次
//          inline的话  调用函数的话 相当于宏定义在这重写一遍这个函数  不用重复进出栈
//           函数过长不使用于内联  ，inline必须写在函数定义前而不是函数声明前
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{ 
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
                             //写入的值大于reload的值24位则违法值 返回1                                    
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
	                        //把reload值加载进寄存器 要进一位所以减1 
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
	                        //配置优先级，配置为16-1=15=1111b  然后按优先级分组只能分成2组  抢占和子优先分别为3和3  
	                        //他只能配置一个数，按照NVIC的抢占和比较优先级的配置去解析他，看他的优先级到底是多少
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
                            //写入计数值
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
	            //配置时钟72M
                   SysTick_CTRL_TICKINT_Msk   |
                //使能中断	
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
                //使能sysytick
	return (0);                                                  /* Function successful */
}
#endif
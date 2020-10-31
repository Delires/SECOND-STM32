#include "delay.h"
/***********
如果stm32里有os系统(系统要sysytick提供时间片)，则用systick延时就用摘取法，摘取systick的变化次数进行计时
如果没有os，则直接对systick操作，让它倒数计数从而进行计时
*****************/

//如果需要使用OS,则包括下面的头文件即可.
//使用os的初始化
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

static u8  fac_us=0;							//us延时倍乘数,即延时的时间1us转换成sys要计数多少下			   
static u16 fac_ms=0;							//延时的ms转换成sys要计数多少下
	
	
#if SYSTEM_SUPPORT_OS							//如果SYSTEM_SUPPORT_OS定义了,说明要支持OS了(不限于UCOS).
//当delay_us/delay_ms需要支持OS的时候需要三个与OS相关的宏定义和函数来支持
//首先是3个宏定义:
//    delay_osrunning:用于表示OS当前是否正在运行,以决定是否可以使用相关函数
//delay_ostickspersec:用于表示OS设定的时钟节拍,delay_init将根据这个参数来初始哈systick
// delay_osintnesting:用于表示OS中断嵌套级别,因为中断里面不可以调度,delay_ms使用该参数来决定如何运行
//然后是3个函数:
//  delay_osschedlock:用于锁定OS任务调度,禁止调度
//delay_osschedunlock:用于解锁OS任务调度,重新开启调度
//    delay_ostimedly:用于OS延时,可以引起任务调度.

//本例程仅作UCOSII和UCOSIII的支持,其他OS,请自行参考着移植
//支持UCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD定义了,说明要支持UCOSII				
#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OS时钟节拍,即每秒调度次数
#define delay_osintnesting 	OSIntNesting		//中断嵌套级别,即中断嵌套次数
#endif


//支持UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD定义了,说明要支持UCOSIII	
#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OS时钟节拍,即每秒调度次数
#define delay_osintnesting 	OSIntNestingCtr		//中断嵌套级别,即中断嵌套次数
#endif


//us级延时时,关闭任务调度(防止打断us级延迟)
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   				//使用UCOSIII
	OS_ERR err; 
	OSSchedLock(&err);							//UCOSIII的方式,禁止调度，防止打断us延时
#else											//否则UCOSII
	OSSchedLock();								//UCOSII的方式,禁止调度，防止打断us延时
#endif
}

//us级延时时,恢复任务调度
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   				//使用UCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);						//UCOSIII的方式,恢复调度
#else											//否则UCOSII
	OSSchedUnlock();							//UCOSII的方式,恢复调度
#endif
}

//调用OS自带的延时函数延时
//ticks:延时的节拍数
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);	//UCOSIII延时采用周期模式
#else
	OSTimeDly(ticks);							//UCOSII延时
#endif 
}
 
//systick中断服务函数,使用ucos时用到
void SysTick_Handler(void)
{	
	if(delay_osrunning==1)						//OS开始跑了,才执行正常的调度处理
	{
		OSIntEnter();							//进入中断
		OSTimeTick();       					//调用ucos的时钟服务程序               
		OSIntExit();       	 					//触发任务切换软中断
	}
}
#endif




//systick初始化 os的初始化 和 普通初始化
void delay_init(void)
{

#if SYSTEM_SUPPORT_OS  //判断有无os
	u32 reload; //重新装载值
#endif
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //给sys一个时钟9mhz 则计时一次1/9us
	fac_us=SystemCoreClock/8000000; //则fac_us的基准需要计时9次 才等于1ms

#if SYSTEM_SUPPORT_OS  //判断有无os
	reload = SystemCoreClock/8000000; //1us需计数的此时
	reload*=1000000/delay_ostickspersec;		//根据delay_ostickspersec设定溢出时间
												//reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右	
	fac_ms=1000/delay_ostickspersec;			//代表OS可以延时的最少单位	 
  
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;//开启systick异常请求 位1
	SysTick->LOAD |= reload;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开启使能

#else //非os下
	fac_ms = (u16)fac_us*1000; //ms时所需要的计时次数
#endif	

}


#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD的值	    	 
	ticks=nus*fac_us; 							//需要的节拍数	  		 
	tcnt=0;
	delay_osschedlock();						//阻止OS调度，防止打断us延时
	told=SysTick->VAL;        					//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//时间超过/等于要延迟的时间,则退出.
		}  
	};
	delay_osschedunlock();						//恢复OS调度									    
}
//延时nms
//nms:要延时的ms数
void delay_ms(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)	//如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度)	    
	{		 
		if(nms>=fac_ms)							//延时的时间大于OS的最少时间周期 
		{ 
   			delay_ostimedly(nms/fac_ms);		//OS延时
		}
		nms%=fac_ms;							//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((u32)(nms*1000));					//普通方式延时  
}





#else //不用OS时
//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{
	u32 temp; //构造一个临时变量用于检测sys寄存器的变化
	SysTick->LOAD = nus*fac_us;//加载计数器值=延时毫秒数*1us计时器计时次数
	SysTick->VAL = 0x00;   //sys寄存器值清0
  SysTick->CTRL |= 0x01; //SysTick_CTRL_ENABLE_Msk; //寄存器使能  sys是一个倒数的24位寄存器
	do   
	{
		temp = SysTick->CTRL;//把当前寄存器状态的值赋给 temp 通过检测temp检测寄存器状态
	}while( (temp&0x01) && !(temp&(1<<16)) );  //(temp&0x01)表示sys再打开  temp&(1<<16)没倒数完一直是0
	
	SysTick->CTRL &= ~0x01; //打开计时器的位取反 然后置0 关闭计数器
	SysTick->VAL = 0x00; //sys寄存器清0
	
}

void delay_ms(u16 nms) //毫秒延时函数
{
	u32 temp;
	SysTick->LOAD = (u32)nms*fac_ms;  //乘延时1ms应该的计时次数
	SysTick->VAL = 0x00; //值清零
	SysTick->CTRL |= 0x01;  //使能计时器
	do
	{
		temp = SysTick->CTRL;
	}while( (temp&0x01) && !(temp&(1<<16)) );  //sys打开 && sys倒数未到0
	
	SysTick->CTRL &= ~0x01; //关闭计时器
	SysTick->VAL=0x00; //清零
	
}
#endif 



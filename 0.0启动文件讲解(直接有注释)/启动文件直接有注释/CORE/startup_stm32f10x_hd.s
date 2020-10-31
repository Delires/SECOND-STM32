;******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
;* File Name          : startup_stm32f10x_hd.s
;* Author             : MCD Application Team
;* Version            : V3.5.0
;* Date               : 11-March-2011
;* Description        : STM32F10x High Density Devices vector table for MDK-ARM 
;*                      toolchain. 
;*                      This module performs:
;*                      - Set the initial SP                   1.初始化堆栈指针SP
;*                      - Set the initial PC == Reset_Handler  2.初始化PC指针，程序每执行一次PC指针+1   初始化PC指针=Reset_Handler 
;*                                                               程序一开始上电从151Reset handler开始执行
;*                      - Set the vector table entries with the exceptions ISR address 3.初始化中断向量表 61行开始的那些中断名
;*                      - Configure the clock system and also configure the external 
;*                        SRAM mounted on STM3210E-EVAL board to be used as data 
;*                        memory (optional, to be enabled by user) 3.配置时钟system_init() 和 挂载在STM32上的SRAM作为数据存储(此项不需要)
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).                           4.调用c语言库函数_main 最终转到c语言的编程处(跳转到你的程序)
;*                      After Reset the CortexM3 processor is in Thread mode,;CortexM3线程模式 
;*                      priority is Privileged, and the Stack is set to Main.堆栈设置成主堆栈MSP
;* <<< Use Configuration Wizard in Context Menu >>>   
;*******************************************************************************
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
; AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
; INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
; CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
; INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************
;********************启动文件用会汇编语言写的***********************************



; Amount of memory (in bytes) allocated for Stack 分配堆栈的大小
; Tailor this value to your application needs     如果你需要的话改这部分值
; <h> Stack Configuration 配置栈   堆和栈有区别，栈放变量(局部)，函数调用(当你函数要调用其它函数时，会把当前函数的地址存在栈里，当函数调用完再跳回)
;                                               堆
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

;*********** 配置栈 ************/
;*栈：放变量(局部/全局)，函数调用(当你函数要调用其它函数时，会把当前函数的地址存在栈里，当函数调用完再跳回)*/
;栈放变量(局部/全局)，函数调用(当你函数要调用其它函数时，会把当前函数的地址存在栈里，当函数调用完再跳回)
Stack_Size      EQU     0x00000400   ;EQU=define  给Stack_Size定义成0x00000400      1k   400h=1024d=1k

                AREA    STACK, NOINIT, READWRITE, ALIGN=3 ;AREA汇编一个新的代码段(相当于定义数组) STACK段名 NOINIT不初始化 READWRITE可读可写  ALIGN长度为2^ALIGN=2^3=8字节 按照8字节对齐
Stack_Mem       SPACE   Stack_Size ;分配内存大小空间(单位为字节) Stack_Mem大小为Stack_Size(1K)
	                        ;整个语句就是 分配1K的可读可写的空间给 STACK这个段
__initial_sp ;/表示栈的结束地址，即栈顶地址，栈是由高向低生长的。
           ;**栈是一端内存，内存地址是由低地址到高地址，而栈使用是高地址到低 每使用一部分SP就网上弹地址就会减少 即初始地址-SP弹到的地址=实际使用的栈大小**/  
           ;                                                                 出栈：当栈释放的话 SP指针就向下弹一部分     		   
; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>


;*********** 配置堆 ************/
;*堆：主要用于动态内存的分配  如malloc()函数,给指针分配一段动态内存 */
Heap_Size       EQU     0x00000200   ;EQU=define  给Heap_Size 定义成0x00000200=512byte 

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3 ;告知汇编器 汇编一个数据段 名字叫HEAP 不初始化(NOINIT) 可读可写(READWRITE) 按8位地址对齐(ALIGN=3)
__heap_base                            ;__heap_base堆的起始地址
Heap_Mem        SPACE   Heap_Size ;SPACE分配空间 大小为Heap_Size=0x00000200=512byte  Heap_Mem即堆的大小
__heap_limit                           ;__heap_limit堆的结束地址

                PRESERVE8   ;当前文件的栈按8字节对齐
                THUMB       ;表示后面指令兼容THUMB指令  (HUMB指令是arm以前的指令集THUBM是ARM以前的指令集16bit，现在Cortex-M系列的都使用THUMB-2 指令集16和31bit的)


;/***********初始化中断向量表***************************/
;中断向量表就是一个数组，每个数组的元素用于存放地址，这些地址指向中断函数。
;Flash实际的是从0x00000000开始的。 32的程序从0x08000000开始存储。即把0x080映射到flash的0x00上。即把flash的0x000地址叫做0x80  所以中断表实在FLsh的0x0开始，而在整个32系统的0x08开始储存
;向量表从Flah的0x0开始储存
;中断向量定义的顺序，根据不同系列芯片查询手册的9.1.2中断向量表
; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY ;开辟区域(AREA) 区域名(RESET DATA)  属性可读(READONLY)
                EXPORT  __Vectors   
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size  ;EXPORT相当于C的extern 说明全局都可以用这三个变量

;向量表起始地址 __Vectors
__Vectors       DCD     __initial_sp               ; Top of Stack  ;DCD分配一个或者多个以字为单位的内存，以四字节对齐，并要求初始化这些内存。在向量表中，DCD 分配了一堆内存，并且以ESR 的入口地址初始化它们。
                DCD     Reset_Handler              ; Reset Handler
                DCD     NMI_Handler                ; NMI Handler
                DCD     HardFault_Handler          ; Hard Fault Handler
                DCD     MemManage_Handler          ; MPU Fault Handler
                DCD     BusFault_Handler           ; Bus Fault Handler
                DCD     UsageFault_Handler         ; Usage Fault Handler
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     SVC_Handler                ; SVCall Handler
                DCD     DebugMon_Handler           ; Debug Monitor Handler
                DCD     0                          ; Reserved
                DCD     PendSV_Handler             ; PendSV Handler
                DCD     SysTick_Handler            ; SysTick Handler

                ; External Interrupts
                DCD     WWDG_IRQHandler            ; Window Watchdog
                DCD     PVD_IRQHandler             ; PVD through EXTI Line detect
                DCD     TAMPER_IRQHandler          ; Tamper
                DCD     RTC_IRQHandler             ; RTC
                DCD     FLASH_IRQHandler           ; Flash
                DCD     RCC_IRQHandler             ; RCC
                DCD     EXTI0_IRQHandler           ; EXTI Line 0
                DCD     EXTI1_IRQHandler           ; EXTI Line 1
                DCD     EXTI2_IRQHandler           ; EXTI Line 2
                DCD     EXTI3_IRQHandler           ; EXTI Line 3
                DCD     EXTI4_IRQHandler           ; EXTI Line 4
                DCD     DMA1_Channel1_IRQHandler   ; DMA1 Channel 1
                DCD     DMA1_Channel2_IRQHandler   ; DMA1 Channel 2
                DCD     DMA1_Channel3_IRQHandler   ; DMA1 Channel 3
                DCD     DMA1_Channel4_IRQHandler   ; DMA1 Channel 4
                DCD     DMA1_Channel5_IRQHandler   ; DMA1 Channel 5
                DCD     DMA1_Channel6_IRQHandler   ; DMA1 Channel 6
                DCD     DMA1_Channel7_IRQHandler   ; DMA1 Channel 7
                DCD     ADC1_2_IRQHandler          ; ADC1 & ADC2
                DCD     USB_HP_CAN1_TX_IRQHandler  ; USB High Priority or CAN1 TX
                DCD     USB_LP_CAN1_RX0_IRQHandler ; USB Low  Priority or CAN1 RX0
                DCD     CAN1_RX1_IRQHandler        ; CAN1 RX1
                DCD     CAN1_SCE_IRQHandler        ; CAN1 SCE
                DCD     EXTI9_5_IRQHandler         ; EXTI Line 9..5
                DCD     TIM1_BRK_IRQHandler        ; TIM1 Break
                DCD     TIM1_UP_IRQHandler         ; TIM1 Update
                DCD     TIM1_TRG_COM_IRQHandler    ; TIM1 Trigger and Commutation
                DCD     TIM1_CC_IRQHandler         ; TIM1 Capture Compare
                DCD     TIM2_IRQHandler            ; TIM2
                DCD     TIM3_IRQHandler            ; TIM3
                DCD     TIM4_IRQHandler            ; TIM4
                DCD     I2C1_EV_IRQHandler         ; I2C1 Event
                DCD     I2C1_ER_IRQHandler         ; I2C1 Error
                DCD     I2C2_EV_IRQHandler         ; I2C2 Event
                DCD     I2C2_ER_IRQHandler         ; I2C2 Error
                DCD     SPI1_IRQHandler            ; SPI1
                DCD     SPI2_IRQHandler            ; SPI2
                DCD     USART1_IRQHandler          ; USART1
                DCD     USART2_IRQHandler          ; USART2
                DCD     USART3_IRQHandler          ; USART3
                DCD     EXTI15_10_IRQHandler       ; EXTI Line 15..10
                DCD     RTCAlarm_IRQHandler        ; RTC Alarm through EXTI Line
                DCD     USBWakeUp_IRQHandler       ; USB Wakeup from suspend
                DCD     TIM8_BRK_IRQHandler        ; TIM8 Break
                DCD     TIM8_UP_IRQHandler         ; TIM8 Update
                DCD     TIM8_TRG_COM_IRQHandler    ; TIM8 Trigger and Commutation
                DCD     TIM8_CC_IRQHandler         ; TIM8 Capture Compare
                DCD     ADC3_IRQHandler            ; ADC3
                DCD     FSMC_IRQHandler            ; FSMC
                DCD     SDIO_IRQHandler            ; SDIO
                DCD     TIM5_IRQHandler            ; TIM5
                DCD     SPI3_IRQHandler            ; SPI3
                DCD     UART4_IRQHandler           ; UART4
                DCD     UART5_IRQHandler           ; UART5
                DCD     TIM6_IRQHandler            ; TIM6
                DCD     TIM7_IRQHandler            ; TIM7
                DCD     DMA2_Channel1_IRQHandler   ; DMA2 Channel1
                DCD     DMA2_Channel2_IRQHandler   ; DMA2 Channel2
                DCD     DMA2_Channel3_IRQHandler   ; DMA2 Channel3
                DCD     DMA2_Channel4_5_IRQHandler ; DMA2 Channel4 & Channel5
__Vectors_End ;向量表结束地址 __Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors ;算出向量表地址=向量表结束地址-向量表起始地址 
                                               ;并EQU定义成__Vectors_Size
 
 
;/**********复位程序**********/   
;/**********复位子程序是系统上电后第一个执行的程序，这程序就俩作用 ①配置系统时钟 ②跳到main()函数开始执行 **********/  
        		AREA    |.text|, CODE, READONLY ;汇编一个代码段(AERA)CODE,取名叫做|.text| 只能读
                
; Reset handler
Reset_Handler   PROC  ;定义子程序，与ENDP 成对使用，表示子程序结束
                EXPORT  Reset_Handler             [WEAK] ;EXPORT=extern Reset_Handler这个函数能全局使用  [WEAK]Keil的弱定义，表明如果Reset_Handler被其他文件定义了,就调用其他文件的定义,没定义就调用此处定义
                IMPORT  __main                           ;IMPORT表明这个函数来自外部文件 从外部输入 _mian是c语言的库函数(由keil自带的)
                IMPORT  SystemInit                       ;SystemInit系统时钟函数用来配置系统时钟 由用户自己写的 在system_stm32f10x.c中 用来配置系统时钟     
                LDR     R0, =SystemInit                  ;LDR 地址左移 把SystemInit的地址加载到R0
                BLX     R0                               ;BLX 跳转到R0的地址里去执行这个函数  然后返回一个值
                LDR     R0, =__main                      ;把c库文件的地址赋值给R0  debug这个函数这步运行就是在初始化配置的这些栈堆中断函数表
                BX      R0                               ;BX  跳转到R0的地址里去执行这个函数  执行完不用返回值  先初始化这个字节,然后跳到用户写的main函数里(跳到c语言的世界)
                ENDP  ;PROC与ENDP成对使用                
 
 
 
 

;/**********中断服务函数**********/   
;/********** 如果你在别的函数使能中断且写了中断服务函数就跳到别处 **********/  
;/********** 如果中断服务函数写错了并且还使能了中断 就会跳到此处进行无限循环**********/ 
; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC                                          ;定义子程序，与ENDP 成对使用，表示子程序结束
                EXPORT  NMI_Handler                [WEAK]     ;EXPORT 声明全局函数NMI_Handler   [WEAK]弱定义，当其他函数定义时此处定义失效
                                                                                                ;所以我们可以在别的地方写中断服务函数  在stm32f103x_it.c里写 或 在自己的工程写
   			    B       .                                     ;B跳转指令 .代表无限循环                                           
                ENDP
;如果某一天 你在c文件里写中断服务函数时，中断名写错。如果产生中断，而你的中断服务函数写错了因此无法跳转到你那里，而跳转到此处
;                                                    此处的中断服务函数 B .是无限循环，因此一旦产生中断 你的程序就会卡死 跳转到这里进行无限循环 死机
;                                                    中断服务函数的名称 一定要和 中断向量表里的名称一致，否则程序运行的时候就不会跳到中断服务函数里 并且编译也找不出错误(因为函数名可以随便定义 编译器不会报错)

HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC ;下面这些和上面均一样 是省略的写法

                EXPORT  WWDG_IRQHandler            [WEAK]
                EXPORT  PVD_IRQHandler             [WEAK]
                EXPORT  TAMPER_IRQHandler          [WEAK]
                EXPORT  RTC_IRQHandler             [WEAK]
                EXPORT  FLASH_IRQHandler           [WEAK]
                EXPORT  RCC_IRQHandler             [WEAK]
                EXPORT  EXTI0_IRQHandler           [WEAK]
                EXPORT  EXTI1_IRQHandler           [WEAK]
                EXPORT  EXTI2_IRQHandler           [WEAK]
                EXPORT  EXTI3_IRQHandler           [WEAK]
                EXPORT  EXTI4_IRQHandler           [WEAK]
                EXPORT  DMA1_Channel1_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel2_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel3_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel4_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel5_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel6_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel7_IRQHandler   [WEAK]
                EXPORT  ADC1_2_IRQHandler          [WEAK]
                EXPORT  USB_HP_CAN1_TX_IRQHandler  [WEAK]
                EXPORT  USB_LP_CAN1_RX0_IRQHandler [WEAK]
                EXPORT  CAN1_RX1_IRQHandler        [WEAK]
                EXPORT  CAN1_SCE_IRQHandler        [WEAK]
                EXPORT  EXTI9_5_IRQHandler         [WEAK]
                EXPORT  TIM1_BRK_IRQHandler        [WEAK]
                EXPORT  TIM1_UP_IRQHandler         [WEAK]
                EXPORT  TIM1_TRG_COM_IRQHandler    [WEAK]
                EXPORT  TIM1_CC_IRQHandler         [WEAK]
                EXPORT  TIM2_IRQHandler            [WEAK]
                EXPORT  TIM3_IRQHandler            [WEAK]
                EXPORT  TIM4_IRQHandler            [WEAK]
                EXPORT  I2C1_EV_IRQHandler         [WEAK]
                EXPORT  I2C1_ER_IRQHandler         [WEAK]
                EXPORT  I2C2_EV_IRQHandler         [WEAK]
                EXPORT  I2C2_ER_IRQHandler         [WEAK]
                EXPORT  SPI1_IRQHandler            [WEAK]
                EXPORT  SPI2_IRQHandler            [WEAK]
                EXPORT  USART1_IRQHandler          [WEAK]
                EXPORT  USART2_IRQHandler          [WEAK]
                EXPORT  USART3_IRQHandler          [WEAK]
                EXPORT  EXTI15_10_IRQHandler       [WEAK]
                EXPORT  RTCAlarm_IRQHandler        [WEAK]
                EXPORT  USBWakeUp_IRQHandler       [WEAK]
                EXPORT  TIM8_BRK_IRQHandler        [WEAK]
                EXPORT  TIM8_UP_IRQHandler         [WEAK]
                EXPORT  TIM8_TRG_COM_IRQHandler    [WEAK]
                EXPORT  TIM8_CC_IRQHandler         [WEAK]
                EXPORT  ADC3_IRQHandler            [WEAK]
                EXPORT  FSMC_IRQHandler            [WEAK]
                EXPORT  SDIO_IRQHandler            [WEAK]
                EXPORT  TIM5_IRQHandler            [WEAK]
                EXPORT  SPI3_IRQHandler            [WEAK]
                EXPORT  UART4_IRQHandler           [WEAK]
                EXPORT  UART5_IRQHandler           [WEAK]
                EXPORT  TIM6_IRQHandler            [WEAK]
                EXPORT  TIM7_IRQHandler            [WEAK]
                EXPORT  DMA2_Channel1_IRQHandler   [WEAK]
                EXPORT  DMA2_Channel2_IRQHandler   [WEAK]
                EXPORT  DMA2_Channel3_IRQHandler   [WEAK]
                EXPORT  DMA2_Channel4_5_IRQHandler [WEAK]

WWDG_IRQHandler
PVD_IRQHandler
TAMPER_IRQHandler
RTC_IRQHandler
FLASH_IRQHandler
RCC_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
DMA1_Channel1_IRQHandler
DMA1_Channel2_IRQHandler
DMA1_Channel3_IRQHandler
DMA1_Channel4_IRQHandler
DMA1_Channel5_IRQHandler
DMA1_Channel6_IRQHandler
DMA1_Channel7_IRQHandler
ADC1_2_IRQHandler
USB_HP_CAN1_TX_IRQHandler
USB_LP_CAN1_RX0_IRQHandler
CAN1_RX1_IRQHandler
CAN1_SCE_IRQHandler
EXTI9_5_IRQHandler
TIM1_BRK_IRQHandler
TIM1_UP_IRQHandler
TIM1_TRG_COM_IRQHandler
TIM1_CC_IRQHandler
TIM2_IRQHandler
TIM3_IRQHandler
TIM4_IRQHandler
I2C1_EV_IRQHandler
I2C1_ER_IRQHandler
I2C2_EV_IRQHandler
I2C2_ER_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
USART1_IRQHandler
USART2_IRQHandler
USART3_IRQHandler
EXTI15_10_IRQHandler
RTCAlarm_IRQHandler
USBWakeUp_IRQHandler
TIM8_BRK_IRQHandler
TIM8_UP_IRQHandler
TIM8_TRG_COM_IRQHandler
TIM8_CC_IRQHandler
ADC3_IRQHandler
FSMC_IRQHandler
SDIO_IRQHandler
TIM5_IRQHandler
SPI3_IRQHandler
UART4_IRQHandler
UART5_IRQHandler
TIM6_IRQHandler
TIM7_IRQHandler
DMA2_Channel1_IRQHandler
DMA2_Channel2_IRQHandler
DMA2_Channel3_IRQHandler
DMA2_Channel4_5_IRQHandler
                B       .

                ENDP     ;中断定义子程序结束

                ALIGN    ;ALIGN对齐指令(编译器的指令 不是汇编的) 
                         ;ALIGN后面一班要加一个立即数  如果不加默认4byte对齐
						



;/**********堆栈初始化函数**********/   
;/********** 这个函数是由_main C库来实现的 **********/  	
;/********** **********/
;/********** **********/
;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
              
   			     IF      :DEF:__MICROLIB ;编译前把 魔法棒--Target--Use MicroCLB这个勾上  就相当于定义了__MICROLIB这个宏
 ;if 如果定义了__MICROLIB 就执行下面函数 就把栈和堆地址自动让_mian调用 从而初始化栈和堆              
                 EXPORT  __initial_sp  ;栈的顶地址   声明成全局地址
                 EXPORT  __heap_base   ;堆的起始地址 声明成全局地址
                 EXPORT  __heap_limit  ; 堆的结束地址 声明成全局地址   可供_main函数定义
                
                 ELSE ;否则 就调用__use_two_region_memory 和 __user_initial_stackheap 这两个函数来初始化栈和堆
                
                 IMPORT  __use_two_region_memory   ;由用户自己实现 在core文件里应该
                 EXPORT  __user_initial_stackheap  ;这个函数在下方
                 
__user_initial_stackheap
                 ;R0 R1 R2 R3是CPU的栈寄存器 一共有15个 在《CM3权威指南chapter03寄存器组》
                 LDR     R0, =  Heap_Mem                ;堆的大小加到R0
                 LDR     R1, =(Stack_Mem + Stack_Size)  ;(栈开辟内存大小 + 栈定义的数据大小) 加到R1
                 LDR     R2, = (Heap_Mem +  Heap_Size)  ;(堆开辟内存大小 + 栈定义的数据大小) 加到R2
                 LDR     R3, = Stack_Mem                ;栈的大小加到R3
                 BX      LR                             ;BX  跳转到LR的地址里去执行这个函数

                 ALIGN                                  ;ALIGN：对指令或者数据存放的地址进行对齐，后面会跟一个立即数。缺省立即数表示4字节对齐。

                 ENDIF

                 END

;******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE*****

;总结：
;        启动文件的作用： 1.分配栈的空间
;                         2.分配堆的空间  
;                         3.初始化中断向量表
;                         4.编写复位程序
;                         5.编写中断服务函数程序
;                         6.实现堆和栈的初始化
;                     详情参考：《野火手册15章》

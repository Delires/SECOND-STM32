;******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
;* File Name          : startup_stm32f10x_hd.s
;* Author             : MCD Application Team
;* Version            : V3.5.0
;* Date               : 11-March-2011
;* Description        : STM32F10x High Density Devices vector table for MDK-ARM 
;*                      toolchain. 
;*                      This module performs:
;*                      - Set the initial SP                   1.��ʼ����ջָ��SP
;*                      - Set the initial PC == Reset_Handler  2.��ʼ��PCָ�룬����ÿִ��һ��PCָ��+1   ��ʼ��PCָ��=Reset_Handler 
;*                                                               ����һ��ʼ�ϵ��151Reset handler��ʼִ��
;*                      - Set the vector table entries with the exceptions ISR address 3.��ʼ���ж������� 61�п�ʼ����Щ�ж���
;*                      - Configure the clock system and also configure the external 
;*                        SRAM mounted on STM3210E-EVAL board to be used as data 
;*                        memory (optional, to be enabled by user) 3.����ʱ��system_init() �� ������STM32�ϵ�SRAM��Ϊ���ݴ洢(�����Ҫ)
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).                           4.����c���Կ⺯��_main ����ת��c���Եı�̴�(��ת����ĳ���)
;*                      After Reset the CortexM3 processor is in Thread mode,;CortexM3�߳�ģʽ 
;*                      priority is Privileged, and the Stack is set to Main.��ջ���ó�����ջMSP
;* <<< Use Configuration Wizard in Context Menu >>>   
;*******************************************************************************
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
; AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
; INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
; CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
; INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************
;********************�����ļ��û�������д��***********************************



; Amount of memory (in bytes) allocated for Stack �����ջ�Ĵ�С
; Tailor this value to your application needs     �������Ҫ�Ļ����ⲿ��ֵ
; <h> Stack Configuration ����ջ   �Ѻ�ջ������ջ�ű���(�ֲ�)����������(���㺯��Ҫ������������ʱ����ѵ�ǰ�����ĵ�ַ����ջ�������������������)
;                                               ��
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

;*********** ����ջ ************/
;*ջ���ű���(�ֲ�/ȫ��)����������(���㺯��Ҫ������������ʱ����ѵ�ǰ�����ĵ�ַ����ջ�������������������)*/
;ջ�ű���(�ֲ�/ȫ��)����������(���㺯��Ҫ������������ʱ����ѵ�ǰ�����ĵ�ַ����ջ�������������������)
Stack_Size      EQU     0x00000400   ;EQU=define  ��Stack_Size�����0x00000400      1k   400h=1024d=1k

                AREA    STACK, NOINIT, READWRITE, ALIGN=3 ;AREA���һ���µĴ����(�൱�ڶ�������) STACK���� NOINIT����ʼ�� READWRITE�ɶ���д  ALIGN����Ϊ2^ALIGN=2^3=8�ֽ� ����8�ֽڶ���
Stack_Mem       SPACE   Stack_Size ;�����ڴ��С�ռ�(��λΪ�ֽ�) Stack_Mem��СΪStack_Size(1K)
	                        ;���������� ����1K�Ŀɶ���д�Ŀռ�� STACK�����
__initial_sp ;/��ʾջ�Ľ�����ַ����ջ����ַ��ջ���ɸ���������ġ�
           ;**ջ��һ���ڴ棬�ڴ��ַ���ɵ͵�ַ���ߵ�ַ����ջʹ���Ǹߵ�ַ���� ÿʹ��һ����SP�����ϵ���ַ�ͻ���� ����ʼ��ַ-SP�����ĵ�ַ=ʵ��ʹ�õ�ջ��С**/  
           ;                                                                 ��ջ����ջ�ͷŵĻ� SPָ������µ�һ����     		   
; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>


;*********** ���ö� ************/
;*�ѣ���Ҫ���ڶ�̬�ڴ�ķ���  ��malloc()����,��ָ�����һ�ζ�̬�ڴ� */
Heap_Size       EQU     0x00000200   ;EQU=define  ��Heap_Size �����0x00000200=512byte 

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3 ;��֪����� ���һ�����ݶ� ���ֽ�HEAP ����ʼ��(NOINIT) �ɶ���д(READWRITE) ��8λ��ַ����(ALIGN=3)
__heap_base                            ;__heap_base�ѵ���ʼ��ַ
Heap_Mem        SPACE   Heap_Size ;SPACE����ռ� ��СΪHeap_Size=0x00000200=512byte  Heap_Mem���ѵĴ�С
__heap_limit                           ;__heap_limit�ѵĽ�����ַ

                PRESERVE8   ;��ǰ�ļ���ջ��8�ֽڶ���
                THUMB       ;��ʾ����ָ�����THUMBָ��  (HUMBָ����arm��ǰ��ָ�THUBM��ARM��ǰ��ָ�16bit������Cortex-Mϵ�еĶ�ʹ��THUMB-2 ָ�16��31bit��)


;/***********��ʼ���ж�������***************************/
;�ж����������һ�����飬ÿ�������Ԫ�����ڴ�ŵ�ַ����Щ��ַָ���жϺ�����
;Flashʵ�ʵ��Ǵ�0x00000000��ʼ�ġ� 32�ĳ����0x08000000��ʼ�洢������0x080ӳ�䵽flash��0x00�ϡ�����flash��0x000��ַ����0x80  �����жϱ�ʵ��FLsh��0x0��ʼ����������32ϵͳ��0x08��ʼ����
;�������Flah��0x0��ʼ����
;�ж����������˳�򣬸��ݲ�ͬϵ��оƬ��ѯ�ֲ��9.1.2�ж�������
; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY ;��������(AREA) ������(RESET DATA)  ���Կɶ�(READONLY)
                EXPORT  __Vectors   
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size  ;EXPORT�൱��C��extern ˵��ȫ�ֶ�����������������

;��������ʼ��ַ __Vectors
__Vectors       DCD     __initial_sp               ; Top of Stack  ;DCD����һ�����߶������Ϊ��λ���ڴ棬�����ֽڶ��룬��Ҫ���ʼ����Щ�ڴ档���������У�DCD ������һ���ڴ棬������ESR ����ڵ�ַ��ʼ�����ǡ�
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
__Vectors_End ;�����������ַ __Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors ;����������ַ=�����������ַ-��������ʼ��ַ 
                                               ;��EQU�����__Vectors_Size
 
 
;/**********��λ����**********/   
;/**********��λ�ӳ�����ϵͳ�ϵ���һ��ִ�еĳ��������������� ������ϵͳʱ�� ������main()������ʼִ�� **********/  
        		AREA    |.text|, CODE, READONLY ;���һ�������(AERA)CODE,ȡ������|.text| ֻ�ܶ�
                
; Reset handler
Reset_Handler   PROC  ;�����ӳ�����ENDP �ɶ�ʹ�ã���ʾ�ӳ������
                EXPORT  Reset_Handler             [WEAK] ;EXPORT=extern Reset_Handler���������ȫ��ʹ��  [WEAK]Keil�������壬�������Reset_Handler�������ļ�������,�͵��������ļ��Ķ���,û����͵��ô˴�����
                IMPORT  __main                           ;IMPORT����������������ⲿ�ļ� ���ⲿ���� _mian��c���ԵĿ⺯��(��keil�Դ���)
                IMPORT  SystemInit                       ;SystemInitϵͳʱ�Ӻ�����������ϵͳʱ�� ���û��Լ�д�� ��system_stm32f10x.c�� ��������ϵͳʱ��     
                LDR     R0, =SystemInit                  ;LDR ��ַ���� ��SystemInit�ĵ�ַ���ص�R0
                BLX     R0                               ;BLX ��ת��R0�ĵ�ַ��ȥִ���������  Ȼ�󷵻�һ��ֵ
                LDR     R0, =__main                      ;��c���ļ��ĵ�ַ��ֵ��R0  debug��������ⲽ���о����ڳ�ʼ�����õ���Щջ���жϺ�����
                BX      R0                               ;BX  ��ת��R0�ĵ�ַ��ȥִ���������  ִ���겻�÷���ֵ  �ȳ�ʼ������ֽ�,Ȼ�������û�д��main������(����c���Ե�����)
                ENDP  ;PROC��ENDP�ɶ�ʹ��                
 
 
 
 

;/**********�жϷ�����**********/   
;/********** ������ڱ�ĺ���ʹ���ж���д���жϷ������������� **********/  
;/********** ����жϷ�����д���˲��һ�ʹ�����ж� �ͻ������˴���������ѭ��**********/ 
; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC                                          ;�����ӳ�����ENDP �ɶ�ʹ�ã���ʾ�ӳ������
                EXPORT  NMI_Handler                [WEAK]     ;EXPORT ����ȫ�ֺ���NMI_Handler   [WEAK]�����壬��������������ʱ�˴�����ʧЧ
                                                                                                ;�������ǿ����ڱ�ĵط�д�жϷ�����  ��stm32f103x_it.c��д �� ���Լ��Ĺ���д
   			    B       .                                     ;B��תָ�� .��������ѭ��                                           
                ENDP
;���ĳһ�� ����c�ļ���д�жϷ�����ʱ���ж���д����������жϣ�������жϷ�����д��������޷���ת�����������ת���˴�
;                                                    �˴����жϷ����� B .������ѭ�������һ�������ж� ��ĳ���ͻῨ�� ��ת�������������ѭ�� ����
;                                                    �жϷ����������� һ��Ҫ�� �ж��������������һ�£�����������е�ʱ��Ͳ��������жϷ������� ���ұ���Ҳ�Ҳ�������(��Ϊ������������㶨�� ���������ᱨ��)

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

Default_Handler PROC ;������Щ�������һ�� ��ʡ�Ե�д��

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

                ENDP     ;�ж϶����ӳ������

                ALIGN    ;ALIGN����ָ��(��������ָ�� ���ǻ���) 
                         ;ALIGN����һ��Ҫ��һ��������  �������Ĭ��4byte����
						



;/**********��ջ��ʼ������**********/   
;/********** �����������_main C����ʵ�ֵ� **********/  	
;/********** **********/
;/********** **********/
;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
              
   			     IF      :DEF:__MICROLIB ;����ǰ�� ħ����--Target--Use MicroCLB�������  ���൱�ڶ�����__MICROLIB�����
 ;if ���������__MICROLIB ��ִ�����溯�� �Ͱ�ջ�Ͷѵ�ַ�Զ���_mian���� �Ӷ���ʼ��ջ�Ͷ�              
                 EXPORT  __initial_sp  ;ջ�Ķ���ַ   ������ȫ�ֵ�ַ
                 EXPORT  __heap_base   ;�ѵ���ʼ��ַ ������ȫ�ֵ�ַ
                 EXPORT  __heap_limit  ; �ѵĽ�����ַ ������ȫ�ֵ�ַ   �ɹ�_main��������
                
                 ELSE ;���� �͵���__use_two_region_memory �� __user_initial_stackheap ��������������ʼ��ջ�Ͷ�
                
                 IMPORT  __use_two_region_memory   ;���û��Լ�ʵ�� ��core�ļ���Ӧ��
                 EXPORT  __user_initial_stackheap  ;����������·�
                 
__user_initial_stackheap
                 ;R0 R1 R2 R3��CPU��ջ�Ĵ��� һ����15�� �ڡ�CM3Ȩ��ָ��chapter03�Ĵ����顷
                 LDR     R0, =  Heap_Mem                ;�ѵĴ�С�ӵ�R0
                 LDR     R1, =(Stack_Mem + Stack_Size)  ;(ջ�����ڴ��С + ջ��������ݴ�С) �ӵ�R1
                 LDR     R2, = (Heap_Mem +  Heap_Size)  ;(�ѿ����ڴ��С + ջ��������ݴ�С) �ӵ�R2
                 LDR     R3, = Stack_Mem                ;ջ�Ĵ�С�ӵ�R3
                 BX      LR                             ;BX  ��ת��LR�ĵ�ַ��ȥִ���������

                 ALIGN                                  ;ALIGN����ָ��������ݴ�ŵĵ�ַ���ж��룬������һ����������ȱʡ��������ʾ4�ֽڶ��롣

                 ENDIF

                 END

;******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE*****

;�ܽ᣺
;        �����ļ������ã� 1.����ջ�Ŀռ�
;                         2.����ѵĿռ�  
;                         3.��ʼ���ж�������
;                         4.��д��λ����
;                         5.��д�жϷ���������
;                         6.ʵ�ֶѺ�ջ�ĳ�ʼ��
;                     ����ο�����Ұ���ֲ�15�¡�

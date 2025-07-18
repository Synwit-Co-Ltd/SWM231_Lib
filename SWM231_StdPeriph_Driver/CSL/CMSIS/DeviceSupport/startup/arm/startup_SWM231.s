;******************************************************************************************************************************************
; 文件名称:	startup_SWM231.s
; 功能说明:	SWM231单片机的启动文件
; 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
; 注意事项:
; 版本日期: V1.0.0		2016年1月30日
; 升级记录:
;
;
;******************************************************************************************************************************************
; @attention
;
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
; REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
; FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
; OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
; -ECTION WITH THEIR PRODUCTS.
;
; COPYRIGHT 2012 Synwit Technology
;******************************************************************************************************************************************


; Amount of memory (in bytes) allocated for Stack
Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; Amount of memory (in bytes) allocated for Heap
Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, CODE, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp               ; Top of Stack
                DCD    Reset_Handler              ; Reset Handler
                DCD    NMI_Handler                ; NMI Handler
                DCD    HardFault_Handler          ; Hard Fault Handler
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    0
                DCD    SVC_Handler                ; SVCall Handler
                DCD    0                          ; Reserved
                DCD    0                          ; Reserved
                DCD    PendSV_Handler             ; PendSV Handler
                DCD    SysTick_Handler            ; SysTick Handler

                ; External Interrupts
				DCD    UART0_Handler
				DCD    UART1_Handler
				DCD    TIMR0_Handler
				DCD    PWM0_Handler
				DCD    PWM1_Handler
				DCD    PWMBRK_Handler
				DCD    DMA_Handler
				DCD    SPI0_Handler
				DCD    WDT_Handler
				DCD    PVD_Handler
				DCD    HALL_Handler
				DCD    ADC_Handler
				DCD    ACMP_Handler
				DCD    BTIMR0_Handler
				DCD    BTIMR1_Handler
				DCD    BTIMR2_Handler
				DCD    DIV_Handler
				DCD    XTALSTOP_Handler
				DCD    FOC_Handler
				DCD    GPIOA_Handler
				DCD    GPIOB_Handler
				DCD    GPIOA0_GPIOB0_Handler
				DCD    GPIOA1_GPIOB1_Handler
				DCD    GPIOA2_GPIOB2_Handler
				DCD    GPIOA3_GPIOB3_Handler
				DCD    GPIOA4_GPIOB4_Handler
				DCD    GPIOA5_GPIOB5_Handler
				DCD    GPIOA6_GPIOB6_Handler
				DCD    GPIOA7_GPIOB7_Handler
				DCD    GPIOA8_GPIOB8_Handler
				DCD    GPIOA9_GPIOB9_Handler
				DCD    GPIOA10_GPIOB10_Handler

__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors



                ;AREA    |.text|, CODE, READONLY

Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
				 IMPORT  __main
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP

HardFault_Handler PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
				
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
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

UART0_Handler  PROC
				EXPORT  UART0_Handler            [WEAK]
				B       .
				ENDP

UART1_Handler  PROC
				EXPORT  UART1_Handler            [WEAK]
				B       .
				ENDP

TIMR0_Handler  PROC
				EXPORT  TIMR0_Handler            [WEAK]
				B       .
				ENDP

PWM0_Handler  PROC
				EXPORT  PWM0_Handler             [WEAK]
				B       .
				ENDP

PWM1_Handler  PROC
				EXPORT  PWM1_Handler             [WEAK]
				B       .
				ENDP

PWMBRK_Handler  PROC
				EXPORT  PWMBRK_Handler           [WEAK]
				B       .
				ENDP

DMA_Handler  PROC
				EXPORT  DMA_Handler              [WEAK]
				B       .
				ENDP

SPI0_Handler  PROC
				EXPORT  SPI0_Handler             [WEAK]
				B       .
				ENDP

WDT_Handler  PROC
				EXPORT  WDT_Handler              [WEAK]
				B       .
				ENDP

PVD_Handler  PROC
				EXPORT  PVD_Handler              [WEAK]
				B       .
				ENDP

HALL_Handler  PROC
				EXPORT  HALL_Handler             [WEAK]
				B       .
				ENDP

ADC_Handler  PROC
				EXPORT  ADC_Handler              [WEAK]
				B       .
				ENDP

ACMP_Handler  PROC
				EXPORT  ACMP_Handler             [WEAK]
				B       .
				ENDP

BTIMR0_Handler  PROC
				EXPORT  BTIMR0_Handler           [WEAK]
				B       .
				ENDP

BTIMR1_Handler  PROC
				EXPORT  BTIMR1_Handler           [WEAK]
				B       .
				ENDP

BTIMR2_Handler  PROC
				EXPORT  BTIMR2_Handler           [WEAK]
				B       .
				ENDP

DIV_Handler  PROC
				EXPORT  DIV_Handler              [WEAK]
				B       .
				ENDP

XTALSTOP_Handler  PROC
				EXPORT  XTALSTOP_Handler         [WEAK]
				B       .
				ENDP

FOC_Handler  PROC
				EXPORT  FOC_Handler              [WEAK]
				B       .
				ENDP

GPIOA_Handler  PROC
				EXPORT  GPIOA_Handler            [WEAK]
				B       .
				ENDP

GPIOB_Handler  PROC
				EXPORT  GPIOB_Handler            [WEAK]
				B       .
				ENDP

GPIOA0_GPIOB0_Handler  PROC
				EXPORT  GPIOA0_GPIOB0_Handler    [WEAK]
				B       .
				ENDP

GPIOA1_GPIOB1_Handler  PROC
				EXPORT  GPIOA1_GPIOB1_Handler    [WEAK]
				B       .
				ENDP

GPIOA2_GPIOB2_Handler  PROC
				EXPORT  GPIOA2_GPIOB2_Handler    [WEAK]
				B       .
				ENDP

GPIOA3_GPIOB3_Handler  PROC
				EXPORT  GPIOA3_GPIOB3_Handler    [WEAK]
				B       .
				ENDP

GPIOA4_GPIOB4_Handler  PROC
				EXPORT  GPIOA4_GPIOB4_Handler    [WEAK]
				B       .
				ENDP

GPIOA5_GPIOB5_Handler  PROC
				EXPORT  GPIOA5_GPIOB5_Handler    [WEAK]
				B       .
				ENDP

GPIOA6_GPIOB6_Handler  PROC
				EXPORT  GPIOA6_GPIOB6_Handler    [WEAK]
				B       .
				ENDP

GPIOA7_GPIOB7_Handler  PROC
				EXPORT  GPIOA7_GPIOB7_Handler    [WEAK]
				B       .
				ENDP

GPIOA8_GPIOB8_Handler  PROC
				EXPORT  GPIOA8_GPIOB8_Handler    [WEAK]
				B       .
				ENDP

GPIOA9_GPIOB9_Handler  PROC
				EXPORT  GPIOA9_GPIOB9_Handler    [WEAK]
				B       .
				ENDP

GPIOA10_GPIOB10_Handler  PROC
				EXPORT  GPIOA10_GPIOB10_Handler  [WEAK]
				B       .
				ENDP

                ALIGN


;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF
    
                END

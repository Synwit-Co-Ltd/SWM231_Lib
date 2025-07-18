;******************************************************************************************************************************************
; 文件名称:    startup_SWM231.s
; 功能说明:    SWM231单片机的启动文件
; 技术支持:    http://www.synwit.com.cn/e/tool/gbook/?bid=1
; 注意事项:
; 版本日期: V1.0.0        2016年1月30日
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

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler              ; Reset Handler
        DCD     NMI_Handler                ; NMI Handler
        DCD     HardFault_Handler          ; Hard Fault Handler
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     SVC_Handler                ; SVCall Handler
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     PendSV_Handler             ; PendSV Handler
        DCD     SysTick_Handler            ; SysTick Handler

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
        

        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =__iar_program_start
        BX      R0
        
        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler


        PUBWEAK UART0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_Handler
        B UART0_Handler

        PUBWEAK UART1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART1_Handler
        B UART1_Handler

        PUBWEAK TIMR0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR0_Handler
        B TIMR0_Handler

        PUBWEAK PWM0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM0_Handler
        B PWM0_Handler

        PUBWEAK PWM1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM1_Handler
        B PWM1_Handler

        PUBWEAK PWMBRK_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWMBRK_Handler
        B PWMBRK_Handler

        PUBWEAK DMA_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA_Handler
        B DMA_Handler

        PUBWEAK SPI0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI0_Handler
        B SPI0_Handler

        PUBWEAK WDT_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
WDT_Handler
        B WDT_Handler

        PUBWEAK PVD_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PVD_Handler
        B PVD_Handler

        PUBWEAK HALL_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HALL_Handler
        B HALL_Handler

        PUBWEAK ADC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC_Handler
        B ADC_Handler

        PUBWEAK ACMP_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
ACMP_Handler
        B ACMP_Handler

        PUBWEAK BTIMR0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BTIMR0_Handler
        B BTIMR0_Handler

        PUBWEAK BTIMR1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BTIMR1_Handler
        B BTIMR1_Handler

        PUBWEAK BTIMR2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BTIMR2_Handler
        B BTIMR2_Handler

        PUBWEAK DIV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DIV_Handler
        B DIV_Handler

        PUBWEAK XTALSTOP_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
XTALSTOP_Handler
        B XTALSTOP_Handler

        PUBWEAK FOC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
FOC_Handler
        B FOC_Handler

        PUBWEAK GPIOA_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA_Handler
        B GPIOA_Handler

        PUBWEAK GPIOB_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOB_Handler
        B GPIOB_Handler

        PUBWEAK GPIOA0_GPIOB0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA0_GPIOB0_Handler
        B GPIOA0_GPIOB0_Handler

        PUBWEAK GPIOA1_GPIOB1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA1_GPIOB1_Handler
        B GPIOA1_GPIOB1_Handler

        PUBWEAK GPIOA2_GPIOB2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA2_GPIOB2_Handler
        B GPIOA2_GPIOB2_Handler

        PUBWEAK GPIOA3_GPIOB3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA3_GPIOB3_Handler
        B GPIOA3_GPIOB3_Handler

        PUBWEAK GPIOA4_GPIOB4_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA4_GPIOB4_Handler
        B GPIOA4_GPIOB4_Handler

        PUBWEAK GPIOA5_GPIOB5_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA5_GPIOB5_Handler
        B GPIOA5_GPIOB5_Handler

        PUBWEAK GPIOA6_GPIOB6_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA6_GPIOB6_Handler
        B GPIOA6_GPIOB6_Handler

        PUBWEAK GPIOA7_GPIOB7_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA7_GPIOB7_Handler
        B GPIOA7_GPIOB7_Handler

        PUBWEAK GPIOA8_GPIOB8_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA8_GPIOB8_Handler
        B GPIOA8_GPIOB8_Handler

        PUBWEAK GPIOA9_GPIOB9_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA9_GPIOB9_Handler
        B GPIOA9_GPIOB9_Handler

        PUBWEAK GPIOA10_GPIOB10_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA10_GPIOB10_Handler
        B GPIOA10_GPIOB10_Handler

        END

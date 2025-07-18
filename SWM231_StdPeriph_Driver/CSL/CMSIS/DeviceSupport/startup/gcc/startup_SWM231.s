    .syntax unified
    .arch armv6-m

/* Memory Model
   The HEAP starts at the end of the DATA section and grows upward.
   
   The STACK starts at the end of the RAM and grows downward     */
    .section .stack
    .align 3
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    0x800
__StackTop:


    .section .heap
    .align 3
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .space    0x000
__HeapLimit:


    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop            
    .long    Reset_Handler         
    .long    NMI_Handler          
    .long    HardFault_Handler     
    .long    0     
    .long    0      
    .long    0   
    .long    0                    
    .long    0                    
    .long    0                    
    .long    0
    .long    SVC_Handler          
    .long    0     
    .long    0                     
    .long    PendSV_Handler           
    .long    SysTick_Handler         

    /* External interrupts */
    .long    UART0_Handler
    .long    UART1_Handler
    .long    TIMR0_Handler
    .long    PWM0_Handler
    .long    PWM1_Handler
    .long    PWMBRK_Handler
    .long    DMA_Handler
    .long    SPI0_Handler
    .long    WDT_Handler
    .long    PVD_Handler
    .long    HALL_Handler
    .long    ADC_Handler
    .long    ACMP_Handler
    .long    BTIMR0_Handler
    .long    BTIMR1_Handler
    .long    BTIMR2_Handler
    .long    DIV_Handler
    .long    XTALSTOP_Handler
    .long    FOC_Handler
    .long    GPIOA_Handler
    .long    GPIOB_Handler
    .long    GPIOA0_GPIOB0_Handler
    .long    GPIOA1_GPIOB1_Handler
    .long    GPIOA2_GPIOB2_Handler
    .long    GPIOA3_GPIOB3_Handler
    .long    GPIOA4_GPIOB4_Handler
    .long    GPIOA5_GPIOB5_Handler
    .long    GPIOA6_GPIOB6_Handler
    .long    GPIOA7_GPIOB7_Handler
    .long    GPIOA8_GPIOB8_Handler
    .long    GPIOA9_GPIOB9_Handler
    .long    GPIOA10_GPIOB10_Handler

	.section .text.Reset_Handler
    .align 2
    .thumb
    .globl    Reset_Handler
    .type     Reset_Handler, %function
Reset_Handler:
/* Loop to copy data from read only memory to RAM. The ranges
 * of copy from/to are specified by symbols evaluated in linker script.  */
    ldr    r1, =__data_load__
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs   r3, r2
    ble .Lflash_to_ram_done

.Lflash_to_ram_loop:
    subs r3, #4
    ldr r0, [r1, r3]
    str r0, [r2, r3]
    bgt .Lflash_to_ram_loop
.Lflash_to_ram_done:


    ldr    r2, =__bss_start__
    ldr    r3, =__bss_end__

    subs   r3, r2
    ble .Lbss_to_ram_done

    movs    r0, 0
.Lbss_to_ram_loop:
    subs r3, #4
    str r0, [r2, r3]
    bgt .Lbss_to_ram_loop
.Lbss_to_ram_done:

    ldr    r0, =main
    bx     r0
    .pool    


    .text
/* Macro to define default handlers. 
   Default handler will be weak symbol and just dead loops. */
    .macro    def_default_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    b    .
    .endm

    def_default_handler    NMI_Handler
    def_default_handler    HardFault_Handler
    def_default_handler    SVC_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler

    def_default_handler    UART0_Handler
    def_default_handler    UART1_Handler
    def_default_handler    TIMR0_Handler
    def_default_handler    PWM0_Handler
    def_default_handler    PWM1_Handler
    def_default_handler    PWMBRK_Handler
    def_default_handler    DMA_Handler
    def_default_handler    SPI0_Handler
    def_default_handler    WDT_Handler
    def_default_handler    PVD_Handler
    def_default_handler    HALL_Handler
    def_default_handler    ADC_Handler
    def_default_handler    ACMP_Handler
    def_default_handler    BTIMR0_Handler
    def_default_handler    BTIMR1_Handler
    def_default_handler    BTIMR2_Handler
    def_default_handler    DIV_Handler
    def_default_handler    XTALSTOP_Handler
    def_default_handler    FOC_Handler
    def_default_handler    GPIOA_Handler
    def_default_handler    GPIOB_Handler
    def_default_handler    GPIOA0_GPIOB0_Handler
    def_default_handler    GPIOA1_GPIOB1_Handler
    def_default_handler    GPIOA2_GPIOB2_Handler
    def_default_handler    GPIOA3_GPIOB3_Handler
    def_default_handler    GPIOA4_GPIOB4_Handler
    def_default_handler    GPIOA5_GPIOB5_Handler
    def_default_handler    GPIOA6_GPIOB6_Handler
    def_default_handler    GPIOA7_GPIOB7_Handler
    def_default_handler    GPIOA8_GPIOB8_Handler
    def_default_handler    GPIOA9_GPIOB9_Handler
    def_default_handler    GPIOA10_GPIOB10_Handler
    
    def_default_handler    Default_Handler

    .end

#include "SWM231.h"

/* 注意：WFE可以通过IO 唤醒,  WFI只能通过中断唤醒
 *
 */
 
#define USE_WFI 0

	
void EnterSleepMode(void) ;
void GPIO_Sleep_Config(void);

int main(void)
{
	SW_DelayMS(2000);	//防止调试过程中卡死
	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//接 LED，指示程序执行状态
	GPIO_SetBit(GPIOA, PIN5);				//点亮LED
	
	GPIO_Init(GPIOA, PIN4, 0, 1, 0, 0);		//接按键，上拉使能
	
#if USE_WFI	//WFI 只能通过中断唤醒
	EXTI_Init(GPIOA, PIN4, EXTI_FALL_EDGE);		//下降沿触发中断
	NVIC_EnableIRQ(GPIOA4_GPIOB4_IRQn);
	EXTI_Open(GPIOA, PIN4);
#endif
	
	SYS->PAWKEN |= (1 << PIN4);				//开启PA4引脚低电平唤醒
	
	SYS->RCCR |= SYS_RCCR_LON_Msk;			//休眠模式使用低频时钟 
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);							//点亮LED
		SW_DelayMS(500);
		GPIO_ClrBit(GPIOA, PIN5);							//熄灭LED
		
		Flash_Param_at_xMHz(60);
		switchToHRC();	//休眠前切换到内部RC时钟，进入低功耗模式之后自动切换到内部低频时钟
		
		SYS->XTALCR&= ~((1 << SYS_XTALCR_ON_Pos) | (1 << SYS_XTALCR_DET_Pos)  );//关闭外振,降低功耗
		
		SYS->CLKEN0 &=~ (1 << SYS_CLKEN0_ANAC_Pos); //关闭模拟模块时钟节省功耗--按需关闭
		
		EnterSleepMode();	//在SRAM中进入sleep,等待唤醒
		
		SYS->CLKEN0 |=(1 << SYS_CLKEN0_ANAC_Pos); 
		switchToHRC();
		Flash_Param_at_xMHz(CyclesPerUs);
	}
}

#if USE_WFI	

#define USE_GPIOA4_IRQ	1

#if USE_GPIOA4_IRQ
void GPIOA4_GPIOB4_Handler(void)
#else
void GPIOA_Handler(void)
#endif
{
	if(EXTI_State(GPIOA, PIN4))
	{
		EXTI_Clear(GPIOA, PIN4);
		
		GPIO_InvBit(GPIOA, PIN5);
	}
}

#endif

void GPIO_Sleep_Config(void)
{
	PORT_Init(PORTB, PIN0, PORTB_PIN0_GPIO,0);//SWCLK
	PORT_Init(PORTB, PIN1, PORTB_PIN1_GPIO,0);//SWDIO

	PORT_Init(PORTA, PIN9, PORTA_PIN9_GPIO,0);    //TMS
	PORT_Init(PORTB, PIN3, PORTB_PIN3_GPIO, 0);   //TDI
	PORT_Init(PORTB, PIN6, PORTB_PIN6_GPIO, 0);   //TCLK

	PORT_Init	(PORTB, PIN10, PORTB_PIN10_GPIO, 0); //ISP
}

void EnterSleepMode(void) 
{
	int i;
	
	while(1)
	{
		#if USE_WFI	
		__WFI();
		#else
		__WFE();
		#endif

		if ((SYS->PAWKSR & (1 << PIN4)) != 0) 
		{
			break;
		}
		
		__NOP();
	}
	
	SYS->PAWKSR |= (1 << PIN4);							//清除唤醒状态
	
}  


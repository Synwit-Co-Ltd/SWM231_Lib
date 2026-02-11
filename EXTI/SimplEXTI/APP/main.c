#include "SWM231.h"


#define USE_GPIOA4_IRQ	1


int main(void)
{
	SystemInit();
	
	GPIO_INIT(GPIOA, PIN5, GPIO_OUTPUT);	//输出，接LED
	
	GPIO_INIT(GPIOA, PIN4, GPIO_INPUT_PullUp);	//输入，上拉使能，接KEY
	
	EXTI_Init(GPIOA, PIN4, EXTI_FALL_EDGE);		//下降沿触发中断
	
#if USE_GPIOA4_IRQ
	NVIC_EnableIRQ(GPIOA4_GPIOB4_IRQn);
#else
	NVIC_EnableIRQ(GPIOA_IRQn);
#endif
	
	EXTI_Open(GPIOA, PIN4);
	
	while(1==1)
	{
	}
}


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

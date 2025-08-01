#include "SWM231.h"


int main(void)
{	
	uint32_t i;
	SystemInit();
	
	GPIO_Init(GPIOA, PIN4, 1, 0, 0, 0);					//模拟外部被计数信号
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);					//GPIOA.5 => LED
	
	PORT_Init(PORTA, PIN0, PORTA_PIN0_TIMR0_IN, 1);		//连接外部被计数信号，PA4 => PA0
	PORTA->PULLU |= (1 << PIN0);
	
	TIMR_Init(TIMR0, TIMR_MODE_COUNTER, 1, 3, 1);		//每计3个上升沿进入中断
	TIMR_Start(TIMR0);
	
	while(1==1)
	{
		GPIO_InvBit(GPIOA, PIN4);
		for(i = 0; i < CyclesPerUs*1000; i++) __NOP();
	}
}

void TIMR0_Handler(void)
{
	TIMR_INTClr(TIMR0);
	
	GPIO_InvBit(GPIOA, PIN5);
}

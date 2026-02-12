#include "SWM231.h"


int main(void)
{
	SystemInit();
	
	GPIO_INIT(GPIOA, PIN5, GPIO_OUTPUT);		//输出，接LED
	
	SysTick_Config(SystemCoreClock/4);			//每0.25秒钟触发一次中断
	
	while(1==1)
	{
	}
}

void SysTick_Handler(void)
{
	GPIO_InvBit(GPIOA, PIN5);	//反转LED亮灭状态
}

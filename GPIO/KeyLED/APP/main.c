#include "SWM231.h"


int main(void)
{	
	SystemInit();
	
	GPIO_INIT(GPIOA, PIN4, GPIO_INPUT_PullUp);	//输入，上拉使能，接KEY
	
	GPIO_INIT(GPIOA, PIN5, GPIO_OUTPUT);		//输出， 接LED
	
	while(1==1)
	{
		if(GPIO_GetBit(GPIOA, PIN4) == 0)		//按键被按下
		{
			GPIO_SetBit(GPIOA, PIN5);
		}
		else
		{
			GPIO_ClrBit(GPIOA, PIN5);
		}
	}
}

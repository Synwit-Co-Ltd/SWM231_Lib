#include "SWM231.h"

void EnterDeepSleepMode(void) 
{
  SCB->SCR|= SCB_SCR_SLEEPDEEP_Msk;//使能DEEPSLEEP
  SYS->PAWKSR |= (1 << PIN4);
  
  while (1) {
    __WFE();
    if ((SYS->PAWKSR & (1 << PIN4)) != 0) {
        break;
    }
    __NOP();
	}
  
	SYS->PAWKSR |= (1 << PIN4);							//清除唤醒状态
	
}  


void GPIO_Sleep_Config(void)
{
	PORT_Init(PORTB, PIN0, PORTB_PIN0_GPIO,0);//SWCLK
	PORT_Init(PORTB, PIN1, PORTB_PIN1_GPIO,0);//SWDIO

	PORT_Init(PORTA, PIN9, PORTA_PIN9_GPIO,0);    //TMS
	PORT_Init(PORTB, PIN3, PORTB_PIN3_GPIO, 0);   //TDI
	PORT_Init(PORTB, PIN6, PORTB_PIN6_GPIO, 0);   //TCLK

	PORT_Init	(PORTB, PIN10, PORTB_PIN10_GPIO, 0); //ISP
}
	
	
int main(void)
{
	SystemInit();

	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//接 LED，指示程序执行状态
	GPIO_SetBit(GPIOA, PIN5);				      //点亮LED

	SW_DelayMS(2000);	//防止调试过程中卡死	
	
  GPIO_ClrBit(GPIOA, PIN5); //熄灭LED

	SW_DelayMS(500);	//防止调试过程中卡死	
	
	GPIO_Init(GPIOA, PIN4, 0, 1, 0, 0);		//唤醒脚接按键，上拉使能

  GPIO_Sleep_Config();		
  
	SYS->PAWKEN |= (1 << PIN4);		//开启PA4引脚低电平唤醒
	
	SYS->RCCR |= SYS_RCCR_LON_Msk;		//休眠模式使用低频时钟 
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);							//点亮LED
		SW_DelayMS(3000);
		GPIO_ClrBit(GPIOA, PIN5);							//熄灭LED
		
		Flash_Param_at_xMHz(60);
		switchToHRC();	//休眠前切换到内部RC时钟，进入低功耗模式之后自动切换到内部低频时钟
		
		SYS->XTALCR&= ~((1 << SYS_XTALCR_ON_Pos) | (1 << SYS_XTALCR_DET_Pos)  );//关闭外振,降低功耗
		
		SYS->CLKEN0 &=~ (1 << SYS_CLKEN0_ANAC_Pos); //关闭模拟模块时钟节省功耗--按需关闭
		
		EnterDeepSleepMode();	
		
		SYS->CLKEN0 |=(1 << SYS_CLKEN0_ANAC_Pos); 
		
		switchToHRC();
		
		Flash_Param_at_xMHz(CyclesPerUs);
	}
}



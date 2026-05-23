/*******************************************************************************************************************************
* @brief	SWM231 System Clock setting
*
*
********************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology 
*******************************************************************************************************************************/
#include <stdint.h>
#include "SWM231.h"


/******************************************************************************************************************************************
 * 系统时钟设定
 *****************************************************************************************************************************************/
#define SYS_CLK_60MHz		3	 	//内部高频60MHz RC振荡器
#define SYS_CLK_XTAL		2		//外部晶体振荡器（4-24MHz）
#define SYS_CLK_32KHz		0		//内部低频32KHz RC振荡器

#define SYS_CLK   	SYS_CLK_60MHz


#define SYS_CLK_DIV	SYS_CLK_DIV_1	//SYS_CLK 选择的时钟，经过 SYS_CLK_DIV 分频后用作系统时钟



#define __HSI		(60000000UL)		//高速内部时钟
#define __LSI		(   32000UL)		//低速内部时钟
#define __HSE		(12000000UL)		//高速外部时钟
#define __LSE		(   32768UL)		//低速外部时钟



uint32_t SystemCoreClock  = __HSI;   				//System Clock Frequency (Core Clock)
uint32_t CyclesPerUs      = (__HSI / 1000000); 		//Cycles per micro second


/****************************************************************************************************************************************** 
* 函数名称: SystemCoreClockUpdate()
* 功能说明: This function is used to update the variable SystemCoreClock and must be called whenever the core clock is changed
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
void SystemCoreClockUpdate(void)    
{
	if(SYS->CLKSEL & SYS_CLKSEL_SYS_Msk)			//SYS  <= HRC
	{
		SystemCoreClock = __HSI;
	}
	else											//SYS  <= CLK
	{
		switch((SYS->CLKSEL & SYS_CLKSEL_CLK_Msk) >> SYS_CLKSEL_CLK_Pos)
		{
		case SYS_CLK_60MHz:
			SystemCoreClock = __HSI;
			break;
		
		case SYS_CLK_XTAL:
			SystemCoreClock = __HSE;
			break;
		
		case SYS_CLK_32KHz:
			SystemCoreClock = __LSI;
			break;
		}
		
		SystemCoreClock /= (1 << ((SYS->CLKSEL & SYS_CLKSEL_CLK_DIVx_Msk) >> SYS_CLKSEL_CLK_DIVx_Pos));
	}
	
	CyclesPerUs = SystemCoreClock / 1000000;
}


/****************************************************************************************************************************************** 
* 函数名称: 
* 功能说明: The necessary initializaiton of systerm
* 输    入: 
* 输    出: 
* 注意事项: 
******************************************************************************************************************************************/
void SystemInit(void)
{
	SYS->CLKEN0 |= (1 << SYS_CLKEN0_ANAC_Pos);
	
	Flash_Param_at_xMHz(60);
	
	switchToHRC();
	
	switch(SYS_CLK)
	{
		case SYS_CLK_60MHz:
			switchOnHRC();
			break;
		
		case SYS_CLK_XTAL:
			switchOnXTAL();
			break;
		
		case SYS_CLK_32KHz:
			switchOn32KHz();
			break;
	}
	
	switchToDIV(SYS_CLK, SYS_CLK_DIV);
	
	Flash_Param_at_xMHz(CyclesPerUs);
	
	PORTA->PULLU = 0;
	PORTB->PULLD = 0;
	PORTB->PULLU = 0;
}

void switchToHRC(void)
{
	SYS->RCCR |= (1 << SYS_RCCR_HON_Pos);
	
	for(int i = 0; i < CyclesPerUs; i++) {}
	
	SYS->CLKSEL |= (1 << SYS_CLKSEL_SYS_Pos);		//SYS <= HRC
	
	SystemCoreClockUpdate();
}

void switchToDIV(uint32_t src, uint32_t div)
{
	SYS->CLKSEL &=~(SYS_CLKSEL_CLK_Msk | SYS_CLKSEL_CLK_DIVx_Msk);
	SYS->CLKSEL |= (src << SYS_CLKSEL_CLK_Pos) |
				   (div << SYS_CLKSEL_CLK_DIVx_Pos);
	
	SYS->CLKDIVx_ON = 1;
	
	for(int i = 0; i < CyclesPerUs; i++) {}
	
	SYS->CLKSEL &=~(1 << SYS_CLKSEL_SYS_Pos);		//SYS <= CLK_DIVx
	
	SystemCoreClockUpdate();
}

void switchOnHRC(void)
{
	SYS->RCCR |= (1 << SYS_RCCR_HON_Pos);
}

void switchOnXTAL(void)
{
	PORTA->PULLU &= ~((1 << PIN8) | (1 << PIN9));
	PORTA->PULLD &= ~((1 << PIN8) | (1 << PIN9));
	
	PORT_Init(PORTA, PIN8, PORTA_PIN8_XTAL_OUT, 0);
	PORT_Init(PORTA, PIN9, PORTA_PIN9_XTAL_IN,  0);
	
	SYS->XTALCR |= (1 << SYS_XTALCR_ON_Pos) | (1 << SYS_XTALCR_DET_Pos);
}

void switchOn32KHz(void)
{
	SYS->RCCR |= (1 << SYS_RCCR_LON_Pos);
}


void SW_DelayUS(uint32_t us)
{
	us = CyclesPerUs * us / 8;
	
	for(int i = 0; i < us; i++) __NOP();
}

void SW_DelayMS(uint32_t ms)
{
	for(int i = 0; i < ms; i++) SW_DelayUS(1000);
}

#include "SWM231.h"

void SerialInit(void);

int main(void)
{
	SystemInit();
	
	SerialInit();
	
	if(SYS->RSTSR & SYS_RSTSR_WDT_Msk)
	{
		SYS->RSTSR = (1 << SYS_RSTSR_WDT_Pos);
		
		printf("WDT Reset Happened\r\n");
	}
	
	WDT_Init(WDT, 0, 1000);
	WDT_Start(WDT);
	
	while(1==1)
	{
		WDT_Feed(WDT);	//注释掉喂狗操作，观察WDT复位现象
		
		/* 注意：
		 *	1、两次喂狗之间至少间隔 5 个 WDT 时钟周期，即 1000000 / 32768 * 5 = 150us；又考虑到 WDT 时钟误差很大，建议间隔不小于 300us
		 *	2、WDT 停止状态下，不要执行 WDT_Feed()
		 *	3、执行 WDT_Feed() 后，不能立即执行 WDT_Stop()，必须间隔 5 个 WDT 时钟周期再执行 WDT_Stop()
		*/
		
		SW_DelayUS(300);
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN2, PORTA_PIN2_UART0_TX, 0);
	PORT_Init(PORTA, PIN3, PORTA_PIN3_UART0_RX, 1);
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}

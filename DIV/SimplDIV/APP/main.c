#include "SWM231.h"

void SerialInit(void);

int main(void)
{
	uint32_t uquo, urem;
	int32_t  squo, srem;
	
	SystemInit();
	
	SerialInit();
   	
	DIV_Init(DIV);
	
	DIV_UDiv(15, 6);
	while(DIV_Div_IsBusy());
	DIV_UDiv_Result(&uquo, &urem);
	printf("15/6 QUO=%d, REM=%d\r\n", uquo, urem);
	
	DIV_SDiv(-15, 6);
	while(DIV_Div_IsBusy());
	DIV_SDiv_Result(&squo, &srem);
	printf("-15/6 QUO=%d, REM=%d\r\n", squo, srem);
   	
	while(1==1)
	{
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

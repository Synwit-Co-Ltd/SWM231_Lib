#include "SWM231.h"

void SerialInit(void);

int main(void)
{
	uint32_t chr;
	uint32_t err;
	
	SystemInit();
	
	SerialInit();
   	
	printf("Hi from SWM231\n");
	
	while(1==1)
	{
		while(UART_IsRXFIFOEmpty(UART0));
		
		err = UART_ReadByte(UART0, &chr);
		if(err == 0)
		{
			UART_WriteByte(UART0, chr);
		}
		else if(err == UART_ERR_PARITY)
		{
			printf("Parity error!\n");
			
			while(1) __NOP();
		}
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

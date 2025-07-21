#include "SWM231.h"

void SerialInit(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTB, PIN7, PORTB_PIN7_OPA0_INN, 0);
	PORT_Init(PORTB, PIN6, PORTB_PIN6_OPA0_INP, 0);
	PORT_Init(PORTB, PIN5, PORTB_PIN5_OPA0_OUT, 0);
	PORT_Init(PORTA, PIN0, PORTA_PIN0_OPA1_INN, 0);
	PORT_Init(PORTB, PIN9, PORTB_PIN9_OPA1_INP, 0);
	PORT_Init(PORTB, PIN8, PORTB_PIN8_OPA1_OUT, 0);
	
	SYS->PGA0CR = (1 << SYS_PGA0CR_EN_Pos) |
				  (1 << SYS_PGA0CR_MODE_Pos) |
				  (2 << SYS_PGA0CR_GAIN_Pos) |		// PGA 增益选择：0 x1   1 x5   2 x10   3 x10
				  (2 << SYS_PGA0CR_ROUT_Pos);		// 输出电阻选择：0 open   1 100   2 1k   3 10k
	
	SYS->PGA1CR = (1 << SYS_PGA1CR_EN_Pos) |
				  (1 << SYS_PGA1CR_MODE_Pos) |
				  (2 << SYS_PGA1CR_GAIN_Pos) |
				  (2 << SYS_PGA1CR_ROUT_Pos);
	
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

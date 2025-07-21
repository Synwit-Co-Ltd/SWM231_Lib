#include "SWM231.h"

void SerialInit(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTB, PIN5, PORTB_PIN5_ACMP0_INP0, 0);
	PORT_Init(PORTB, PIN4, PORTB_PIN4_ACMP0_INP1, 0);
	PORT_Init(PORTB, PIN3, PORTB_PIN3_ACMP0_INP2, 0);
	PORT_Init(PORTB, PIN7, PORTB_PIN7_ACMP0_INN,  0);
	
	SYS->ACMP0CR |= (1 << SYS_ACMP0CR_EN_Pos)  | 
					(1 << SYS_ACMP0CR_HYS_Pos) |
					(2 << SYS_ACMP0CR_VNSEL_Pos) |		//VPX 作为 VN 输入
					(0 << SYS_ACMP0CR_VPSEL_Pos) |
					(1 << SYS_ACMP0CR_VPXEN_Pos);		//VP0/VP1/VP2星形连接,中心点作为VPX

	while(1==1)
	{
		printf("ACMP0->OUT = %d\n", (SYS->ACMPSR & SYS_ACMPSR_CMP0OUT_Msk) ? 1 : 0);
		
		for(int i = 0; i < SystemCoreClock/32; i++) __NOP();
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

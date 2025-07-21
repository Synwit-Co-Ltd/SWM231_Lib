#include "SWM231.h"

void SerialInit(void);
void TestSignal(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	TestSignal();
	
	PORT_Init(PORTB, PIN3, PORTB_PIN3_HALL_IN0, 1);		//连接PA3
	PORT_Init(PORTB, PIN4, PORTB_PIN4_HALL_IN1, 1);		//连接PA4
	PORT_Init(PORTB, PIN5, PORTB_PIN5_HALL_IN2, 1);		//连接PA5
	PORTB->PULLU |= ((1 << PIN3) | (1 << PIN4) | (1 << PIN5));
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, CyclesPerUs, 2000000, 1);		//2秒钟未检测到HALL输入变化，触发超时中断
	
	TIMRG->HALLEN = 1;
	TIMRG->HALLIF = 7;
	TIMRG->HALLIE = 1;
	NVIC_EnableIRQ(HALL_IRQn);
	
	TIMR_Start(TIMR0);
	
	while(1==1)
	{
	}
}


void HALL_Handler(void)
{
	TIMRG->HALLIF = 7;
	
	printf("%dus\r\n", TIMRG->HALLDR);
}


void TIMR0_Handler(void)
{
	TIMR0->IF = (1 << TIMR_IF_TO_Pos);
	
	printf("HALL Time-out\r\n");
}


void TestSignal(void)
{
	GPIO_Init(GPIOA, PIN3,  1, 0, 0, 0);
	GPIO_Init(GPIOA, PIN4, 1, 0, 0, 0);
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);
	
	TIMR_Init(BTIMR1, TIMR_MODE_TIMER, CyclesPerUs, 100000, 1);
	TIMR_Start(BTIMR1);
}


void BTIMR1_Handler(void)
{
	static uint32_t setp = 0;
	
	BTIMR1->IF = TIMR_IF_TO_Msk;
	
	switch(setp++)
	{
	case 0: GPIO_SetBit(GPIOA, PIN3); break;
	case 1: GPIO_SetBit(GPIOA, PIN4); break;
	case 2: GPIO_SetBit(GPIOA, PIN5); break;
	case 3: GPIO_ClrBit(GPIOA, PIN3); break;
	case 4: GPIO_ClrBit(GPIOA, PIN4); break;
	case 5: GPIO_ClrBit(GPIOA, PIN5); setp = 0; break;
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

#include "SWM231.h"

void SerialInit(void);

int main(void)
{	
	uint32_t rxdata, txdata = 0x23;
	SPI_InitStructure SPI_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTA, PIN4, PORTA_PIN4_SPI0_SCLK, 0);
	PORT_Init(PORTA, PIN5, PORTA_PIN5_SPI0_MOSI, 0);
	PORT_Init(PORTA, PIN6, PORTA_PIN6_SPI0_MISO, 1);	//将MOSI与MISO连接，自发、自收、然后打印
	PORT_Init(PORTA, PIN7, PORTA_PIN7_SPI0_SSEL, 0);
	
	SPI_initStruct.clkDiv = SPI_CLKDIV_4;
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_SECOND_EDGE;
	SPI_initStruct.IdleLevel = SPI_LOW_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 1;
	SPI_initStruct.RXThreshold = 0;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThreshold = 0;
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_initStruct.TXCompleteIEn  = 0;
	SPI_Init(SPI0, &SPI_initStruct);
	SPI_Open(SPI0);
	
	while(1==1)
	{
		rxdata = SPI_ReadWrite(SPI0, txdata);
		txdata = rxdata + 1;		//将SPI0_MOSI与SPI0_MISO短接，则可看到打印数值加一递增
		
		printf("rxdata: 0x%X\r\n", rxdata);
		
		for(int i = 0; i < SystemCoreClock/10; i++);
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

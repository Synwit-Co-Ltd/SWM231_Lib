#include "SWM231.h"
#include <string.h>


#define TEST_DMA_TX	1
#define TEST_DMA_RX	2
#define TEST_DMA	TEST_DMA_TX


char TX_Buffer[] = "Hello From Synwit\r\n";

#define BUF_SIZE  32
char RX_Buffer[BUF_SIZE] = {0};

void SerialInit(void);

int main(void)
{
	SPI_InitStructure SPI_initStruct;
	DMA_InitStructure DMA_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTA, PIN4, PORTA_PIN4_SPI0_SCLK, 0);
	PORT_Init(PORTA, PIN5, PORTA_PIN5_SPI0_MOSI, 0);
	PORT_Init(PORTA, PIN6, PORTA_PIN6_SPI0_MISO, 1);	//将MOSI与MISO连接，自发、自收、然后打印
	PORT_Init(PORTA, PIN7, PORTA_PIN7_SPI0_SSEL, 0);
	
	SPI_initStruct.clkDiv = SPI_CLKDIV_32;
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
	
#if TEST_DMA == TEST_DMA_RX
	// SPI0 RX DMA
	SPI0->CTRL |= SPI_CTRL_DMARXEN_Msk;
	
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_BYTE;
	DMA_initStruct.Count = BUF_SIZE;
	DMA_initStruct.MemoryAddr = (uint32_t)RX_Buffer;
	DMA_initStruct.MemoryAddrInc = 1;
	DMA_initStruct.PeripheralAddr = (uint32_t)&SPI0->DATA;
	DMA_initStruct.PeripheralAddrInc = 0;
	DMA_initStruct.Handshake = DMA_CH0_SPI0RX;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = 0;
	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	DMA_CH_Open(DMA_CH0);
#else
	// SPI0 TX DMA
	SPI0->CTRL |= SPI_CTRL_DMATXEN_Msk;
	
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_BYTE;
	DMA_initStruct.Count = strlen(TX_Buffer);
	DMA_initStruct.MemoryAddr = (uint32_t)TX_Buffer;
	DMA_initStruct.MemoryAddrInc = 1;
	DMA_initStruct.PeripheralAddr = (uint32_t)&SPI0->DATA;
	DMA_initStruct.PeripheralAddrInc = 0;
	DMA_initStruct.Handshake = DMA_CH0_SPI0TX;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = 0;
	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	DMA_CH_Open(DMA_CH0);
#endif

	while(1==1)
	{
		SW_DelayMS(500);
		
#if TEST_DMA == TEST_DMA_TX
		DMA_CH_Open(DMA_CH0);
#else
		for(int i = 0; i < strlen(TX_Buffer); i++)
			SPI_WriteWithWait(SPI0, TX_Buffer[i]);
		
		int rx_count = BUF_SIZE - DMA_CH_GetRemaining(DMA_CH0);
		
		for(int i = 0; i < rx_count; i++) printf("%c", RX_Buffer[i]);
		
		DMA_CH_Close(DMA_CH0);
		DMA_CH_Open(DMA_CH0);
#endif
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

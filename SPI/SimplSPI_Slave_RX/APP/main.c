#include "SWM231.h"
#include "IOSPI.h"
#include <string.h>

#define RX_COUNT  32
uint16_t SPI_RXBuffer[RX_COUNT] = {0};
uint32_t SPI_RXIndex = 0;
volatile uint32_t SPI_RXFinish = 0;

void SerialInit(void);
void SPISlvInit(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();	
	
	SPISlvInit();
	
	IOSPI_Init();
	
	while(1==1)
	{
		IOSPI_CS_Low();
		
		for(i = 0; i < 16; i++)
			IOSPI_ReadWrite(i | (i << 4), 8);
		
		IOSPI_CS_High();
		
		while(SPI_RXFinish == 0) __NOP();
		SPI_RXFinish = 0;
		
		for(i = 0; i < SPI_RXIndex; i++)
			printf("%02X, ", SPI_RXBuffer[i]);
		printf("\r\n\r\n");
		
		SPI_RXIndex = 0;
		memset(SPI_RXBuffer, 0, 64);
		
		for(i = 0; i < SystemCoreClock/10; i++) __NOP();
	}
}


void SPISlvInit(void)
{
	SPI_InitStructure SPI_initStruct;
	
	PORT_Init(PORTA, PIN4, PORTA_PIN4_SPI0_SCLK, 1);
	PORT_Init(PORTA, PIN5, PORTA_PIN5_SPI0_MOSI, 1);
	PORT_Init(PORTA, PIN6, PORTA_PIN6_SPI0_MISO, 0);
	PORT_Init(PORTA, PIN7, PORTA_PIN7_SPI0_SSEL, 1);
	
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_SECOND_EDGE;
	SPI_initStruct.IdleLevel = SPI_HIGH_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 0;
	SPI_initStruct.RXThreshold = 4;		//接收FIFO中数据个数≥CTRL.RFTHR时产生中断
	SPI_initStruct.RXThresholdIEn = 1;
	SPI_initStruct.TXThreshold = 0;
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_initStruct.TXCompleteIEn  = 0;
	SPI_Init(SPI0, &SPI_initStruct);
	
	SPI_INTEn(SPI0, SPI_IT_CS_RISE);

	SPI_Open(SPI0);
}

void SPI0_Handler(void)
{
	if(SPI0->IF & SPI_IF_RFTHR_Msk)
	{
		while(SPI0->STAT & SPI_STAT_RFNE_Msk)
		{
			SPI_RXBuffer[SPI_RXIndex] = SPI0->DATA;
			if(SPI_RXIndex < RX_COUNT - 1) SPI_RXIndex++;
		}
		
		SPI0->IF = (1 << SPI_IF_RFTHR_Pos);	// 先读出数据，再清除标志
	}
	else if(SPI0->IF & SPI_IF_CSRISE_Msk)
	{
		SPI0->IF = SPI_IF_CSRISE_Msk;
		
		while(SPI0->STAT & SPI_STAT_RFNE_Msk)
		{
			SPI_RXBuffer[SPI_RXIndex] = SPI0->DATA;
			if(SPI_RXIndex < RX_COUNT - 1) SPI_RXIndex++;
		}
		
		SPI_RXFinish = 1;
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

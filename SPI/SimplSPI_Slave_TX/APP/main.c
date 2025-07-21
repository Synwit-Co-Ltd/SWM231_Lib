#include "SWM231.h"
#include "IOSPI.h"

uint16_t *SPI_TXBuff = 0;
uint32_t SPI_TXCount = 0;
uint32_t SPI_TXIndex = 0;

void SerialInit(void);
void SPISlvInit(void);
void SPISlvSend(uint16_t buff[], uint32_t cnt);

int main(void)
{	
	uint32_t i;
	uint16_t txbuff[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	uint16_t rxbuff[16];
	
	SystemInit();
	
	SerialInit();
	
	IOSPI_Init();
	
	SPISlvInit();
	
	SPISlvSend(txbuff, 16);

	while(1==1)
	{
		IOSPI_CS_Low();
		
		for(i = 0; i < 16; i++)
			rxbuff[i] = IOSPI_ReadWrite(0xFF, 8);
		
		IOSPI_CS_High();
		
		for(i = 0; i < 16; i++)
			printf("%02X, ", rxbuff[i]);
		printf("\n\n");
		
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
	SPI_initStruct.RXThreshold = 0;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThreshold = 4;		//发送FIFO中数据个数≤CTRL.TFTHR时产生中断
	SPI_initStruct.TXThresholdIEn = 1;
	SPI_initStruct.TXCompleteIEn  = 0;
	SPI_Init(SPI0, &SPI_initStruct);
	
	SPI_INTEn(SPI0, SPI_IT_CS_FALL);
	
	SPI_Open(SPI0);
}

void SPISlvSend(uint16_t buff[], uint32_t cnt)
{
	SPI_TXBuff = buff;
	SPI_TXCount = cnt;
}

void GPIOB3_GPIOA11_SPI0_Handler(void)
{
	if(SPI0->IF & SPI_IF_TFTHR_Msk)
	{
		while(SPI0->STAT & SPI_STAT_TFNF_Msk)
		{
			if(SPI_TXIndex < SPI_TXCount)
			{
				SPI0->DATA = SPI_TXBuff[SPI_TXIndex++];
			}
			else
			{
				SPI0->DATA = 0xFF;	// 没数据了，发送0xFF
			}
		}
		
		SPI0->IF = (1 << SPI_IF_TFTHR_Pos);
	}
	else if(SPI0->IF & SPI_IF_CSFALL_Msk)
	{
		SPI0->IF = SPI_IF_CSFALL_Msk;
		
		SPI_TXIndex = 0;
		
		SPI0->CTRL |=  (1 << SPI_CTRL_TFCLR_Pos);	// 发送FIFO清空
		SPI0->CTRL &= ~(1 << SPI_CTRL_TFCLR_Pos);
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

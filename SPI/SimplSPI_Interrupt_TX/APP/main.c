#include "SWM231.h"

uint16_t *SPI_TXBuff = 0; 
uint32_t SPI_TXCount = 0;
uint32_t SPI_TXIndex = 0;

void SerialInit(void);
void SPIMstInit(void);
void SPIMstSend(uint16_t buff[], uint32_t cnt);

int main(void)
{	
	uint16_t buff[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
	
	SystemInit();
	
	SerialInit();

	SPIMstInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//用作中断进入指示信号
	
	while(1==1)
	{
		SPIMstSend(buff, 16);
		
		for(int i = 0; i < SystemCoreClock/10; i++) __NOP();
	}
}


void SPIMstInit(void)
{
	SPI_InitStructure SPI_initStruct;
	
	GPIO_Init(GPIOA, PIN7, 1, 0, 0, 0);				//软件控制片选
#define SPI_CS_Low()	GPIO_ClrBit(GPIOA, PIN7)
#define SPI_CS_High()	GPIO_SetBit(GPIOA, PIN7)
	SPI_CS_High();
	
	PORT_Init(PORTA, PIN4, PORTA_PIN4_SPI0_SCLK, 0);
	PORT_Init(PORTA, PIN5, PORTA_PIN5_SPI0_MOSI, 0);
	PORT_Init(PORTA, PIN6, PORTA_PIN6_SPI0_MISO, 1);
	
	SPI_initStruct.clkDiv = SPI_CLKDIV_32;
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_SECOND_EDGE;
	SPI_initStruct.IdleLevel = SPI_HIGH_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 1;
	SPI_initStruct.RXThreshold    = 0;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThreshold    = 4;	// 当发送FIFO中数据少于4个时产生中断
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_initStruct.TXCompleteIEn  = 0;
	SPI_Init(SPI0, &SPI_initStruct);
	
	SPI_Open(SPI0);
}

void SPIMstSend(uint16_t buff[], uint32_t cnt)
{	
	SPI_TXBuff = buff;
	SPI_TXCount = cnt;
	SPI_TXIndex = 0;
	
	SPI_CS_Low();
	
	SPI_INTEn(SPI0, SPI_IT_TX_DONE);
 	SPI_INTEn(SPI0, SPI_IT_TX_THRES);
}

void SPI0_Handler(void)
{
	if(SPI_INTStat(SPI0, SPI_IT_TX_THRES))
	{
		while((SPI_IsTXFull(SPI0) == 0) && (SPI_TXIndex < SPI_TXCount))
		{
			SPI_Write(SPI0, SPI_TXBuff[SPI_TXIndex++]);
		}
		
		if(SPI_TXIndex == SPI_TXCount)
		{
			SPI_INTDis(SPI0, SPI_IT_TX_THRES);
		}
		
		SPI_INTClr(SPI0, SPI_IT_TX_THRES);	//清除中断标志，必须在填充TX FIFO后清中断标志
	}
	
	if(SPI_INTStat(SPI0, SPI_IT_TX_DONE))
	{
		SPI_INTClr(SPI0, SPI_IT_TX_DONE);
		
		if(SPI_TXIndex == SPI_TXCount)	// 要发送的数据已全部填入SPI TX FIFO
		{
			SPI_CS_High();
			SPI_INTDis(SPI0, SPI_IT_TX_DONE);
		}
	}
	
	GPIO_InvBit(GPIOA, PIN5);
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

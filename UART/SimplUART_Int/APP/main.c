#include "SWM231.h"
#include "CircleBuffer.h"

volatile bool msg_rcvd = false;

CircleBuffer_t CirBuf;

void SerialInit(void);

int main(void)
{
	SystemInit();
	
	SerialInit();
	   	
	while(1==1)
	{
		if(msg_rcvd)
		{
			msg_rcvd = false;
			
			UART_INTEn(UART0, UART_IT_TX_THR);
		}
	}
}


void UART0_Handler(void)
{
	uint32_t chr;
	
	if(UART_INTStat(UART0, UART_IT_RX_THR | UART_IT_RX_TOUT))
	{
		while(UART_IsRXFIFOEmpty(UART0) == 0)
		{
			if(UART_ReadByte(UART0, &chr) == 0)
			{
				CirBuf_Write(&CirBuf, (uint8_t *)&chr, 1);
			}
		}
		
		if(UART_INTStat(UART0, UART_IT_RX_TOUT))
		{
			UART_INTClr(UART0, UART_IT_RX_TOUT);
			
			msg_rcvd = true;
		}
	}
	
	if(UART_INTStat(UART0, UART_IT_TX_THR))
	{
		while(!UART_IsTXFIFOFull(UART0))
		{
			if(!CirBuf_Empty(&CirBuf))
			{
				CirBuf_Read(&CirBuf, (uint8_t *)&chr, 1);
				
				UART_WriteByte(UART0, chr);
			}
			else
			{
				UART_INTDis(UART0, UART_IT_TX_THR);
				
				break;
			}
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
	UART_initStruct.RXThresholdIEn = 1;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;		//10个字符时间内未接收到新的数据则触发超时中断
	UART_initStruct.TimeoutIEn = 1;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}

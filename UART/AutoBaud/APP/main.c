#include "SWM231.h"

void SerialInit(void);

int main(void)
{
	uint32_t res, i;
	UART_InitStructure UART_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTA, PIN6, PORTA_PIN6_UART1_TX, 0);
	PORT_Init(PORTA, PIN7, PORTA_PIN7_UART1_RX, 1);
	
	UART_initStruct.Baudrate = 9600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART1, &UART_initStruct);
	UART_Open(UART1);
   	
	/* ����˵��
	*  ��UART0_TX�����ӵ�UART1_RX��UART0������57600��UART1������9600
	*  ��UART0����0xF8��UART1ͨ������0xF8�Զ�����������ʽ��Լ��Ĳ�������Ϊ57600
	****************************************************************************/
	while(1==1)
	{		
		UART_ABRStart(UART1, 0xF8);
		
		while((res = UART_ABRIsDone(UART1)) == 0)
		{
			UART_WriteByte(UART0, 0xF8);
			while(UART_IsTXBusy(UART0));
		}
		
		if(res == UART_ABR_RES_OK) printf("AutoBaud OK\r\n");
		else                       printf("AutoBaud Error\r\n");
		
		printf("UART0 Baud Reg: %d\r\n", UART_GetBaudrate(UART0));
		printf("UART1 Baud Reg: %d\r\n", UART_GetBaudrate(UART1));
		
		for(i = 0; i < 1000000; i++);
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

#include "SWM231.h"

#include <string.h>
#include "GD25Q21.h"

#define EEPROM_ADDR	  0x008000

uint8_t WrBuff[20] = {0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
				      0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
uint8_t RdBuff[20] = {0};

void SerialInit(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	GD25Q21_Init();
	
	GD25Q21_EraseSector(EEPROM_ADDR);
 	while(GD25Q21_IsBusy()) __NOP();	// 等待擦除操作完成
	
	GD25Q21_ReadData(EEPROM_ADDR, RdBuff, 20);
	
	printf("\r\nAfter Erase: \r\n");
	for(i = 0; i < 20; i++) printf("0x%02X, ", RdBuff[i]);
	
	/* 1线读 */
	GD25Q21_WritePage(EEPROM_ADDR, WrBuff, 20);
 	while(GD25Q21_IsBusy()) __NOP();	// 等待写入操作完成
	
	GD25Q21_ReadData(EEPROM_ADDR, RdBuff, 20);
	
	printf("\r\nAfter Write: \r\n");
	for(i = 0; i < 20; i++) printf("0x%02X, ", RdBuff[i]);
	
	/* 2线读 */
	memset(RdBuff, 0x00, 20);
	GD25Q21_ReadData_2bit(EEPROM_ADDR, RdBuff, 20);
	
	printf("\r\nDual Read: \r\n");
	for(i = 0; i < 20; i++) printf("0x%02X, ", RdBuff[i]);

	/* 2线读（DMA） */
	memset(RdBuff, 0x00, 20);
	GD25Q21_ReadData_2bit_DMA(EEPROM_ADDR, RdBuff, 20);
	
	printf("\r\nDual DMA Read: \r\n");
	for(i = 0; i < 20; i++) printf("0x%02X, ", RdBuff[i]);
	
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

#include "SWM231.h"
#include "CircleBuffer.h"

CircleBuffer_t CirBuf;

#define ADC_SIZE  250	// ADC �������ת�� 256 ��
uint16_t ADC_Result[ADC_SIZE] = {0};


void SerialInit(void);

int main(void)
{
	DMA_InitStructure DMA_initStruct;
	ADC_InitStructure ADC_initStruct;
	ADC_SEQ_InitStructure ADC_SEQ_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTA, PIN1,  PORTA_PIN1_ADC0_CH0,  0);
	PORT_Init(PORTB, PIN9,  PORTB_PIN9_ADC0_CH1,  0);
	PORT_Init(PORTB, PIN8,  PORTB_PIN8_ADC0_CH4,  0);
	PORT_Init(PORTB, PIN7,  PORTB_PIN7_ADC0_CH5,  0);
	PORT_Init(PORTB, PIN6,  PORTB_PIN6_ADC0_CH6,  0);
	PORT_Init(PORTB, PIN5,  PORTB_PIN5_ADC0_CH7,  0);
	
	ADC_initStruct.clkdiv = 3;
	ADC_initStruct.refsrc = ADC_REF_VDD;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE1;
	ADC_Init(ADC0, &ADC_initStruct);
	
	ADC_SEQ_initStruct.trig_src = ADC_TRIGGER_SW;
	ADC_SEQ_initStruct.samp_tim = 6;
	ADC_SEQ_initStruct.conv_cnt = ADC_SIZE;
	ADC_SEQ_initStruct.EOCIntEn = 0;
	ADC_SEQ_initStruct.channels = (uint8_t []){ ADC_CH0, 0xF };
	ADC_SEQ_Init(ADC0, ADC_SEQ1, &ADC_SEQ_initStruct);
	
	ADC0->CR |= (1 << ADC_CR_SEQ1DMAEN_Pos);
	
	ADC_Open(ADC0);
	
	DMA_initStruct.Mode = DMA_MODE_CIRCLE;
	DMA_initStruct.Unit = DMA_UNIT_HALFWORD;
	DMA_initStruct.Count = ADC_SIZE;
	DMA_initStruct.PeripheralAddr = (uint32_t)&ADC0->SEQ1DMA;
	DMA_initStruct.PeripheralAddrInc = 0;
	DMA_initStruct.MemoryAddr = (uint32_t)ADC_Result;
	DMA_initStruct.MemoryAddrInc = 1;
	DMA_initStruct.Handshake = DMA_CH0_ADC0SEQ1;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = DMA_IT_HALF | DMA_IT_DONE;
	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	DMA_CH_Open(DMA_CH0);
	
	ADC_Start(ADC_SEQ1, 0);
	
	while(1==1)
	{
		while(CirBuf_Full(&CirBuf) == 0) __NOP();
		
		for(int i = 0; i < 1000; i++)
		{
			uint16_t data;
			
			CirBuf_Read(&CirBuf, &data, 1);
			
			printf("%4d,", data & ADC_SEQ1DMA_DATA_Msk);
		}
		
		CirBuf_Clear(&CirBuf);	// ��ӡ�����д��� CirBuf �е��������Բ������ĵ�
	}
}


void GPIOB1_GPIOA9_DMA_Handler(void)
{
	if(DMA_CH_INTStat(DMA_CH0, DMA_IT_HALF))
	{
		DMA_CH_INTClr(DMA_CH0, DMA_IT_HALF);
		
		CirBuf_Write(&CirBuf, &ADC_Result[0], ADC_SIZE/2);
	}
	else if(DMA_CH_INTStat(DMA_CH0, DMA_IT_DONE))
	{
		DMA_CH_INTClr(DMA_CH0, DMA_IT_DONE);
		
		ADC_Start(ADC_SEQ1, 0);		// ת�� 250 �κ�ֹͣ������������
		
		CirBuf_Write(&CirBuf, &ADC_Result[ADC_SIZE/2], ADC_SIZE - ADC_SIZE/2);
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

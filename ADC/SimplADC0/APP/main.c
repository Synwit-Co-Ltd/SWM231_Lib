#include "SWM231.h"


#define ADC_USE_INT  0


void SerialInit(void);

int main(void)
{
	ADC_InitStructure ADC_initStruct;
	ADC_SEQ_InitStructure ADC_SEQ_initStruct;
	
	SystemInit();
	
	SerialInit();
	
//	PORT_Init(PORTA, PIN1,  PORTA_PIN1_ADC0_CH0,  0);	// ADC0_REFP 引脚，使用内部基准时基准电压会输出在此引脚上
	PORT_Init(PORTB, PIN9,  PORTB_PIN9_ADC0_CH1,  0);
	// OPA0_OUT => ADC0_CH2
	// OPA1_OUT => ADC0_CH3
	PORT_Init(PORTB, PIN8,  PORTB_PIN8_ADC0_CH4,  0);
	PORT_Init(PORTB, PIN7,  PORTB_PIN7_ADC0_CH5,  0);
	PORT_Init(PORTB, PIN6,  PORTB_PIN6_ADC0_CH6,  0);
	PORT_Init(PORTB, PIN5,  PORTB_PIN5_ADC0_CH7,  0);
	PORT_Init(PORTB, PIN4,  PORTB_PIN4_ADC0_CH8,  0);
	PORT_Init(PORTB, PIN3,  PORTB_PIN3_ADC0_CH9,  0);
//	PORT_Init(PORTA, PIN8,  PORTA_PIN8_ADC0_CH10, 0);	// XTAL_OUT
//	PORT_Init(PORTB, PIN0,  PORTB_PIN0_ADC0_CH11, 0);	// SWCLK
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_ADC0_CH12, 0);
	PORT_Init(PORTB, PIN10, PORTB_PIN10_ADC0_CH13,0);
	// Temperator_Sensor => ADC0_CH14
	
	ADC_initStruct.clkdiv = 4;
	ADC_initStruct.refsrc = ADC_REF_VDD;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE1;
	ADC_Init(ADC0, &ADC_initStruct);
	
	ADC_SEQ_initStruct.trig_src = ADC_TRIGGER_SW;
	ADC_SEQ_initStruct.samp_tim = 6;
	ADC_SEQ_initStruct.conv_cnt = 1;
	ADC_SEQ_initStruct.EOCIntEn = ADC_USE_INT;
	ADC_SEQ_initStruct.channels = (uint8_t []){ ADC_CH1, 0xF };
	ADC_SEQ_Init(ADC0, ADC_SEQ0, &ADC_SEQ_initStruct);
	
	ADC_Open(ADC0);
	
#if ADC_USE_INT
	ADC_Start(ADC_SEQ0);
	
	while(1==1)
	{
	}
#else
	while(1==1)
	{
		ADC_Start(ADC_SEQ0);
		while(ADC_Busy(ADC0)) __NOP();
		printf("%4d,", ADC_Read(ADC0, ADC_CH1));
	}
#endif
}


void ADC_Handler(void)
{
	if(ADC_INTStat(ADC0, ADC_SEQ0, ADC_IT_EOC))
	{
		ADC_INTClr(ADC0, ADC_SEQ0, ADC_IT_EOC);
		
		printf("%4d,", ADC_Read(ADC0, ADC_CH1));
		
		ADC_Start(ADC_SEQ0);
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

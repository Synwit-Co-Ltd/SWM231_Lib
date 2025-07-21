#include "SWM231.h"

/* ע�⣺
 *	оƬ�� ISP��SWD ����Ĭ�Ͽ������������裬���������߹��ģ�������������߹��ģ�����ǰ��ر��������ŵ���������������
 *
 *	��Ҫע��FLASH��������˯��ģʽ��������Ҫ�ȴ�20uSʱ�䣬����˯�ߺ󣬲��ܷ���FLASH,���Խ��뻽�Ѻ͵ȴ���Ҫ��RAM��ִ�С�
 */
 
extern void EnterSleepMode(void) ;

int main(void)
{
	uint32_t i;
	for(i = 0; i < SystemCoreClock/2; i++)  __NOP();//��ֹ���Թ����п���
	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//�� LED��ָʾ����ִ��״̬
	GPIO_SetBit(GPIOA, PIN5);				//����LED
	
	GPIO_Init(GPIOA, PIN4, 0, 1, 0, 0);		//�Ӱ���������ʹ��
	SYS->PAWKEN |= (1 << PIN4);				//����PA4���ŵ͵�ƽ����
	
	SYS->RCCR |= SYS_RCCR_LON_Msk;			//����ģʽʹ�õ�Ƶʱ�� 
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);							//����LED
		for(int i = 0; i < SystemCoreClock/8; i++) __NOP();
		GPIO_ClrBit(GPIOA, PIN5);							//Ϩ��LED
		
		Flash_Param_at_xMHz(48);
		switchToHRC();	//����ǰ�л����ڲ�RCʱ�ӣ�����͹���ģʽ֮���Զ��л����ڲ���Ƶʱ��
		
		SYS->XTALCR&= ~((1 << SYS_XTALCR_ON_Pos) | (1 << SYS_XTALCR_DET_Pos)  );//�ر�����,���͹���
		
		SYS->CLKEN0 &=~ (1 << SYS_CLKEN0_ANAC_Pos); //�ر�ģ��ģ��ʱ�ӽ�ʡ����--����ر�
		
		EnterSleepMode();	//��SRAM�н���sleep,�ȴ�����
		
		SYS->CLKEN0 |=(1 << SYS_CLKEN0_ANAC_Pos); 
		switchToHRC();
		SystemCoreClockUpdate();
		Flash_Param_at_xMHz(CyclesPerUs);
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



#include "SWM231.h"

/* 注意：
 *	芯片的 ISP、SWD 引脚默认开启了上拉电阻，会增加休眠功耗，若想获得最低休眠功耗，休眠前请关闭所有引脚的上拉和下拉电阻
 *
 *	需要注意FLASH会进入深度睡眠模式，唤醒需要等待20uS时间，进入睡眠后，不能访问FLASH,所以进入唤醒和等待需要在RAM中执行。
 */
 
extern void EnterSleepMode(void) ;

int main(void)
{
	uint32_t i;
	for(i = 0; i < SystemCoreClock/2; i++)  __NOP();//防止调试过程中卡死
	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//接 LED，指示程序执行状态
	GPIO_SetBit(GPIOA, PIN5);				//点亮LED
	
	GPIO_Init(GPIOA, PIN4, 0, 1, 0, 0);		//接按键，上拉使能
	SYS->PAWKEN |= (1 << PIN4);				//开启PA4引脚低电平唤醒
	
	SYS->RCCR |= SYS_RCCR_LON_Msk;			//休眠模式使用低频时钟 
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);							//点亮LED
		for(int i = 0; i < SystemCoreClock/8; i++) __NOP();
		GPIO_ClrBit(GPIOA, PIN5);							//熄灭LED
		
		Flash_Param_at_xMHz(48);
		switchToHRC();	//休眠前切换到内部RC时钟，进入低功耗模式之后自动切换到内部低频时钟
		
		SYS->XTALCR&= ~((1 << SYS_XTALCR_ON_Pos) | (1 << SYS_XTALCR_DET_Pos)  );//关闭外振,降低功耗
		
		SYS->CLKEN0 &=~ (1 << SYS_CLKEN0_ANAC_Pos); //关闭模拟模块时钟节省功耗--按需关闭
		
		EnterSleepMode();	//在SRAM中进入sleep,等待唤醒
		
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



/****************************************************************************************************************************************** 
* 文件名称: SWM231_foc.c
* 功能说明:	SWM231单片机的FOC模块驱动库
* 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* 注意事项:
* 版本日期:	V1.0.0		2016年1月30日
* 升级记录:  
*
*
*******************************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology
*******************************************************************************************************************************************/
#include "SWM231.h"
#include "SWM231_foc.h"


/****************************************************************************************************************************************** 
* 函数名称:	FOC_Init()
* 功能说明:	FOC模块初始化
* 输    入: FOC_TypeDef * FOCx	指定要被设置的FOC模块，有效值包括FOC
*			FOC_InitStructure * initStruct    包含FOC模块相关设定值的结构体
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void FOC_Init(FOC_TypeDef * FOCx, FOC_InitStructure * initStruct)
{
	SYS->CLKEN0 |= SYS_CLKEN0_FOC_Msk;
	
	for(int i = 0; i < 3; i++) __NOP();
	
	FOCx->CR = (initStruct->Mode		   << FOC_CR_MODE_Pos) |
			   (initStruct->Clark3Input	   << FOC_CR_CLARKI3_Pos) |
			   (initStruct->SVPWM2Resistor << FOC_CR_PWMI2_Pos);
	
	FOC_INTClr(initStruct->INTEn);
	FOC_INTEn(initStruct->INTEn);
}


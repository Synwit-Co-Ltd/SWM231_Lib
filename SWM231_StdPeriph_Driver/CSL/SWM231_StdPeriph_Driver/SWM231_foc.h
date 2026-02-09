#ifndef __SWM231_FOC_H__
#define __SWM231_FOC_H__


typedef struct {
	uint8_t  Mode;				// FOC_MODE_ALL, FOC_MODE_PART
	
	uint8_t  Clark3Input;		// 0 Clark 工作于两项输入模式（CLARKIa、CLARKIb），1 Clark 工作于三相输入模式（CLARKIa、CLARKIb、CLARKIc）
	
	uint8_t  SVPWM2Resistor;	// 0 SVPWM 使用单电阻采样，1 SVPWM 使用双电阻采样
	
	uint8_t  INTEn;				// interrupt enable，有效值有 FOC_IT_FOC、FOC_IT_CLARK、FOC_IT_PARK、FOC_IT_PID、FOC_IT_iPARK、FOC_IT_SVPWM 及其“或”
} FOC_InitStructure;


#define FOC_MODE_PART	0		// CLARK、PARK、PID、iPARK、SVPWM 功能通过 FOC->STA 中的对应位启动
#define FOC_MODE_ALL	1		// 所有功能通过 FOC->STA.FOC 控制位整体启动


/* Interrupt Type */
#define FOC_IT_FOC		FOC_IE_FOC_Msk
#define FOC_IT_CLARK	FOC_IE_CLARK_Msk
#define FOC_IT_PARK		FOC_IE_PARK_Msk
#define FOC_IT_PID		FOC_IE_PID_Msk
#define FOC_IT_iPARK	FOC_IE_IPARK_Msk
#define FOC_IT_SVPWM	FOC_IE_SVPWM_Msk



void FOC_Init(FOC_TypeDef * FOCx, FOC_InitStructure * initStruct);


static __INLINE void FOC_Clark_Input(int16_t ia, int16_t ib)
{
	FOC->CLARKIA = ia;
	FOC->CLARKIB = ib;
}

static __INLINE void FOC_Clark_Input3(int16_t ia, int16_t ib, int16_t ic)
{
	FOC->CLARKIA = ia;
	FOC->CLARKIB = ib;
	FOC->CLARKIC = ic;
}

static __INLINE void FOC_Clark_Start(void)
{
	FOC->STA |= FOC_STA_CLARK_Msk;
}

static __INLINE uint32_t FOC_Clark_Busy(void)
{
	return FOC->STA & FOC_STA_CLARK_Msk;
}

static __INLINE uint32_t FOC_Clark_Error(void)
{
	uint32_t overflow = FOC->SR & FOC_SR_CLARKOF_Msk;
	
	FOC->SR = overflow;
	
	return overflow;
}

static __INLINE void FOC_Clark_Result(int16_t * alpha, int16_t * beta)
{
	*alpha = FOC->CLARKOA;
	*beta  = FOC->CLARKOB;
}


static __INLINE void FOC_Park_Input(int16_t alpha, int16_t beta, int16_t sin, int16_t cos)
{
	FOC->PARKIA = alpha;
	FOC->PARKIB = beta;
	FOC->PARKSIN = sin;
	FOC->PARKCOS = cos;
}

static __INLINE void FOC_Park_Start(void)
{
	FOC->STA |= FOC_STA_PARK_Msk;
}

static __INLINE uint32_t FOC_Park_Busy(void)
{
	return FOC->STA & FOC_STA_PARK_Msk;
}

static __INLINE uint32_t FOC_Park_Error(void)
{
	uint32_t overflow = FOC->SR & FOC_SR_PARKOF_Msk;
	
	FOC->SR = overflow;
	
	return overflow;
}

static __INLINE void FOC_Park_Result(int16_t * id, int16_t * iq)
{
	*id = FOC->PARKOD;
	*iq = FOC->PARKOQ;
}


static __INLINE void FOC_iPark_Input(int16_t vd, int16_t vq, int16_t sin, int16_t cos)
{
	FOC->IPARKID = vd;
	FOC->IPARKIQ = vq;
	FOC->IPARKSIN = sin;
	FOC->IPARKCOS = cos;
}

static __INLINE void FOC_iPark_Start(void)
{
	FOC->STA |= FOC_STA_IPARK_Msk;
}

static __INLINE uint32_t FOC_iPark_Busy(void)
{
	return FOC->STA & FOC_STA_IPARK_Msk;
}

static __INLINE uint32_t FOC_iPark_Error(void)
{
	uint32_t overflow = FOC->SR & FOC_SR_IPARKOF_Msk;
	
	FOC->SR = overflow;
	
	return overflow;
}

static __INLINE void FOC_iPark_Result(int16_t * alpha, int16_t * beta)
{
	*alpha = FOC->IPARKOA;
	*beta  = FOC->IPARKOB;
}


// it: interrupt type，有效值有 FOC_IT_FOC、FOC_IT_CLARK、FOC_IT_PARK、FOC_IT_PID、FOC_IT_iPARK、FOC_IT_SVPWM 及其“或”
static __INLINE void FOC_INTEn(uint32_t it)
{
	FOC->IE |= it;
}

static __INLINE void FOC_INTDis(uint32_t it)
{
	FOC->IE &= ~it;
}

static __INLINE void FOC_INTClr(uint32_t it)
{
	FOC->SR = (it << 1);
}

static __INLINE uint32_t FOC_INTStat(uint32_t it)
{
	return (FOC->SR & (it << 1));
}

#endif //__SWM231_FOC_H__

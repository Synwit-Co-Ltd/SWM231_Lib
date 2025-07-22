#ifndef __SWM231_FOC_H__
#define __SWM231_FOC_H__


typedef struct {
	uint8_t  Mode;				// FOC_MODE_ALL, FOC_MODE_PART
	
	uint8_t  Clark3Input;		// 0 Clark ��������������ģʽ��CLARKIa��CLARKIb����1 Clark ��������������ģʽ��CLARKIa��CLARKIb��CLARKIc��
	
	uint8_t  SVPWM2Resistor;	// 0 SVPWM ʹ�õ����������1 SVPWM ʹ��˫�������
	
	uint8_t  INTEn;				// interrupt enable����Чֵ�� FOC_IT_FOC��FOC_IT_CLARK��FOC_IT_PARK��FOC_IT_PID��FOC_IT_iPARK��FOC_IT_SVPWM ���䡰��
} FOC_InitStructure;


#define FOC_MODE_PART	0		// CLARK��PARK��PID��iPARK��SVPWM ����ͨ�� FOC->STA �еĶ�Ӧλ����
#define FOC_MODE_ALL	1		// ���й���ͨ�� FOC->STA.FOC ����λ��������


/* Interrupt Type */
#define FOC_IT_FOC		FOC_IE_FOC_Msk
#define FOC_IT_CLARK	FOC_IE_CLARK_Msk
#define FOC_IT_PARK		FOC_IE_PARK_Msk
#define FOC_IT_PID		FOC_IE_PID_Msk
#define FOC_IT_iPARK	FOC_IE_IPARK_Msk
#define FOC_IT_SVPWM	FOC_IE_SVPWM_Msk



void FOC_Init(FOC_TypeDef * FOCx, FOC_InitStructure * initStruct);




// it: interrupt type����Чֵ�� FOC_IT_FOC��FOC_IT_CLARK��FOC_IT_PARK��FOC_IT_PID��FOC_IT_iPARK��FOC_IT_SVPWM ���䡰��
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

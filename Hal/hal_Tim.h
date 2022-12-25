#ifndef __HAL_TIM_H
#define __HAL_TIM_H

#include "stm32f4xx_hal.h"

//��ʱ������
typedef enum
{
	T_LED,      ///LED��ʱ�� 
	T_SUM,
}TIMER_ID_TYPEDEF;

typedef enum
{
	T_SUCEFFUL,
	T_FAIL,
}TIMER_RESULT_TYPEDEF;

///��ʱ��״̬
typedef enum
{
	TIM_EMPUT,	///��ʱ����
	TIM_START,  ///��ʱ������
	TIM_STOP,   ///��ʱ��ֹͣ
}TIMER_STATU_TYPEDEF;

///��ʱ�������ʼ��
typedef struct
{
	void (*func)(void); ///����ָ��
	TIMER_STATU_TYPEDEF state; ///��ʱ����״̬ 0��ʱ��δ����  1��ʱ������
	unsigned short Corrtcount;  ///��ʱ����ǰ����ֵ
	unsigned short Period;	///��ʱ����ʱƵ��

}TASK_Timer_TYPEDEF;
	
void  hal_CreatTask(TIMER_ID_TYPEDEF id,void(*proc)(void),unsigned short Period,TIMER_STATU_TYPEDEF state);

TIMER_RESULT_TYPEDEF hal_ResetTimer(TIMER_ID_TYPEDEF id,TIMER_STATU_TYPEDEF sta);

void hal_tim4_Init(void);
void HAL_Tim4_MspInit(TIM_HandleTypeDef *handle);
#endif

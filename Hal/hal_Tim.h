#ifndef __HAL_TIM_H
#define __HAL_TIM_H

#include "stm32f4xx_hal.h"

//定时器任务
typedef enum
{
	T_LED,      ///LED定时器 
	T_SUM,
}TIMER_ID_TYPEDEF;

typedef enum
{
	T_SUCEFFUL,
	T_FAIL,
}TIMER_RESULT_TYPEDEF;

///定时器状态
typedef enum
{
	TIM_EMPUT,	///定时器空
	TIM_START,  ///定时器启动
	TIM_STOP,   ///定时器停止
}TIMER_STATU_TYPEDEF;

///定时器任务初始化
typedef struct
{
	void (*func)(void); ///函数指针
	TIMER_STATU_TYPEDEF state; ///定时器的状态 0定时器未启动  1定时器启动
	unsigned short Corrtcount;  ///定时器当前计数值
	unsigned short Period;	///定时器定时频率

}TASK_Timer_TYPEDEF;
	
void  hal_CreatTask(TIMER_ID_TYPEDEF id,void(*proc)(void),unsigned short Period,TIMER_STATU_TYPEDEF state);

TIMER_RESULT_TYPEDEF hal_ResetTimer(TIMER_ID_TYPEDEF id,TIMER_STATU_TYPEDEF sta);

void hal_tim4_Init(void);
void HAL_Tim4_MspInit(TIM_HandleTypeDef *handle);
#endif

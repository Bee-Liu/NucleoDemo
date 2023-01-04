#include "hal_Tim.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_nucleo.h"

static void Hal_Tim4_Init(void);
static void hal_TimerHandle(void);



TIM_HandleTypeDef Htim4;

volatile  TASK_Timer_TYPEDEF  TASK_Timer[T_SUM];

void hal_tim4_Init(void)
{
	unsigned char i;

	Hal_Tim4_Init();
	
	for (i = 0;i < T_SUM;i++)
	{
		TASK_Timer[i].func = 0;
		TASK_Timer[i].state = TIM_STOP;
		TASK_Timer[i].Period = 0;
		TASK_Timer[i].Corrtcount = 0;
	}

}

void  hal_CreatTask(TIMER_ID_TYPEDEF id, void(*proc)(void), unsigned short Period, TIMER_STATU_TYPEDEF state)
{
	TASK_Timer[id].func = proc;
	TASK_Timer[id].state = state;
	TASK_Timer[id].Period = Period;
	TASK_Timer[id].Corrtcount = 0;
}

///复位定时器重装载计时
TIMER_RESULT_TYPEDEF hal_ResetTimer(TIMER_ID_TYPEDEF id, TIMER_STATU_TYPEDEF sta)
{
	if (TASK_Timer[id].func)		//判断定时器是否存在
	{
		TASK_Timer[id].state = sta;
		TASK_Timer[id].Corrtcount = 0;

		return T_SUCEFFUL;
	}
	else
	{
		return T_FAIL;
	}
}


//定时器中断执行函数  
static void hal_TimerHandle(void)
{
	unsigned char i;

	for (i = 0; i < T_SUM; i++)
	{
		if ((TASK_Timer[i].func) && (TASK_Timer[i].state == TIM_START))
		{
			TASK_Timer[i].Corrtcount++;	//每+1就等于50us,100us,
			if (TASK_Timer[i].Corrtcount >= TASK_Timer[i].Period)
			{
				TASK_Timer[i].state = TIM_STOP;
				TASK_Timer[i].Corrtcount = TASK_Timer[i].Corrtcount;

				TASK_Timer[i].func();
			}

		}

	}
}

static void Hal_Tim4_Init(void)
{
	
	Htim4.Instance = TIM4;
	Htim4.Init.Prescaler = 1600 - 1;//1s
	Htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	Htim4.Init.Period = 10000 - 1;
	Htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

	HAL_TIM_Base_Init(&Htim4);
		__HAL_RCC_TIM4_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(TIM4_IRQn);
	__HAL_TIM_CLEAR_IT(&Htim4, TIM_IT_UPDATE);

	HAL_TIM_Base_Start_IT(&Htim4);
}



void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim)
{
	// if (htim->Instance == TIM2)
    // {
    //     __HAL_RCC_TIM2_CLK_ENABLE();
    // }
	// if(htim->Instance == TIM4)
	// {
	// 	__HAL_RCC_TIM4_CLK_ENABLE();
	// 	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);
	// 	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	// }
}





void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&Htim4);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
}

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
// {
	
// 	if (htim->Instance == TIM4)
// 	{
		
		
		
// 	}
	
// }




#include "hal_Tim.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_nucleo.h"

static void Hal_Tim4_Init(void);
static void hal_TimerHandle(void);
static void MX_NVIC_Init(void);
static void Error_Handler(void);
static void SystemClock_Config(void);


TIM_HandleTypeDef Htim4;

volatile  TASK_Timer_TYPEDEF  TASK_Timer[T_SUM];

void hal_tim4_Init(void)
{
	unsigned char i;

	Hal_Tim4_Init();
	//MX_NVIC_Init();
	// HAL_TIM_Base_Start_IT(&Htim4);
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
	//	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	//	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

		/* USER CODE BEGIN TIM4_Init 1 */

		/* USER CODE END TIM4_Init 1 */
	Htim4.Instance = TIM4;
	Htim4.Init.Prescaler = 1600 - 1;//1s
	Htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	Htim4.Init.Period = 10000 - 1;
	Htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	// Htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&Htim4);
	__HAL_TIM_CLEAR_IT(&Htim4, TIM_IT_UPDATE);


	HAL_TIM_Base_Start_IT(&Htim4);
}



void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim)
{
	__HAL_RCC_TIM4_CLK_ENABLE();
	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

static void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	*/
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	//RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
		| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

// void HAL_Tim4_MspInit(TIM_HandleTypeDef* handle)
// {
// 	if (handle->Instance == TIM4)
// 	{
// 		GPIO_InitTypeDef GPIO_Handler;
// 		__HAL_RCC_GPIOB_CLK_ENABLE();
// 		__TIM4_CLK_ENABLE();

// 		GPIO_Handler.Pin = GPIO_PIN_6;
// 		GPIO_Handler.Mode = GPIO_MODE_AF_PP;
// 		GPIO_Handler.Pull = GPIO_NOPULL;
// 		GPIO_Handler.Speed = GPIO_SPEED_FREQ_LOW;
// 		GPIO_Handler.Alternate = GPIO_AF2_TIM4;
// 		HAL_GPIO_Init(GPIOB, &GPIO_Handler);
// 	}
// }

void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&Htim4);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{

	if (htim->Instance == TIM4)
	{
		BSP_LED_Toggle(LED2);
		//HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);

		// __HAL_TIM_CLEAR_FLAG(&TIM4_Handler, TIM_FLAG_UPDATE);
		// hal_TimerHandle();
	}
}


static void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}



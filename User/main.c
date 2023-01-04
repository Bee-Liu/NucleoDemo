#include "OS_System.h"
#include "hal_led.h"
#include "hal_Tim.h"
#include "hal_pwm.h"

int main()
{


	uint16_t pulse = 0; // 占空比
	uint8_t dir=1;
	hal_CPUInit();
	HAL_Init();
	OS_TaskInit();
	Led_init();
	hal_tim4_Init();
	HAL_PWM_Init();
	GPIOC_14_Init();
	// OS_CreatTask(OS_TASK1, LED_proc, 1, OS_RUN);
	while (1)
	{
		
		while (dir)
		{
			pulse++;

			TIM2->CCR1 = pulse;
			if(pulse==250) dir=0;
			HAL_Delay(1);

		}
		while (!dir)
		{
			pulse--;
			if(pulse==0) dir=1;
			TIM2->CCR1 = pulse;
			HAL_Delay(1);
		}
		// HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_10);
		 HAL_Delay(10);
	}



	OS_Start();

}


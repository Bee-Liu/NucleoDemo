#include "OS_System.h"
#include "hal_led.h"
#include "hal_Tim.h"


int main()
{
	hal_CPUInit();	
	HAL_Init();
	OS_TaskInit();
	Led_init();	
	hal_tim4_Init();
	
	OS_CreatTask(OS_TASK1, LED_proc, 100, OS_RUN);




	OS_Start();

}


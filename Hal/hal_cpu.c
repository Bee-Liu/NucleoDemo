#include "OS_System.h"
#include "hal_led.h"
#include "hal_cpu.h"
#include "stm32f4xx_hal.h"

static void hal_CPU_Critical_Control(CPU_EA_TYPEDEF cmd,unsigned char *pSta);


void hal_CPUInit(void)
{
	OS_CPUInterruptCBSRegister(hal_CPU_Critical_Control);
}







/********************************************************************************************************
*  @函数名   hal_getprimask						                                                           
*  @描述     获取CPU总中断状态							                                     
*  @参数     无
*  @返回值   0-总中断关闭 1-总中断打开
*  @注意     无
********************************************************************************************************/
static unsigned char hal_getprimask(void)
{
	return (!__get_PRIMASK());		//0是中断打开，1是中断关闭，所以要取反
}


/********************************************************************************************************
*  @函数名   hal_CPU_Critical_Control						                                                           
*  @描述     CPU临界处理控制						                                     
*  @参数     cmd-控制命令  *pSta-总中断状态
*  @返回值   无
*  @注意     无
********************************************************************************************************/
static void hal_CPU_Critical_Control(CPU_EA_TYPEDEF cmd,unsigned char *pSta)
{
	if(cmd == CPU_ENTER_CRITICAL)
	{
		*pSta = hal_getprimask();	//保存中断状态
		__disable_irq();		//关CPU总中断
	}else if(cmd == CPU_EXIT_CRITICAL)
	{
		if(*pSta)
		{
			__enable_irq();		//打开中断
		}else 
		{
			__disable_irq();	//关闭中断
		}
	}
}


void SysTick_Handler(void)
{
	HAL_IncTick();
	OS_ClockInterruptHandle();
	 //定时中断处理
}

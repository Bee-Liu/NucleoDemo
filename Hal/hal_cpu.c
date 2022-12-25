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
*  @������   hal_getprimask						                                                           
*  @����     ��ȡCPU���ж�״̬							                                     
*  @����     ��
*  @����ֵ   0-���жϹر� 1-���жϴ�
*  @ע��     ��
********************************************************************************************************/
static unsigned char hal_getprimask(void)
{
	return (!__get_PRIMASK());		//0���жϴ򿪣�1���жϹرգ�����Ҫȡ��
}


/********************************************************************************************************
*  @������   hal_CPU_Critical_Control						                                                           
*  @����     CPU�ٽ紦�����						                                     
*  @����     cmd-��������  *pSta-���ж�״̬
*  @����ֵ   ��
*  @ע��     ��
********************************************************************************************************/
static void hal_CPU_Critical_Control(CPU_EA_TYPEDEF cmd,unsigned char *pSta)
{
	if(cmd == CPU_ENTER_CRITICAL)
	{
		*pSta = hal_getprimask();	//�����ж�״̬
		__disable_irq();		//��CPU���ж�
	}else if(cmd == CPU_EXIT_CRITICAL)
	{
		if(*pSta)
		{
			__enable_irq();		//���ж�
		}else 
		{
			__disable_irq();	//�ر��ж�
		}
	}
}


void SysTick_Handler(void)
{
	HAL_IncTick();
	OS_ClockInterruptHandle();
	 //��ʱ�жϴ���
}

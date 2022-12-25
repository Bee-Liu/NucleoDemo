#include "hal_led.h"
#include "Os_System.h"
#include "stm32f4xx_nucleo.h"
#include "hal_Tim.h"

Queue4    LedCmdBuff[LED_NUM]; ///定义一个队列数组为每一个Led外设单独分配一个队列缓存

unsigned char LedLoadFlag[LED_NUM];  //判断队列的运行状态的标志位     

//为每一个Led的外设类型都定义了一个计时存储数组和 指针数组（用来指向特效功能数组的灯的状态）
unsigned char LedTimer[LED_NUM];
unsigned short* pLed[LED_NUM];


///static:表示只在本.c文件中使用

static void hal_Led1Drive(unsigned char sta);
static void hal_LedHandle(void); //放到定时器中断中
//static void LedBlink_Handle(void); //led翻转
///定时器中断计时1ms


unsigned short  Led_Close[] = { 0,10,LED_NUM_END }; //led初始化：灭
unsigned short  Led_Light[] = { 1,10,LED_NUM_END }; //led常亮
unsigned short  Led_Light_100ms[] = { 1,100,0,LED_NUM_END }; //led亮100ms后熄灭 
unsigned short  Led_Turn1[] = { 1,10,0,10,1,10,0,100,LED_NUM_ANG,4 }; //led以1秒的频率快闪两次
unsigned short  Led_Turn2[] = { 1,50,0,50,1,50,0,200,LED_NUM_ANG,4 };  //led每两秒闪两次


void  (*hal_LedDrive[LED_NUM])(unsigned char) = {
													hal_Led1Drive,
};


void Led_init()  //LED初始化函数
{
	unsigned char i;
	BSP_LED_Init(LED2);///LED初始化配置

	for (i = 0;i < LED_NUM;i++)
	{
		LedLoadFlag[i] = 0;
		pLed[i] = (unsigned short*)Led_Close;
		LedTimer[i] = *(pLed[i] + 1);
		QueueEmpty(LedCmdBuff[i]);
	}
	hal_CreatTask(T_LED, hal_LedHandle,1 , TIM_START);
	LedMsgInput(LED1, LED_TURN1, 1);
	LedMsgInput(LED1, LED_TURN2, 0);
}

///LED任务入队函数
void LedMsgInput(unsigned char type, LED_EFFECT_TYPEDEF cmd, unsigned char clr)
{
	unsigned char cLedCmd;  ///用来存储特效功能变量
	if (type >= LED_NUM)
	{
		return;
	}
	cLedCmd = cmd;
	if (clr)
	{
		QueueEmpty(LedCmdBuff[type]);  ///清空队列缓存
		LedLoadFlag[type] = 0;		   ///队列不在运行状态
	}
	QueueDataIn(LedCmdBuff[type], &cLedCmd, 1);
}





void LED_proc(void)
{

	// BSP_LED_Toggle(LED2);
	// unsigned char i;
	// unsigned char cmd;
	// for (i = 0;i < LED_NUM;i++)
	// {
	// 	if ((QueueDataLen(LedCmdBuff[i]) > 0) && (LedLoadFlag[i] == 0))
	// 	{
	// 		QueueDataOut(LedCmdBuff[i], &cmd);
	// 		LedLoadFlag[i] = 1;
	// 		switch (cmd)
	// 		{
	// 		case LED_CLOSE:
	// 			pLed[i] = (unsigned short*)Led_Close;
	// 			LedTimer[i] = *(pLed[i] + 1);
	// 			break;
	// 		case LED_LIGHT:
	// 			pLed[i] = (unsigned short*)Led_Light;
	// 			LedTimer[i] = *(pLed[i] + 1);
	// 			break;
	// 		case LED_LIGHT_100MS:
	// 			pLed[i] = (unsigned short*)Led_Light_100ms;
	// 			LedTimer[i] = *(pLed[i] + 1);
	// 			break;
	// 		case LED_TURN1:
	// 			pLed[i] = (unsigned short*)Led_Turn1;
	// 			LedTimer[i] = *(pLed[i] + 1);
	// 			break;
	// 		case LED_TURN2:
	// 			pLed[i] = (unsigned short*)Led_Turn2;
	// 			LedTimer[i] = *(pLed[i] + 1);
	// 			break;

	// 		}
	// 	}
	// }
	//hal_Led1Drive(1);
}

static void hal_LedHandle(void)
{
	unsigned char i;
	for (i = 0;i < LED_NUM;i++)
	{
		if (LedTimer[i])
		{
			LedTimer[i]--;
		}
		if (!LedTimer[i])
		{
			if (*(pLed[i] + 2) == LED_NUM_END)
			{
				LedLoadFlag[i] = 0;
			}
			else
			{
				pLed[i] += 2;
				if (*pLed[i] == LED_NUM_ANG)
				{
					pLed[i] = pLed[i] - (*(pLed[i] + 1) * 2);
				}
				LedTimer[i] = *(pLed[i] + 1);
			}

		}
		hal_LedDrive[i](*pLed[i]);
	}
	hal_ResetTimer(T_LED, TIM_START);
}


static void hal_Led1Drive(unsigned char sta)
{
	if (sta)
	{
		BSP_LED_On(LED2);
	}
	else
	{
		BSP_LED_Off(LED2);
	}
}

#ifndef __HAL_LED_H
#define __HAL_LED_H




#define LED_NUM_END         0xFFFE    ///不循环执行
#define LED_NUM_ANG         0xFFFF	  ///循环执行




typedef enum
{
	LED1,   //0   LED灯
	BUZ,	//1	  蜂鸣器
	LED_NUM,
}LED_TYPEDEF;

typedef enum
{
	LED_CLOSE,
	LED_LIGHT,
	LED_LIGHT_100MS,
	LED_TURN1,
	LED_TURN2,
}LED_EFFECT_TYPEDEF;

void Led_init(void);
void LED_proc(void);
void LedMsgInput(unsigned char type,LED_EFFECT_TYPEDEF cmd,unsigned char clr);
void GPIOC_14_Init(void);
#endif /* __HAL_LED_H */

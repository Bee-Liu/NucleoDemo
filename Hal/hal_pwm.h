#ifndef __HAL_PWM_H
#define __HAL_PWM_H

#include "stm32f4xx_hal.h"


void HAL_PWM_Init(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);
void TIM_SetPluse(uint16_t pluse);


#endif

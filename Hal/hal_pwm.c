#include "hal_pwm.h"



void HAL_PWM_Init(void)
{

    TIM_HandleTypeDef TIM_Handle;
    TIM_OC_InitTypeDef TIM_OC_Handle;

    TIM_Handle.Channel = HAL_TIM_ACTIVE_CHANNEL_1; //NOTES:通道1
    TIM_Handle.Instance = TIM2;                    //NOTES:选择定时器4
    TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //NOTES:时钟1分频
    TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;//NOTES:向上计数
    TIM_Handle.Init.Period = 999;                //NOTES:arr寄存器重装载值
    TIM_Handle.Init.Prescaler = 159;           //NOTES:预分频系数
    //HAL_TIM_Base_Init(&TIM_Handle);
    HAL_TIM_PWM_Init(&TIM_Handle);                  //NOTES:初始化定时器pwm


    TIM_OC_Handle.OCMode = TIM_OCMODE_PWM1;
    TIM_OC_Handle.OCPolarity = TIM_OCPOLARITY_HIGH; //NOTES:
    TIM_OC_Handle.Pulse = 500;

    HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &TIM_OC_Handle, TIM_CHANNEL_1);
    HAL_TIM_MspPostInit(&TIM_Handle);

    HAL_TIM_PWM_Start(&TIM_Handle, TIM_CHANNEL_1);
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    if (timHandle->Instance == TIM2)
    {

        //    __HAL_RCC_TIM2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM2 GPIO Configuration
        PA5     ------> TIM2_CH1
        */
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

}
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
}
// void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim)
// {
//     if (htim->Instance == TIM2)
//     {
//         __HAL_RCC_TIM2_CLK_ENABLE();
//     }
//     // if(htim->Instance == TIM4)
//     // {
//     //     __HAL_RCC_TIM4_CLK_ENABLE();
// 	//     HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);
// 	//     HAL_NVIC_EnableIRQ(TIM4_IRQn);
//     // }
// }
void TIM_SetPluse(uint16_t pluse)
{
    TIM2->CCR1 = pluse;
}


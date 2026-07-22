/**
 * @file Buzzer.c
 * @brief TIM2 PWM을 이용한 상태 알림용 부저 제어.
 */
#include "Buzzer.h"
#include "tim.h"
#include "cmsis_os.h"

extern TIM_HandleTypeDef htim2;

void Buzzer_Beep(int count, int delay_ms, uint16_t freq_arr)
{
    for (int i = 0; i < count; i++) {
        /* 듀티비 50%의 PWM을 생성하며 ARR 값으로 음 높이를 조절한다. */
        __HAL_TIM_SET_AUTORELOAD(&htim2, freq_arr);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, freq_arr / 2);
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
        osDelay(delay_ms);
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
        osDelay(delay_ms);
    }
}

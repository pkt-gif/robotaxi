#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "main.h"

/**
 * @brief 지정한 횟수만큼 PWM 부저음을 출력한다.
 * @param count 반복 횟수.
 * @param delay_ms 음 출력 및 음소거 구간의 길이(ms).
 * @param freq_arr TIM2 Auto-Reload 값(음 높이 결정).
 */
void Buzzer_Beep(int count, int delay_ms, uint16_t freq_arr);

#endif /* INC_BUZZER_H_ */

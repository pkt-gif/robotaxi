#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "main.h"

/**
 * @brief HC-SR04에 Trigger 펄스를 출력하고 Echo 폭을 거리(cm)로 변환한다.
 * @return 측정 거리(cm). 타임아웃 발생 시 0.
 */
uint16_t HCSR04_Read(GPIO_TypeDef *trigPort, uint16_t trigPin,
                     GPIO_TypeDef *echoPort, uint16_t echoPin);

#endif /* INC_ULTRASONIC_H_ */

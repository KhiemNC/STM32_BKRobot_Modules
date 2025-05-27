#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "stm32f1xx_hal.h"

void BUZZER_Init(TIM_HandleTypeDef *htim, uint32_t channel);
void BUZZER_SetSound(uint32_t freq, uint8_t duty);
void BUZZER_Play(void);
void BUZZER_Stop(void);

#endif /* INC_BUZZER_H_ */

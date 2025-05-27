#ifndef INC_DBG_LED_H_
#define INC_DBG_LED_H_

#include "stm32f1xx_hal.h"

void DBG_LED_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void DBG_LED_On();
void DBG_LED_Off();
void DBG_LED_Toggle();

#endif /* INC_DBG_LED_H_ */

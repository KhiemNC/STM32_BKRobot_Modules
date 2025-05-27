#ifndef INC_LED7_H_
#define INC_LED7_H_

#include "stm32f1xx_hal.h"

void LED7_Init(
		SPI_HandleTypeDef *hspi,
		GPIO_TypeDef *LATCH_GPIOx, uint16_t LATCH_GPIO_Pin,
		GPIO_TypeDef *EN1_GPIOx, uint16_t EN1_GPIO_Pin,
		GPIO_TypeDef *EN2_GPIOx, uint16_t EN2_GPIO_Pin);
void LED7_DisplayDigit(uint8_t digit, uint8_t position, uint8_t dot);
void LED7_DisplayAny(uint8_t data, uint8_t position);
void LED7_Clear();

#endif /* INC_LED7_H_ */

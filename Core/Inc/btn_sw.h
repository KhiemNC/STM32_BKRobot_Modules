#ifndef INC_BTN_SW_H_
#define INC_BTN_SW_H_

#include "stm32f1xx_hal.h"

uint8_t BTN_SW_Init(
		SPI_HandleTypeDef *hspi,
		GPIO_TypeDef *INP_LOAD_GPIOx, uint16_t INP_LOAD_GPIO_Pin);
uint8_t BTN_SW_ReadButton(uint8_t index);
uint8_t BTN_SW_GetStateSwitch(uint8_t index);


#endif /* INC_BTN_SW_H_ */

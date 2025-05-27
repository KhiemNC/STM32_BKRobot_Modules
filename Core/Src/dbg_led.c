#include "dbg_led.h"

#define LED_ON 0
#define LED_OFF 1

GPIO_TypeDef *DBG_LED_GPIOx;
uint16_t DBG_LED_GPIO_Pin;

void DBG_LED_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	DBG_LED_GPIOx = GPIOx;
	DBG_LED_GPIO_Pin = GPIO_Pin;
}

void DBG_LED_On()
{
	HAL_GPIO_WritePin(DBG_LED_GPIOx, DBG_LED_GPIO_Pin, LED_ON);
}

void DBG_LED_Off()
{
	HAL_GPIO_WritePin(DBG_LED_GPIOx, DBG_LED_GPIO_Pin, LED_OFF);
}

void DBG_LED_Toggle()
{
	HAL_GPIO_TogglePin(DBG_LED_GPIOx, DBG_LED_GPIO_Pin);
}

#include "btn_sw.h"

#define ENABLE 0
#define DISABLE 1

#define SCAN_TIMEOUT 10

static SPI_HandleTypeDef *BTN_SW_hspi = NULL;
static GPIO_TypeDef *BTN_SW_INP_LOAD_GPIOx = NULL;
static uint16_t BTN_SW_INP_LOAD_GPIO_Pin = 0;

// D0 D1 D2 D3 D4 D5 D6 D7
// B1 B2 B3 B4 S1 S2 S3 S4
static uint8_t scan_data = 0b00000000;

uint8_t BTN_SW_Init(
		SPI_HandleTypeDef *hspi,
		GPIO_TypeDef *INP_LOAD_GPIOx, uint16_t INP_LOAD_GPIO_Pin)
{
	BTN_SW_hspi = hspi;
	BTN_SW_INP_LOAD_GPIOx = INP_LOAD_GPIOx;
	BTN_SW_INP_LOAD_GPIO_Pin = INP_LOAD_GPIO_Pin;

	HAL_GPIO_WritePin(BTN_SW_INP_LOAD_GPIOx, BTN_SW_INP_LOAD_GPIO_Pin, DISABLE);

	return 0;
}

uint8_t BTN_SW_Scan()
{
	HAL_GPIO_WritePin(BTN_SW_INP_LOAD_GPIOx, BTN_SW_INP_LOAD_GPIO_Pin, ENABLE);
	HAL_GPIO_WritePin(BTN_SW_INP_LOAD_GPIOx, BTN_SW_INP_LOAD_GPIO_Pin, DISABLE);
	HAL_SPI_Receive(BTN_SW_hspi, &scan_data, 1, SCAN_TIMEOUT);
	return 0;
}

uint8_t BTN_SW_ReadButton(uint8_t index)
{
    if (index < 1 || index > 4) return 0xFF; // Invalid button index (valid: 1–4)

    BTN_SW_Scan();
    uint8_t bit_pos = index - 1;  // B1→bit0, B2→bit1, etc.
    return (scan_data >> bit_pos) & 0x01;
}

uint8_t BTN_SW_GetStateSwitch(uint8_t index)
{
    if (index < 1 || index > 4) return 0xFF; // Invalid switch index (valid: 1–4)

    BTN_SW_Scan();
    uint8_t bit_pos = index + 3;  // S1→bit4, S2→bit5, etc.
    return (scan_data >> bit_pos) & 0x01;
}

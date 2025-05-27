#include "led7.h"

#define ENABLE 0
#define DISABLE 1

#define LED_ON 0
#define LED_OFF 1

// Bit order: A B C D E F G DP
static const uint8_t led7_digit_table[10] = {
	0b00000011, // 0 > reverse of 0b11000000
	0b10011111, // 1 > reverse of 0b11111001
	0b00100101, // 2 > reverse of 0b10100100
	0b00001101, // 3 > reverse of 0b10110000
	0b10011001, // 4 > reverse of 0b10011001
	0b01001001, // 5 > reverse of 0b10010010
	0b01000001, // 6 > reverse of 0b10000010
	0b00011111, // 7 > reverse of 0b11111000
	0b00000001, // 8 > reverse of 0b10000000
	0b00001001  // 9 > reverse of 0b10010000
};

static SPI_HandleTypeDef *LED7_hspi = NULL;
static GPIO_TypeDef *LED7_LATCH_GPIOx = NULL;
static uint16_t LED7_LATCH_GPIO_Pin = 0;
static GPIO_TypeDef *LED7_EN1_GPIOx = NULL;
static uint16_t LED7_EN1_GPIO_Pin = 0;
static GPIO_TypeDef *LED7_EN2_GPIOx = NULL;
static uint16_t LED7_EN2_GPIO_Pin = 0;
static GPIO_TypeDef *LED8_EN_GPIOx = NULL;
static uint16_t LED8_EN_GPIO_Pin = 0;

void LED7_Init(
		SPI_HandleTypeDef *hspi,
		GPIO_TypeDef *LATCH_GPIOx, uint16_t LATCH_GPIO_Pin,
		GPIO_TypeDef *EN1_GPIOx, uint16_t EN1_GPIO_Pin,
		GPIO_TypeDef *EN2_GPIOx, uint16_t EN2_GPIO_Pin,
		GPIO_TypeDef *LED_EN_GPIOx, uint16_t LED_EN_GPIO_Pin)
{
	LED7_hspi = hspi;
	LED7_LATCH_GPIOx = LATCH_GPIOx;
	LED7_LATCH_GPIO_Pin = LATCH_GPIO_Pin;
	LED7_EN1_GPIOx = EN1_GPIOx;
	LED7_EN1_GPIO_Pin = EN1_GPIO_Pin;
	LED7_EN2_GPIOx = EN2_GPIOx;
	LED7_EN2_GPIO_Pin = EN2_GPIO_Pin;
	LED8_EN_GPIOx = LED_EN_GPIOx;
	LED8_EN_GPIO_Pin = LED_EN_GPIO_Pin;
}

void LED7_DisplayDigit(uint8_t digit, uint8_t position, uint8_t dot, uint8_t show_8leds)
{
	if (digit > 9 || position < 1 || position > 2) return;
	if (dot != 0) dot = 1;

	// Get segment pattern for digit
	uint8_t seg = led7_digit_table[digit];

	// Add dot if requested (active low)
	seg = seg - dot;

	// Enable the correct digit (active low)
	if (position == 1) {
		HAL_GPIO_WritePin(LED7_EN1_GPIOx, LED7_EN1_GPIO_Pin, ENABLE); // EN1 ON
		HAL_GPIO_WritePin(LED7_EN2_GPIOx, LED7_EN2_GPIO_Pin, DISABLE);   // EN2 OFF
	} else {
		HAL_GPIO_WritePin(LED7_EN1_GPIOx, LED7_EN1_GPIO_Pin, DISABLE);   // EN1 OFF
		HAL_GPIO_WritePin(LED7_EN2_GPIOx, LED7_EN2_GPIO_Pin, ENABLE); // EN2 ON
	}

	if (show_8leds == 0) HAL_GPIO_WritePin(LED8_EN_GPIOx, LED8_EN_GPIO_Pin, DISABLE);
	else HAL_GPIO_WritePin(LED8_EN_GPIOx, LED8_EN_GPIO_Pin, ENABLE);

	// Send segment data via SPI
	HAL_SPI_Transmit(LED7_hspi, &seg, 1, 100);

	// Latch the data
	HAL_GPIO_WritePin(LED7_LATCH_GPIOx, LED7_LATCH_GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED7_LATCH_GPIOx, LED7_LATCH_GPIO_Pin, GPIO_PIN_RESET);
}

void LED7_DisplayAny(uint8_t data, uint8_t show_digit1, uint8_t show_digit2, uint8_t show_8leds)
{
	if (show_digit1 == 0) HAL_GPIO_WritePin(LED7_EN1_GPIOx, LED7_EN1_GPIO_Pin, DISABLE);
	else HAL_GPIO_WritePin(LED7_EN1_GPIOx, LED7_EN1_GPIO_Pin, ENABLE);

	if (show_digit2 == 0) HAL_GPIO_WritePin(LED7_EN2_GPIOx, LED7_EN2_GPIO_Pin, DISABLE);
	else HAL_GPIO_WritePin(LED7_EN2_GPIOx, LED7_EN2_GPIO_Pin, ENABLE);

	if (show_8leds == 0) HAL_GPIO_WritePin(LED8_EN_GPIOx, LED8_EN_GPIO_Pin, DISABLE);
	else HAL_GPIO_WritePin(LED8_EN_GPIOx, LED8_EN_GPIO_Pin, ENABLE);

	// Send segment data via SPI
	HAL_SPI_Transmit(LED7_hspi, &data, 1, 100);

	// Latch the data
	HAL_GPIO_WritePin(LED7_LATCH_GPIOx, LED7_LATCH_GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED7_LATCH_GPIOx, LED7_LATCH_GPIO_Pin, GPIO_PIN_RESET);
}

void LED7_Clear()
{
	uint8_t seg = 0xFF;

	HAL_GPIO_WritePin(LED7_EN1_GPIOx, LED7_EN1_GPIO_Pin, DISABLE);
	HAL_GPIO_WritePin(LED7_EN2_GPIOx, LED7_EN2_GPIO_Pin, DISABLE);

	HAL_SPI_Transmit(LED7_hspi, &seg, 1, 100);
	HAL_GPIO_WritePin(LED7_LATCH_GPIOx, LED7_LATCH_GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED7_LATCH_GPIOx, LED7_LATCH_GPIO_Pin, GPIO_PIN_RESET);
}


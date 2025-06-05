#include "buzzer.h"

static TIM_HandleTypeDef *buzzer_htim = NULL;
static uint32_t buzzer_channel = 0;

void BUZZER_Init(TIM_HandleTypeDef *htim, uint32_t channel)
{
	buzzer_htim = htim;
	buzzer_channel = channel;
}

/**
 * @brief  Set the buzzer sound frequency and volume (duty cycle).
 * @param  freq: Desired frequency in Hz (e.g., 440 for A4 note).
 * @param  duty: Duty cycle in percent (1-99), controls volume.
 * @note   This function configures the timer's prescaler and period to generate
 *         the requested frequency, and sets the PWM duty cycle for volume.
 */
void BUZZER_SetSound(uint32_t freq, uint8_t duty)
{
    if (!buzzer_htim) return;

    // Get the timer clock frequency (APB1 timers are clocked at 2x PCLK1 on STM32F1)
    uint32_t timer_clk = HAL_RCC_GetPCLK1Freq() * 2;

    // Set prescaler so timer runs at 1 MHz (1 tick = 1 us)
    uint32_t prescaler = (timer_clk / 1000000) - 1;

    // Calculate the auto-reload value for the desired frequency
    // period = (timer_freq / freq) - 1 = (1,000,000 / freq) - 1
    uint32_t period = (1000000 / freq) - 1;

    // Apply prescaler and period to timer
    buzzer_htim->Instance->PSC = prescaler;
    buzzer_htim->Instance->ARR = period;

    // Set the compare value for the desired duty cycle (volume)
    // CCR1 = (period + 1) * duty / 100
    buzzer_htim->Instance->CCR1 = ((period + 1) * duty) / 100;
}

void BUZZER_Play(void) {
	if (!buzzer_htim) return;
	HAL_TIM_PWM_Start(buzzer_htim, buzzer_channel);
}

void BUZZER_Stop(void) {
	if (!buzzer_htim) return;
	HAL_TIM_PWM_Stop(buzzer_htim, buzzer_channel);
}

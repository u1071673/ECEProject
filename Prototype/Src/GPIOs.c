#include "GPIOs.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void GPIOx_init(GPIO_TypeDef *gpiox_base, uint16_t gpio_pins, uint32_t mode, uint32_t pull, uint32_t speed);

void GPIO_PINs_init(void) {
	GPIOx_init(ROLL_MOTORS_GPIO_BASE, ROLL_DIR_PIN|ROLL_STEP_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	GPIOx_init(PITCH_MOTORS_GPIO_BASE, PITCH_DIR_PIN|PITCH_STEP_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
}

bool A_ENABLED = false;
bool B_ENABLED = false;
bool C_ENABLED = false;
bool D_ENABLED = false;
bool E_ENABLED = false;
bool F_ENABLED = false;

// Initialises any GPIO between A - F pins as specified by the parameters.
void GPIOx_init(GPIO_TypeDef *gpiox_base, uint16_t gpio_pins, uint32_t mode, uint32_t pull, uint32_t speed) {
	GPIO_InitTypeDef GPIO_InitStruct;
  	/* GPIO Ports Clock Enable */
	if(!A_ENABLED && gpiox_base == GPIOA) {
		__HAL_RCC_GPIOA_CLK_ENABLE();
		A_ENABLED = true;
	}
	if(!B_ENABLED && gpiox_base == GPIOB) {
		__HAL_RCC_GPIOB_CLK_ENABLE();
		B_ENABLED = true;
	}
	if(!C_ENABLED && gpiox_base == GPIOC) {
		__HAL_RCC_GPIOC_CLK_ENABLE();
		C_ENABLED = true;
	}
	if(!D_ENABLED && gpiox_base ==  GPIOD) {
		__HAL_RCC_GPIOD_CLK_ENABLE();
		D_ENABLED = true;
	}
	if(!E_ENABLED && gpiox_base == GPIOE) {
		__HAL_RCC_GPIOE_CLK_ENABLE();
		E_ENABLED = true;
	}
	if(!F_ENABLED && gpiox_base == GPIOF) {
		__HAL_RCC_GPIOF_CLK_ENABLE();
		F_ENABLED = true;
	}
	GPIO_InitStruct.Pin = gpio_pins;
	GPIO_InitStruct.Mode = mode;
	GPIO_InitStruct.Pull = pull;
	GPIO_InitStruct.Speed = speed;
	HAL_GPIO_Init(gpiox_base, &GPIO_InitStruct);
	/* Default to zero output */
	HAL_GPIO_WritePin(gpiox_base, gpio_pins, GPIO_PIN_RESET);
}

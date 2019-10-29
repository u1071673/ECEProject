#include "GPIOs.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void GPIO_PINs_init(void) {
	printf("Initialization GPIOs...\n");
	GPIOx_init(MOTORS_GPIO_BASE, ROLL_DIR_PIN|ROLL_STEP_PIN|PITCH_DIR_PIN|PITCH_STEP_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	printf("GPIO Initialization Complete!\n");

}

// Initialises any GPIO between A - F pins as specified by the parameters.
void GPIOx_init(GPIO_TypeDef *gpiox_base, uint16_t gpio_pins, uint32_t mode, uint32_t pull, uint32_t speed) {
	GPIO_InitTypeDef GPIO_InitStruct;
  	/* GPIO Ports Clock Enable */
	if(gpiox_base == GPIOA)
		__HAL_RCC_GPIOA_CLK_ENABLE();
	if(gpiox_base == GPIOB)
		__HAL_RCC_GPIOB_CLK_ENABLE();
	if(gpiox_base == GPIOC)
		__HAL_RCC_GPIOC_CLK_ENABLE();
	if(gpiox_base ==  GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();
	if(gpiox_base == GPIOE)
		__HAL_RCC_GPIOE_CLK_ENABLE();
	if(gpiox_base == GPIOF)
		__HAL_RCC_GPIOF_CLK_ENABLE();
	GPIO_InitStruct.Pin = gpio_pins;
	GPIO_InitStruct.Mode = mode;
	GPIO_InitStruct.Pull = pull;
	GPIO_InitStruct.Speed = speed;
	HAL_GPIO_Init(gpiox_base, &GPIO_InitStruct);
	/* Default to zero output */
	HAL_GPIO_WritePin(gpiox_base, gpio_pins, GPIO_PIN_RESET);
}

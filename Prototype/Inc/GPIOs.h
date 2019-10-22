#ifndef __GPIOS_H
#define __GPIOS_H

#include "stm32f0xx_hal.h"

#define MOTORS_GPIO_BASE GPIOB
#define ROLL_DIR_PIN GPIO_PIN_2
#define ROLL_STEP_PIN GPIO_PIN_3
#define PITCH_DIR_PIN GPIO_PIN_4
#define PITCH_STEP_PIN GPIO_PIN_5

// Initialize PB5 and PB6 for the ROLL motor's direction and step Pins.
// Initialize PB7 and PB8 for the PITCH motor's direction and step Pins.
void GPIO_PINs_init(void);

void GPIOx_init(GPIO_TypeDef *gpiox_base, uint16_t gpio_pins, uint32_t mode, uint32_t pull, uint32_t speed);

#endif /* __GPIOS_H */

#ifndef __GPIOS_H
#define __GPIOS_H

#include "stm32f0xx_hal.h"

#define ROLL_MOTORS_GPIO_BASE GPIOB
#define ROLL_DIR_PIN GPIO_PIN_2
#define ROLL_STEP_PIN GPIO_PIN_3
#define PITCH_MOTORS_GPIO_BASE GPIOC
#define PITCH_DIR_PIN GPIO_PIN_10
#define PITCH_STEP_PIN GPIO_PIN_12

// Initialize PB5 and PB6 for the ROLL motor's direction and step Pins.
// Initialize PB7 and PB8 for the PITCH motor's direction and step Pins.
void GPIO_PINs_init(void);


#endif /* __GPIOS_H */

#include "LEDs.h"
#include "stm32f0xx_hal.h"

void LEDs_init() {
	printf("Initializing LEDs...\n");
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	// Initialize all LEDs to general purpose output mode 
	GPIOC->MODER |= (1 << 12) | (1 << 14) | (1 << 16) | (1 << 18);
	
	// all LEDs to push-pull
	GPIOC->OTYPER &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
	
	// all LEDs to low speed
	GPIOC->OSPEEDR &= ~((1 << 12) | (1 << 14) | (1 << 16) | (1 << 18));
	
	// all LEDs to no pull up and no pull down
	GPIOC->PUPDR &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15) | (1 << 16) | (1 << 17) | (1 << 18) | (1 << 19));
	printf("LED Initialization Complete!\n");
}


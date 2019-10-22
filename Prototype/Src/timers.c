#include "timers.h"
#include "stm32f0xx_hal.h"

// Initializes TIM6 to trigger an interrupt every 37.5 ms
void TIM6_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->PSC = 0x0001;
	TIM6->ARR = 0x927C;
	TIM6->DIER |= TIM_DIER_UIE;						 // Enable update event interrupt
	TIM6->CR1 |= TIM_CR1_CEN;							 // Enable Timer
	NVIC_EnableIRQ(TIM6_DAC_IRQn);					// Enable interrupt in NVIC
	NVIC_SetPriority(TIM6_DAC_IRQn,2);
}

// This method is called once every 37.5 ms.
void TIM6_DAC_IRQHandler(void) {
	// TODO: find out if we need this.
	TIM6->SR &= ~TIM_SR_UIF;				// Acknowledge the interrupt
}

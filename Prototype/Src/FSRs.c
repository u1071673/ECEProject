#include "FSRs.h"
#include "stm32f0xx_hal.h"
#include "GPIOs.h"
#include <stdbool.h>

void FSRs_init() {

	// SETTING UP THE PINS
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	// Configure pin PA0, PA1, PA4 PA5 as analog
	GPIOA->MODER &= ~(GPIO_MODER_MODER4);// | GPIO_MODER_MODER1 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5);
	GPIOA->MODER |=	GPIO_MODER_MODER4;// | GPIO_MODER_MODER1 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5;

	// SETTING UP ADC
	// Enable ADC clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	// Reset ADC configuration
	ADC1->CR 	= 0;
	ADC1->CFGR1  = 0;
	ADC1->CFGR2  = 0;
	ADC1->CHSELR = 0;
	// Enable continuous conversion mode
	ADC1->CFGR1 |= ADC_CFGR1_CONT;
	// 12-bit resolution
	ADC1->CFGR1 &= ~ ADC_CFGR1_RES;
	// Select PCLK/2 as ADC clock
	ADC1->CFGR2 |= ADC_CFGR2_CKMODE_1;
	// Set sampling time to 28.5 ADC clock cycles
	ADC1->SMPR = 0x03;
	// Select channel 0, 1, 4, 5
	ADC1->CHSELR |= ADC_CHSELR_CHSEL4;// | ADC_CHSELR_CHSEL1 | ADC_CHSELR_CHSEL4 | ADC_CHSELR_CHSEL5;
	
	// SETTING UP DMA
	// Start DMA clock
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	// Peripheral is ADC1 DR
	DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
	// Memory is adc_dma_buffer
	DMA1_Channel1->CMAR = (uint32_t)adc_dma_buffer;
	// Set Memory Buffer size
	DMA1_Channel1->CNDTR = 1;
	// Reset DMA1 Channel 1 configuration
	DMA1_Channel1->CCR = 0x00000000;
	// Configure DMA1 Channel 1
	// Peripheral -> Memory
	// Peripheral is 16-bit, no increment
	// Memory is 16-bit, increment
	// Circular mode
	DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_MINC | DMA_CCR_CIRC;
	// Enable DMA1 Channel 1
	DMA1_Channel1->CCR |= DMA_CCR_EN;
	
	/* (1) Ensure that ADEN = 0 */
  /* (2) Clear ADEN by setting ADDIS*/
  /* (3) Clear DMAEN */
  /* (4) Launch the calibration by setting ADCAL */
  /* (5) Wait until ADCAL=0 */
	if ((ADC1->CR & ADC_CR_ADEN) != 0) /* (1) */
	{
	    ADC1->CR |= ADC_CR_ADDIS; /* (2) */
	}
	while ((ADC1->CR & ADC_CR_ADEN) != 0)
	{
	/* For robust implementation, add here time-out management */
	}
	  ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN; /* (3) */
	  ADC1->CR |= ADC_CR_ADCAL; /* (4) */
	    while ((ADC1->CR & ADC_CR_ADCAL) != 0) /* (5) */
	   {
	    /* For robust implementation, add here time-out management */
	   }
	
  // START ADC
	// Enable ADC DMA Request in circular mode
	ADC1->CFGR1 |= ADC_CFGR1_DMACFG | ADC_CFGR1_DMAEN;
	// Enable ADC
	ADC1->CR |= ADC_CR_ADEN;
	
	// Start conversion
	ADC1->CR |= ADC_CR_ADSTART;
}



/**
  ******************************************************************************
  Written by : Usman Ali Butt
	Dated      : 8 July 2018
	Property of: www.microcontroller-projecct.ccom
  ******************************************************************************
  */
#include "main.h"
#include "stm32f0xx_hal.h"
#include <stdio.h>
#include <stdlib.h>

// Custom Macros
#define BAUD_RATE 9600
#define DEBUG_MODE 'a'
#define FG_MODE 'b'
#define ROLL_CLOCKWISE 'w'
#define ROLL_COUNTER_CLOCKWISE 's'
#define PITCH_CLOCKWISE 'd'
#define PITCH_COUNTER_CLOCKWISE 'a'
#define RESET 'r'
#define ENABLE_MOTORS 'e'
#define ROLL_INCREASE '+'
#define ROLL_DECREASE '-'
#define PITCH_INCREASE '>'
#define PITCH_DECREASE '<'
#define QUIT 'q'
#define SPACE ' '
#define DEL 0x7F
#define DELAY_UNIT 10
#define MOTORS_GPIO_BASE GPIOB
#define ROLL_DIR_PIN GPIO_PIN_5
#define ROLL_STEP_PIN GPIO_PIN_6
#define PITCH_DIR_PIN GPIO_PIN_7
#define PITCH_STEP_PIN GPIO_PIN_8
#define MAX_ROLL_STEPS 200
#define MAX_PITCH_STEPS 200
#define TRUE 1
#define FALSE 0
#define BOOL uint8_t
#define ON_DELAY 10
#define OFF_DELAY 10
#define MOTOR_TH 10

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void GPIOx_init(GPIO_TypeDef *gpiox_base, uint16_t gpio_pins, uint32_t mode, uint32_t pull, uint32_t speed);
void USART3_init(void);
void TIM6_init(void);
void I2C2_init(void);
void poll_accel_rate(void);
void transmit_string(char *string);
void transmit_char(char c);
char receive_char(void);
void debug_prompt(void);
void fg_prompt(void);
void debug_menu(void);
void fg_menu(void);
int degrees_to_steps(int degrees, int MAX_STEPS);
void main_prompt(void);
void main_menu(void);
void set_direction_pins(GPIO_TypeDef *gpiox_base, uint16_t dir_pins, BOOL set_pin_high);
void set_step_pin(GPIO_TypeDef *gpiox_base, uint16_t step_pin, BOOL is_roll);
void set_step_pin_manually(GPIO_TypeDef *gpiox_base, uint16_t step_pin, BOOL is_roll, BOOL set_pin_high);
void LEDs_init(void);
void Start_GYRO_TRX(BOOL write, unsigned int dataSize);
void update_motors(void);

/* STATIC GLOBAL VARIABLES */
volatile static char received_char = 0;
volatile static BOOL USART_new_data = FALSE;

volatile BOOL roll_enabled = FALSE;
volatile BOOL pitch_enabled = FALSE;
volatile BOOL roll_toggle = FALSE;
volatile BOOL pitch_toggle = FALSE;
volatile BOOL seat_needs_centering = TRUE;
volatile int16_t roll_delay = 10;
volatile int16_t pitch_delay = 10;
volatile uint8_t xlow, xhigh = 0;
volatile uint8_t ylow, yhigh = 0;
volatile int16_t actual_roll_speed, actual_pitch_speed = 0; 
volatile int target_roll_steps = 0;
volatile int target_pitch_steps = 0;
volatile int actual_roll_steps = 0;
volatile int actual_pitch_steps = 0;
volatile BOOL roll_clockwise = TRUE;
volatile BOOL pitch_clockwise = FALSE;

int main(void) {
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
	
	// Initialize PB5 and PB6 for the ROLL motor's direction and step Pins.
	// Initialize PB7 and PB8 for the PITCH motor's direction and step Pins.
	GPIOx_init(MOTORS_GPIO_BASE, ROLL_DIR_PIN|ROLL_STEP_PIN|PITCH_DIR_PIN|PITCH_STEP_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	
	//initialize LEDs
	LEDs_init();
	
	// Initialize I2C2
	I2C2_init();
	
	// Initialize PC4 and PC5 for USART3_TX (RXD) and USART3_RX (TXD).
	USART3_init();

	// Initialize TIM6 to call interrupt handler method every 
	TIM6_init();
	

  /* Infinite loop */
	while (TRUE)
	{
		main_menu();
	}
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


/* USER CODE BEGIN 4 */
void main_prompt(void) {
	transmit_string("\n\r\tSELECT MODE");
	transmit_string("\n\r[a] \t\tDEBUG Mode");
	transmit_string("\n\r[b] \t\tFG Mode");
	transmit_string("\n\r");
	USART_new_data = FALSE;
	received_char = 0;
	seat_needs_centering = TRUE;
}


void main_menu(void) {
	main_prompt();
	BOOL done = FALSE;
	while (!done) {
		while (!USART_new_data); // Wait for new data.
		target_roll_steps = 0;
		target_pitch_steps = 0;
		switch(received_char) {
		case DEBUG_MODE:
			transmit_char(received_char);
			transmit_string("\tDebug mode selected");
			debug_menu();
			done = TRUE;
			break;
		case FG_MODE:
			transmit_char(received_char);
			transmit_string("\tFlight Gear mode selected");
			fg_menu();		
			done = TRUE;
			break;
		case '\n':
		case '\r':
		default:
			break;
		}
	}
}
void debug_prompt(void) {
	received_char = 0;
	USART_new_data = FALSE;
	target_roll_steps = 0;
	target_pitch_steps = 0;
	transmit_string("\n\r");
	transmit_string("[w]Roll clockwise\t[s]Roll counter-clockwise\t[-]Decrease roll\t[+]Increase roll;");
	transmit_string("\n\r");
	transmit_string("[d]Pitch clockwise\t[a]Pitch coutner-clockwise\t[<]Decrease pitch\t[>]Increase pitch");
	transmit_string("\n\r");
	transmit_string("[r]Reset\t\t[e]Enable Motors\t\t[Space]Stop\t\t[Enter]=Exit");
}

void fg_prompt(void) {
	received_char = 0;
	USART_new_data = FALSE;
	target_roll_steps = 0;
	target_pitch_steps = 0;
	transmit_string("\n\r");
	transmit_string("Reading Roll and Pitch (Press q to quit)");
	transmit_string("\n\r");
	transmit_string("Format: Roll,Pitch");
	transmit_string("\n\r");
}

void debug_menu() {
	debug_prompt();
	while(received_char != '\n' && received_char != '\r') {
		transmit_string("\n\r");
		while(!USART_new_data); // Wait for USART new data to arrive
		USART_new_data = FALSE;
		switch(received_char) {
			case ROLL_CLOCKWISE:
				transmit_string("\tRoll direction is now clockwise!");
				roll_clockwise = TRUE;
				set_direction_pins(MOTORS_GPIO_BASE, ROLL_DIR_PIN, roll_clockwise);
				break;
			case ROLL_COUNTER_CLOCKWISE:
				transmit_string("\tRoll direction is now counter-clockwise!");
				roll_clockwise = FALSE;
				set_direction_pins(MOTORS_GPIO_BASE, ROLL_DIR_PIN, roll_clockwise);
				break;
			case ROLL_DECREASE:
				transmit_string("\tRoll speed decreased by 1 unit.");
				roll_delay += DELAY_UNIT;
				break;
			case ROLL_INCREASE:
				if (roll_delay - DELAY_UNIT < 0) {
					transmit_string("\tMaximum roll speed achieved.");
					roll_delay = 0;
				} else {
					transmit_string("\tRoll speed increased by 1 unit.");
					roll_delay -= DELAY_UNIT;
				}
				break;
			case PITCH_CLOCKWISE:
				transmit_string("\tPitch direction is now clockwise!");
				pitch_clockwise = TRUE;
				set_direction_pins(MOTORS_GPIO_BASE, PITCH_DIR_PIN, pitch_clockwise);
				break;
			case PITCH_COUNTER_CLOCKWISE:
				transmit_string("\tPitch direction is now counter-clockwise!");
				pitch_clockwise = FALSE;
				set_direction_pins(MOTORS_GPIO_BASE, PITCH_DIR_PIN, pitch_clockwise);
				break;
			case PITCH_DECREASE:
				transmit_string("\tPitch speed decreased by 1 unit.");
				pitch_delay += DELAY_UNIT;
				break;
			case PITCH_INCREASE:
				if (pitch_delay - DELAY_UNIT < 0) {
					transmit_string("\tMaximum pitch speed achieved.");
					pitch_delay = 0;
				} else {
					transmit_string("\tPitch speed increased by 1 unit.");
					pitch_delay -= DELAY_UNIT;
				}
				break;
			case SPACE:
				transmit_string("\tStopping motors!");
				roll_enabled = FALSE;
				pitch_enabled = FALSE;
				break;
			case ENABLE_MOTORS:
				transmit_string("\tEnabling Motors!");
				roll_enabled = TRUE;
				pitch_enabled = TRUE;
				break;
			case RESET:
				transmit_string("\tReseting all!");
				roll_enabled = FALSE;
				pitch_enabled = FALSE;
				break;
			case '\n':
			case '\r':
				break;
			default:
				transmit_string("\tUnrecognized Character: ");
				transmit_char(received_char);
				break;
		}
	}
}


void fg_menu() {
	fg_prompt();
	char roll_buff[100];
	char pitch_buff[100];
	USART_new_data = FALSE;
	received_char = 0;
	
	while(received_char != QUIT) {
		while(received_char != '\r' && received_char != QUIT) {
			USART_new_data = FALSE;
			while(!USART_new_data); // Wait for USART new data to arrive
			//transmit_char(received_char);
		}
		//transmit_string("got CR!\r\n");
		while(received_char != '\n' && received_char != QUIT) {
			USART_new_data = FALSE;
			while(!USART_new_data); // Wait for USART new data to arrive
			//transmit_char(received_char);
		}
		//transmit_string("got newline!\r\n");
		int i = 0;
		while(received_char != ',') {
			USART_new_data = FALSE;
			while(!USART_new_data); // Wait for USART new data to arrive
			if(received_char != ',') {
				//transmit_char(received_char);
				roll_buff[i] = received_char;
				i++;
			} else {
				roll_buff[i] = 0;
			}
		}
		i = 0;
		while(received_char != '\r') {
			USART_new_data = FALSE;
			while(!USART_new_data); // Wait for USART new data to arrive
			if(received_char != '\r') {
				//transmit_char(received_char);
				pitch_buff[i] = received_char;
				i++;
			} else {
				pitch_buff[i] = 0;
			}	
		}
		
		//transmit_string("got CR!\r\n");
		char string_to_transmit[205];
		target_roll_steps = degrees_to_steps(atoi(roll_buff), MAX_ROLL_STEPS);
		target_pitch_steps = degrees_to_steps(atoi(pitch_buff), MAX_PITCH_STEPS);

		sprintf(string_to_transmit, "FG GOT: %d,%d\r\n", atoi(roll_buff), atoi(pitch_buff));

		transmit_string(string_to_transmit);
		
		USART_new_data = FALSE;
		while(!USART_new_data); // Wait for USART new data to arrive
		
		update_motors();
	}
	
}

void update_motors(void) {
	BOOL roll_update_needed = TRUE;
	BOOL pitch_update_needed = TRUE;
	
	// Set direction to target
	if(target_roll_steps < (actual_roll_steps - MOTOR_TH)) {
		roll_clockwise = FALSE;
		set_direction_pins(MOTORS_GPIO_BASE, ROLL_DIR_PIN, roll_clockwise);
	} else if (target_roll_steps > (actual_roll_steps + MOTOR_TH)) {
		roll_clockwise = TRUE;
		set_direction_pins(MOTORS_GPIO_BASE, ROLL_DIR_PIN, roll_clockwise);
	} else {
		roll_update_needed = FALSE;
	}
	if(target_pitch_steps < (actual_pitch_steps - MOTOR_TH)) {
		pitch_clockwise = FALSE;
		set_direction_pins(MOTORS_GPIO_BASE, PITCH_DIR_PIN, pitch_clockwise);
	} else if (target_pitch_steps > (actual_pitch_steps + MOTOR_TH)) {
		pitch_clockwise = TRUE;
		set_direction_pins(MOTORS_GPIO_BASE, PITCH_DIR_PIN, pitch_clockwise);
	} else {
		pitch_update_needed = FALSE;
	}
	
	// Step if not at target
	if(roll_update_needed) {
		set_step_pin_manually(MOTORS_GPIO_BASE, ROLL_STEP_PIN, TRUE, TRUE);
		set_step_pin_manually(MOTORS_GPIO_BASE, ROLL_STEP_PIN, TRUE, TRUE);
	}
	
	if(pitch_update_needed) {
		set_step_pin_manually(MOTORS_GPIO_BASE, ROLL_STEP_PIN, FALSE, TRUE);
		set_step_pin_manually(MOTORS_GPIO_BASE, ROLL_STEP_PIN, FALSE, TRUE);
	}
	HAL_Delay(ON_DELAY);
	
	if(roll_update_needed) {
		set_step_pin_manually(MOTORS_GPIO_BASE, ROLL_STEP_PIN, TRUE, FALSE);
		set_step_pin_manually(MOTORS_GPIO_BASE, ROLL_STEP_PIN, TRUE, FALSE);
	}
	
	if(pitch_update_needed) {
		set_step_pin_manually(MOTORS_GPIO_BASE, ROLL_STEP_PIN, FALSE, FALSE);
		set_step_pin_manually(MOTORS_GPIO_BASE, ROLL_STEP_PIN, FALSE, FALSE);
	}
	HAL_Delay(OFF_DELAY);
	
}


// Sets the direction pin according to according to the target direction. 
void set_step_pin(GPIO_TypeDef *gpiox_base, uint16_t step_pin, BOOL is_roll) {
		GPIO_PinState pin_state = GPIO_PIN_RESET;
		if(is_roll) {
			pin_state = roll_enabled ? (roll_toggle ? GPIO_PIN_SET : GPIO_PIN_RESET) : GPIO_PIN_RESET;
			if(roll_toggle) {
				roll_toggle = FALSE;
				actual_roll_steps = roll_clockwise ? ((actual_roll_steps + 1) % MAX_ROLL_STEPS) : ((actual_roll_steps - 1) % MAX_ROLL_STEPS);
			} else {
				roll_toggle = TRUE;
			}
		} else {
			pin_state = roll_enabled ? (pitch_toggle ? GPIO_PIN_SET : GPIO_PIN_RESET) : GPIO_PIN_RESET;
			if(pitch_toggle) {
				pitch_toggle = FALSE;
				actual_pitch_steps = pitch_clockwise ? ((actual_pitch_steps + 1) % MAX_PITCH_STEPS) : ((actual_pitch_steps - 1) % MAX_PITCH_STEPS);
			} else {
				pitch_toggle = TRUE;
			}
		}
		HAL_GPIO_WritePin(gpiox_base, step_pin, pin_state);
}

void set_step_pin_manually(GPIO_TypeDef *gpiox_base, uint16_t step_pin, BOOL is_roll, BOOL set_pin_high) {
	GPIO_PinState pin_state = set_pin_high ? GPIO_PIN_SET : GPIO_PIN_RESET;
		if(is_roll) {
			actual_roll_steps = roll_clockwise ? ((actual_roll_steps + 1) % MAX_ROLL_STEPS) : ((actual_roll_steps - 1) % MAX_ROLL_STEPS);
		} else {
			actual_pitch_steps = pitch_clockwise ? ((actual_pitch_steps + 1) % MAX_PITCH_STEPS) : ((actual_pitch_steps - 1) % MAX_PITCH_STEPS);
		}
		HAL_GPIO_WritePin(gpiox_base, step_pin, pin_state);
}

// Sets the direction pins according to according to the target direction. 
void set_direction_pins(GPIO_TypeDef *gpiox_base, uint16_t dir_pins, BOOL set_pin_high) {
		if(set_pin_high)
			HAL_GPIO_WritePin(gpiox_base, dir_pins, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(gpiox_base, dir_pins, GPIO_PIN_RESET);
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


void I2C2_init() {
		// Initialize clocks
	//RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	//  PB11 and PB13 to alternate function mode
	GPIOB->MODER |= (1 << 23) | (1 << 27);
	//PB11 and PB13 to open drain output type
	GPIOB->OTYPER |= (1 << 11) | (1 << 13);
	// I2C2_SDA as alternate function for PB11
	GPIOB->AFR[1] |= (1 << 12);
	// I2C2_SCL as alternate function for PB13
	GPIOB->AFR[1] |= (1 << 22) | (1 << 20);
	
	// PB14 and PC0 to output mode
	GPIOB->MODER |= (1 << 28);
	GPIOC->MODER |= (1 << 0);
	// PB14 and PC0 to push-pull output 
	GPIOB->OTYPER &= ~(1 << 14);
	GPIOC->OTYPER &= ~(1 << 0);
	//PB14 and PC0 set to high
	GPIOB->ODR |= (1 << 14);
	GPIOC->ODR |= (1 << 0);
	
	// PRESC
	I2C2->TIMINGR |= (1 << 28);
	// SCLL
	I2C2->TIMINGR |= (1 << 0) | (1 << 1) | (1 << 4);
	// SCLH
	I2C2->TIMINGR |= (0xF<<8);
	// SDADEL
	I2C2->TIMINGR |= (1 << 17);
	// SCLDEL
	I2C2->TIMINGR |= (1 << 22);

	// Enable I2C
	I2C2->CR1 |= (1 << 0);
	
		// start
	Start_GYRO_TRX(FALSE, 2);
	
	// Continue once the TXIS flag is set and the NACKF flag is not set
	while( !(I2C2->ISR & I2C_ISR_TXIS) || (I2C2->ISR & I2C_ISR_NACKF) ){}
		
	// address for gyro ccrtl_reg1
	I2C2->TXDR = 0x20;
	
	while( !(I2C2->ISR & I2C_ISR_TXIS) || (I2C2->ISR & I2C_ISR_NACKF) ){}
		
	// enable x and y axis and set PD to 1, everything else set to 0
	I2C2->TXDR = 0xB;

	// wait until TC register is set	
	while(!(I2C2->ISR & I2C_ISR_TC)) {}
	
	//stop
	I2C2->CR2 |= (1 << 14);

}


void LEDs_init() {
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		// Initialize all LEDs to general purpose output mode 
	GPIOC->MODER |= (1 << 12) | (1 << 14) | (1 << 16) | (1 << 18);
	
	// all LEDs to push-pull
	GPIOC->OTYPER &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
	
	// all LEDs to low speed
	GPIOC->OSPEEDR &= ~((1 << 12) | (1 << 14) | (1 << 16) | (1 << 18));
	
	// all LEDs to no pull up and no pull down
	GPIOC->PUPDR &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15) | (1 << 16) | (1 << 17) | (1 << 18) | (1 << 19));
}


// Initializes PC4 and PC5 for USART3_TX (RXD) and USART3_RX (TXD).
void USART3_init(void) {
	//RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	// Configuring PC4 and PC5 to alternate function mode
	GPIOC->MODER |= ((1<<9) | (1<<11));	
	// Configuring the RX and TX lines to alternate function mode
	// AF1 for PC4 (USART3_TX) and PC5 (USART3_RX) // Note: actual installation of cable is backword (PC4 is RX and PC5 is TX)
	GPIOC->AFR[0] |= (1<<20 | 1<<16);
	GPIOC->AFR[0] &= ~(0xE<<20 | 0xE<<16);
	// Set the baud rate for communication to be 115200 bits/seconds
	USART3->BRR = HAL_RCC_GetHCLKFreq()/BAUD_RATE;
	// Enabling the transmitter and reciever hardware.
	USART3->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;
	// Enabling the receive register non empty interupt
	NVIC_EnableIRQ(USART3_4_IRQn);
	NVIC_SetPriority(USART3_4_IRQn, 1);
}


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


// Transmits a char over USART3.
void transmit_char(char c) {
	// Waiting on the USART status flag to indicate the transmit register is empty.
	while(!(USART3->ISR & USART_ISR_TXE));
	USART3->TDR = c;
}


// Transmits every char in the string over USART3.
void transmit_string(char *string) {
	int i = 0;
	while(string[i]) {
		transmit_char(string[i]);
		i++;
	}
}


int steps_to_degrees(int steps, int MAX_STEPS) {
	return 360 * (steps/MAX_STEPS);
}


int degrees_to_steps(int degrees, int MAX_STEPS) {
	return MAX_STEPS * (degrees/360);
}


// Receives the char from the USART3 buffer.
char receive_char(void) {
	// Wait for RXNE=0 inside ISR
	while(!(USART3->ISR & USART_ISR_RXNE));
	return USART3->RDR;
}

// This is called every time a char is received on USART3_4
void USART3_4_IRQHandler(void) {
	received_char = receive_char();
	USART_new_data = TRUE;
	return;
}


// This method is called once every 37.5 ms.
void TIM6_DAC_IRQHandler(void) {
	//set_step_pin(MOTORS_GPIO_BASE, ROLL_STEP_PIN, TRUE);
	//HAL_Delay(roll_delay);

		poll_accel_rate();
		
		TIM6->SR &= ~TIM_SR_UIF;				// Acknowledge the interrupt

}

// READ THE GYRO DATA
void poll_accel_rate(void) {
		char pbuff[15];
		char rbuff[15];
	
		Start_GYRO_TRX(FALSE, 1);
			
		while( !(I2C2->ISR & I2C_ISR_TXIS) || (I2C2->ISR & I2C_ISR_NACKF) ){}
		// send address of x axis data
		I2C2->TXDR = 0xA8;
	
		while(!(I2C2->ISR & I2C_ISR_TC)) {}
		
		//restart to read x data
		Start_GYRO_TRX(TRUE, 2);
			
		while( !(I2C2->ISR & I2C_ISR_RXNE) || (I2C2->ISR & I2C_ISR_NACKF) ){}	
			
		xlow = (I2C2->RXDR & I2C_RXDR_RXDATA);
			
		while( !(I2C2->ISR & I2C_ISR_RXNE) || (I2C2->ISR & I2C_ISR_NACKF) ){}	
			
		while(!(I2C2->ISR & I2C_ISR_TC)) {}
			
		xhigh = (I2C2->RXDR & I2C_RXDR_RXDATA);
			
		actual_roll_speed = (xhigh << 8) | xlow;
			
		//restart to ask for y data
		Start_GYRO_TRX(FALSE, 1);
			
		// Continue once the TXIS flag is set and the NACKF flag is not set
		while( !(I2C2->ISR & I2C_ISR_TXIS) || (I2C2->ISR & I2C_ISR_NACKF) ){}

		// address for y data
		I2C2->TXDR = 0xAA;

		// wait until TC register is set	
		while(!(I2C2->ISR & I2C_ISR_TC)) {}
		
		//restart to read y data
		Start_GYRO_TRX(TRUE, 2);
			
		while( !(I2C2->ISR & I2C_ISR_RXNE) || (I2C2->ISR & I2C_ISR_NACKF) ){}	
			
		ylow = (I2C2->RXDR & I2C_RXDR_RXDATA);
			
		while( !(I2C2->ISR & I2C_ISR_RXNE) || (I2C2->ISR & I2C_ISR_NACKF) ){}	
			
		while(!(I2C2->ISR & I2C_ISR_TC)) {}
			
		yhigh = (I2C2->RXDR & I2C_RXDR_RXDATA);
			
		actual_pitch_speed = (yhigh << 8) | ylow;
			
		if( actual_roll_speed > 5000) {
			GPIOC->ODR |= (1 << 9);
			sprintf(rbuff, "%d", actual_roll_speed);
			transmit_string("\n\r\t");
			transmit_string(rbuff);
		}
		else
			GPIOC->ODR &= ~(1 << 9);
		
	  if ( actual_roll_speed < -5000) {
			GPIOC->ODR |= (1 << 8);
			sprintf(rbuff, "%d", actual_roll_speed);
			transmit_string("\n\r\t");
			transmit_string(rbuff);
		}
  	else
			GPIOC->ODR &= ~(1 << 8);
		
		if( actual_pitch_speed > 5000) {
			GPIOC->ODR |= (1 << 6);
			sprintf(pbuff, "%d", actual_pitch_speed);
			transmit_string("\n\r\t");
			transmit_string(pbuff);
		}
		else
			GPIOC->ODR &= ~(1 << 6);
		
		if ( actual_pitch_speed < -5000) {
			GPIOC->ODR |= (1 << 7);
			sprintf(pbuff, "%d", actual_pitch_speed);
			transmit_string("\n\r\t");
			transmit_string(pbuff);
		}	
		else
			GPIOC->ODR &= ~(1 << 7);

}


void Start_GYRO_TRX(BOOL read, unsigned int dataSize) {
	// clear nbytes and SADD
	I2C2->CR2 &= ~((0xFF << 16) | (0x3FF << 0));
	// SET to 2 Byte message and GYRO ADDRESS 
	I2C2->CR2 |= (dataSize << 16) | (0x6B << 1);
	
	if (read) 
		// RD_WRN to read
		I2C2->CR2 |= (1 << 10);
	else 
		// RD_WRN to write
		I2C2->CR2 &= ~(1 << 10);

	// START bit
	I2C2->CR2 |= (1 << 13);
}
  

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
#endif /* USE_FULL_ASSERT */



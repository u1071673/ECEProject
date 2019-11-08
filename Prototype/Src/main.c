
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
#include <stdbool.h>
#include "EventRecorder.h"	// Keil::ARM_Compiler (Needed For printf to work)
#include "game_parser.h"
#include "motors.h"
#include "putty.h"
#include "LEDs.h"
#include "USARTs.h"
#include "BNO055.h"

#define DEBUG_MODE 'a'
#define GAME_MODE 'b'
#define ROLL_CLOCKWISE 'd'
#define ROLL_COUNTER_CLOCKWISE 'a'
#define PITCH_CLOCKWISE 'w'
#define PITCH_COUNTER_CLOCKWISE 's'
#define GET_ORIENTATION 'o'
#define RESET 'r'
#define ENABLE '\r'
#define ROLL_INCREASE '+'
#define ROLL_DECREASE '-'
#define PITCH_INCREASE '>'
#define PITCH_DECREASE '<'
#define QUIT 'q'
#define SPACE ' '
#define DEL 0x7F
#define DELAY_UNIT 10
#define CW true
#define CCW false
	
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void chair_init(void);
void main_menu(void);
void game_menu(void);
void debug_menu(void);
void reset_values(void);

/* STATIC GLOBAL VARIABLES */

int main(void) {
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
  	
  	/* Configure the system clock */
	SystemClock_Config();

  	/* Initialize all configured peripherals */
	EventRecorderInitialize (EventRecordAll, 1);  // initialize and start Event Recorder (Needed For printf to work)

	chair_init();

	main_menu();
}

// Initialize all peripherals needed to have a functioning chair.
void chair_init(void) {
	printf("Initializing Chair...\n");
	
	// PB5 (Roll Dir), PB6 (Roll Step), PB7 (Pitch Dir), PB8 (Pitch step)
	motors_init();

	// PC6, PC7, PC8, PC9
	LEDs_init();

	// PC4 (USART3_TX), PC5 (USART_RX)
	game_parser_init();
	
	// PB6 (USART1_TX), PB7 (USART1_RX)
	//BNO055_init();
	
	printf("Chair Initialization Complete!\n");

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
void main_menu(void) {
	printf("Opening Main Menu\n");
	reset_values();
	char c = 0;
	putty_main_prompt();
	for(c = wait_for_USART3_data(); c != QUIT; c = wait_for_USART3_data()) {
		putty_putc(c); // Print character pressed for user feedback.
		switch(c) {
		case DEBUG_MODE:
			debug_menu();
			putty_main_prompt();
		break;
		case GAME_MODE:
			game_menu();
			putty_main_prompt();
		break;
		case '\n':
		case '\r':
		break;
		default:
			putty_unrecognized_prompt(c);
		break;
		}
	}
	putty_goodbye_prompt();
}

void game_menu() {
	printf("Enabling Game Mode\n");
	reset_values();
	putty_game_prompt();
	while(get_putty_cmd() != QUIT) {
		while(!recieved_putty_cmd()) {
//			game_data g_data = wait_for_game_data(QUIT);
//			int target_roll_steps = degrees_to_roll_steps(g_data.roll_deg);
//			int target_pitch_steps = degrees_to_pitch_steps(g_data.pitch_deg);

//			set_target_roll_steps(target_roll_steps);
//			set_target_pitch_steps(target_roll_steps);
//			
//			char string_to_transmit[40];
//			sprintf(string_to_transmit, "GAME GOT: %d,%d\r\n", g_data.roll_deg, g_data.pitch_deg);
//			
//			putty_print(string_to_transmit);

//			update_motors();
		}
	}
	putty_print("Leaving Game Mode...");
	putty_print("\n\r");
}

void debug_menu() {
	printf("Debug Mode enabled!\n");
	reset_values();
	putty_debug_prompt();
	char c = 0;
	for(c = wait_for_USART3_data(); c != QUIT; c = wait_for_USART3_data()) { // Loop until quit char is received
		putty_putc(c); // Print character pressed for user feedback.
		putty_print(": ");
		// Decypher user input
		switch(c) {
		case ROLL_CLOCKWISE:
			step_roll_motor(CW);
			putty_print("Rolled Clockwise!\r\n");
		break;
		case ROLL_COUNTER_CLOCKWISE:
			step_roll_motor(CCW);
			putty_print("Rolled Counter-Clockwise!\r\n");
		break;
		case PITCH_CLOCKWISE:
			step_pitch_motor(CW);
			putty_print("Pitched Clockwise!\r\n");
		break;
		case PITCH_COUNTER_CLOCKWISE:
			step_pitch_motor(CCW);
			putty_print("Pitched Counter-Clockwise!\r\n");
		break;
		case GET_ORIENTATION:
			putty_print("data from BNO055!\r\n");
			BNO055_request_data();
		break;
		case RESET:
			reset_values();
			putty_print("Values Reset!\r\n");
		break;
		case '\n':
		case '\r':
		break;
		default:
			putty_unrecognized_prompt(c);
		break;
		}

	}
	putty_print("Leaving Debug Mode...");
}

void reset_values(void) {
	printf("Reseting\n");
	set_target_roll_steps(0);
	set_target_pitch_steps(0);
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



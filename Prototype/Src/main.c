
/**
  ******************************************************************************
  Written by : 
	Dated      :
  ******************************************************************************
  */
	
#include "main.h"
#include "stm32f0xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include "EventRecorder.h"	// Keil::ARM_Compiler (Needed For printf to work)
#include "game_parser.h"
#include "motors.h"
#include "putty.h"
#include "LEDs.h"
#include "USARTs.h"
#include "BNO055.h"
#include "FSRs.h"

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

volatile static float roll;
volatile static float pitch;
void run_all();
uint16_t	adc_dma_buffer[1];
/* STATIC GLOBAL VARIABLES */

int main(void) {
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
  	
  	/* Configure the system clock */
	SystemClock_Config();

  	/* Initialize all configured peripherals */
	//EventRecorderInitialize (EventRecordAll, 1);  // initialize and start Event Recorder (Needed For printf to work)

	chair_init();
		

	game_menu();
}

// Initialize all peripherals needed to have a functioning chair.
void chair_init(void) {
	
	// PB5 (Roll Dir), PB6 (Roll Step), PB7 (Pitch Dir), PB8 (Pitch step)
	motors_init();

	// PC6, PC7, PC8, PC9
	LEDs_init();
	
		// PC4 (USART3_TX), PC5 (USART_RX)
	game_parser_init(); // this must happen after gyro init

	// PB6 (USART1_TX), PB7 (USART1_RX)
  BNO055_init();
	
	//FSRs
	FSRs_init();
	putty_print("INITIALIZATION COMPLETE!");
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
	reset_values();
	// putty_game_prompt();
	BNO055_request_data(); // Request gyro data from the get go.
	
	// Wait for gyro data so we can syncronize motors
	while(!BNO055_orientation_updated());
	euler_data d = BNO055_get_orientation();
	set_actual_roll_steps(d.roll_deg);
	set_actual_pitch_steps(d.pitch_deg);
	set_current_roll_steps(get_actual_roll_steps());
	set_current_pitch_steps(get_actual_pitch_steps());
		
	putty_putc((char)0xFF); // Send start byte.

	// Set current motor to gyro value.
	while(true) {
		run_all();
	}
	//putty_print("Leaving Game Mode...");
	//putty_print("\n\r");
}

void run_all() {
	// Check if there's new game data
		if(has_new_game_data()) {
			game_data g_data = get_game_orientation_data();
			uint16_t fsr1_data = adc_dma_buffer[0];
			// Set target roll steps so motors know how to update.
			set_target_roll_steps(degrees_to_roll_steps(g_data.roll_deg));
			set_target_pitch_steps(degrees_to_pitch_steps(g_data.pitch_deg));
			// Transmit data to user
			char string_to_transmit[40];
			sprintf(string_to_transmit, "game(%d, %d)\tgyro(%d, %d)\tmotor(%d, %d)\tforce(%d)\r\n", get_target_roll_steps(), get_target_pitch_steps(), get_actual_roll_steps(), get_actual_pitch_steps(), get_current_roll_steps(), get_current_pitch_steps(), fsr1_data);
			putty_print(string_to_transmit);
		}
		// Check if the BNO055 has a new orientation to update it.
		if (BNO055_orientation_updated()) {
			euler_data data = BNO055_get_orientation();
			set_actual_roll_steps(data.roll_deg);
			set_actual_pitch_steps(data.pitch_deg);
			BNO055_request_data(); // Request gyro data for next loop.
		}
		
		// Update the motors
		update_motors();
}

void debug_menu() {
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
			while(true) {
				BNO055_request_data();
				char string_to_transmit[40];
				euler_data data = BNO055_get_orientation();
				sprintf(string_to_transmit, "pitch:%.0f, roll:%.0f, heading:%.0f\r\n", data.pitch_deg, data.roll_deg, data.heading_deg);
				putty_print(string_to_transmit);
			}
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



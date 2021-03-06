#ifndef __USARTS_H
#define __USARTS_H

#include "stm32f0xx_hal.h"
#include <stdbool.h>

typedef struct euler_data {
	float pitch_deg;
	float roll_deg;
	float heading_deg;
} euler_data;

typedef struct game_data {
	int pitch_deg;
	int roll_deg;
} game_data;

// Initializes PB6 and PB7 for USART1_TX (RXD) and USART1_RX (TXD).
void USART1_init(void);
// Initializes PC4 and PC5 for USART3_TX (RXD) and USART3_RX (TXD).
void USART3_init(void);
// Wait for USART1 new data to arrive and then return it
char wait_for_USART1_data(void);
// Wait for USART3 new data to arrive and then return it
char wait_for_USART3_data(void);
// Wait till USART3 sends a character and returns true if character matches the parameter.
bool wait_for_USART3_char(char c);

char response_data(void);
bool USART1_read_successfully(void);
bool USART1_wrote_successfully(void);
bool has_new_orientation(void);
bool has_new_game_data(void);
euler_data get_orientation_data(void);
game_data get_game_orientation_data(void);
bool USART3_has_data(void);
char get_USART3_data(void);
char receive_char(USART_TypeDef *USARTx);
void transmit_char(USART_TypeDef *USARTx, char c);
void transmit_string(USART_TypeDef *USARTx, char *string);



#endif /* __USARTS_H */

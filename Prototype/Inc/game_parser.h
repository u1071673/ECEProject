/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GAME_PARSER_H
#define __GAME_PARSER_H

typedef struct FG_data {
	int roll_deg;
	int pitch_deg;
	int accel_mmph; // TODO
} game_data;

// Initialize PC4 and PC5 for USART3_TX (RXD) and USART3_RX (TXD) for Game.
void game_parser_init(void);

// Wait for Game to send data for roll and pitch in the format "\r\n<roll>, <pitch>\r" or until cancel command is given
game_data wait_for_game_data(char cancel_command);

#endif /* __GAME_PARSER_H */

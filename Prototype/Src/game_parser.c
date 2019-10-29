#include <stdlib.h>
#include "game_parser.h"
#include "USARTs.h"

void game_parser_init(void) {
	printf("Initializing Game Parser...\n");
	// Initialize PC4 and PC5 for USART3_TX (RXD) and USART3_RX (TXD) for Game.
	USART3_init();
	printf("Game Parser Initialization Complete!\n");
}

// Wait for Game to send data for roll and pitch in the format "\r\n<roll>, <pitch>\r"
game_data wait_for_game_data(char cancel_cmd) {

	while(!wait_for_USART3_char('\r'));
	printf("got CR!\n");
	while(!wait_for_USART3_char('\n'));
	printf("got newline!\n");	

	int i = 0;
	char c = 0;
	char roll_buff[20];
	char pitch_buff[20];
	// TODO: Add acceleration data.
	for(c = wait_for_USART3_data(); c != ',' || c != cancel_cmd; c = wait_for_USART3_data()) { // Loop through received chars till we get ',' or cancel command
		roll_buff[i++] = c;
	}
	printf("got comma!\n");
	roll_buff[i] = 0;

	i = 0;
	for(c = wait_for_USART3_data(); c != '\r' || c != cancel_cmd; c = wait_for_USART3_data()) { // Loop through received chars till we get '\r' or cancel command
		pitch_buff[i++] = c;
	}
	printf("got CR!\n");
	pitch_buff[i] = 0;
	// TODO: Add acceleration data.

	printf("GOT (%s, %s) from Game\n", roll_buff, pitch_buff); // TODO: Add acceleration data.
	game_data g_data;

	g_data.roll_deg = atoi(roll_buff);
	g_data.pitch_deg = atoi(pitch_buff);
	// TODO: Add acceleration data.

	return g_data;
}

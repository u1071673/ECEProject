#include <stdlib.h>
#include "game_parser.h"
#include "USARTs.h"

void game_parser_init(void) {
	printf("Initializing Game Parser...\n");
	// Initialize PC4 and PC5 for USART3_TX (RXD) and USART3_RX (TXD) for Game.
	USART3_init();
	printf("Game Parser Initialization Complete!\n");
}

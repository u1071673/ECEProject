#include "putty.h"
#include "USARTs.h"

void putty_main_prompt(void) {
	putty_print("\n\r\tSELECT MODE");
	putty_print("\n\r[a] \t\tDebug Mode");
	putty_print("\n\r[b] \t\tGame Mode");
	putty_print("\n\r");
}

void putty_debug_prompt(void) {
	putty_print("\n\r\tDebug mode selected\r\n");
	putty_print("[o]Get Orientation\t[r]Reset\t[q]=quit\n\r");
	putty_print("         [w] Up \n\r");
	putty_print("[a] Left [s] Down [d] Right\n\r");
}

void putty_game_prompt(void) {
	putty_print("\n\r\tGame mode selected");
	putty_print("\n\r");
	putty_print("Reading Roll and Pitch (Press q to quit)");
	putty_print("\n\r");
	putty_print("Format: Roll,Pitch");
	putty_print("\n\r");
}

void putty_unrecognized_prompt(char c) {
	putty_print("\n\rUnrecognized Character: ");
	putty_putc(c);
	putty_print("\n\r");
}

void putty_goodbye_prompt() {
	putty_print("\n\rGood Bye!");
	putty_print("\n\r");
}

void putty_print(char *message) {
	transmit_string(USART3, message);
}

void putty_putc(char c) {
	transmit_char(USART3, c);
}

// Method used to check if putty has input to check
bool recieved_putty_cmd(void) {
	return USART3_has_data();
}

char get_putty_cmd(void) {
	return get_USART3_data();
}

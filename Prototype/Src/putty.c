#include "putty.h"
#include "USARTs.h"

void putty_main_prompt(void) {
	putty_print("\n\r\tSELECT MODE");
	putty_print("\n\r[a] \t\tDEBUG Mode");
	putty_print("\n\r[b] \t\tFG Mode");
	putty_print("\n\r");
}

void putty_debug_prompt(void) {
	putty_print("\n\r\tDebug mode selected");
	putty_print("\n\r");
	putty_print("[w]Roll clockwise\t[s]Roll counter-clockwise\t[-]Decrease roll\t[+]Increase roll;");
	putty_print("\n\r");
	putty_print("[d]Pitch clockwise\t[a]Pitch coutner-clockwise\t[<]Decrease pitch\t[>]Increase pitch");
	putty_print("\n\r");
	putty_print("[r]Reset\t\t[q]=quit");
	putty_print("\n\r");
}

void putty_fg_prompt(void) {
	putty_print("\n\r\tFlight Gear mode selected");
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

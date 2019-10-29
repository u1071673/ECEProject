#ifndef __PUTTY_H
#define __PUTTY_H

#include <stdbool.h>

void putty_main_prompt(void);
void putty_debug_prompt(void);
void putty_game_prompt(void);
void putty_unrecognized_prompt(char c);
void putty_goodbye_prompt(void);
void putty_print(char *message);
void putty_putc(char c);
bool recieved_putty_cmd(void);
char get_putty_cmd(void);

#endif /* __PUTTY_H */


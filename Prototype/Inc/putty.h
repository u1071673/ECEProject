#ifndef __MENUS_H
#define __MENUS_H

void putty_main_prompt(void);
void putty_debug_prompt(void);
void putty_fg_prompt(void);
void putty_unrecognized_prompt(char c);
void putty_goodbye_prompt(void);
void putty_print(char *message);
void putty_putc(char c);

#endif /* __MENUS_H */


#ifndef UI_H
#define UI_H

void ui_init(void);
void ui_cleanup(void);
void ui_display_board(void);
char* ui_get_input(char *buf, int size);
void ui_show_message(const char *msg);

#endif // UI_H

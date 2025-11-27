#include "ui.h"
#include "board.h"
#include "engine.h"
#include <ncurses.h>
#include <string.h>

static int board_row = 1, board_col = 2;
static int msg_row;

void ui_init(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_CYAN, COLOR_BLACK);
    }
    msg_row = board_row + BOARD_SIZE + 6;
}

void ui_cleanup(void) {
    endwin();
}

void ui_display_board(void) {
    int r = board_row;
    mvprintw(r++, board_col, "   a b c d e f g h");
    mvprintw(r++, board_col, "  -----------------");
    for (int i=0;i<BOARD_SIZE;i++){
        int rank = 8 - i;
        mvprintw(r, board_col, "%d|", rank);
        int c = board_col + 3;
        for (int j=0;j<BOARD_SIZE;j++){
            char ch = board[i][j].type;
            if (has_colors() && ch != '.') {
                if (ch >= 'A' && ch <= 'Z') attron(COLOR_PAIR(1));
                else attron(COLOR_PAIR(2));
            }
            mvprintw(r, c, "%c", ch);
            if (has_colors() && ch != '.') {
                if (ch >= 'A' && ch <= 'Z') attroff(COLOR_PAIR(1));
                else attroff(COLOR_PAIR(2));
            }
            c += 2;
        }
        mvprintw(r, c, "|%d", rank);
        r++;
    }
    mvprintw(r++, board_col, "  -----------------");
    mvprintw(r++, board_col, "   a b c d e f g h");
    refresh();
}

char* ui_get_input(char *buf, int size) {
    mvprintw(msg_row+2, board_col, "Ingrese su jugada (ej: e2e4, e7e8Q). Escriba 'salir' para salir.");
    mvprintw(msg_row+3, board_col, "> ");
    echo();
    curs_set(1);
    move(msg_row+3, board_col+2);
    getnstr(buf, size-1);
    noecho();
    curs_set(0);
    return buf;
}

void ui_show_message(const char *msg) {
    mvprintw(msg_row+4, board_col, "                                                  ");
    mvprintw(msg_row+4, board_col, "%s", msg);
    refresh();
}

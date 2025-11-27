#include "board.h"
#include <ctype.h>

Piece board[BOARD_SIZE][BOARD_SIZE];
int castling_flags[2][3] = {{0,0,0},{0,0,0}};

void initialize_board(void) {
    char initial_setup[BOARD_SIZE][BOARD_SIZE] = {
        {'r','n','b','q','k','b','n','r'},
        {'p','p','p','p','p','p','p','p'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'P','P','P','P','P','P','P','P'},
        {'R','N','B','Q','K','B','N','R'}
    };
    for (int i=0;i<BOARD_SIZE;i++) {
        for (int j=0;j<BOARD_SIZE;j++) {
            board[i][j].type = initial_setup[i][j];
            if (initial_setup[i][j]=='.') board[i][j].color = 0;
            else board[i][j].color = (initial_setup[i][j] >= 'A' && initial_setup[i][j] <= 'Z') ? 1 : 2;
        }
    }
    for(int i=0;i<2;i++) for(int j=0;j<3;j++) castling_flags[i][j]=0;
}

Piece get_piece_at(int r, int c) {
    Piece p = {'.',0};
    if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) return p;
    return board[r][c];
}

void set_piece_at(int r, int c, Piece p) {
    if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) return;
    board[r][c] = p;
}

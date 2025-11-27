#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZE 8

typedef struct {
    char type; // 'P','R','N','B','Q','K' or '.'
    int color; // 1 white, 2 black, 0 empty
} Piece;

extern Piece board[BOARD_SIZE][BOARD_SIZE];
extern int castling_flags[2][3]; // king moved, a-rook moved, h-rook moved

void initialize_board(void);
Piece get_piece_at(int r, int c);
void set_piece_at(int r, int c, Piece p);

#endif // BOARD_H

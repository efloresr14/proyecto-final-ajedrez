#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"

int is_valid_square(int r, int c);
int is_path_clear(const Piece b[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc);
int is_legal_pawn_move(const Piece b[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc, int player_color);
int is_legal_rook_move(const Piece b[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc);
int is_legal_knight_move(int fr, int fc, int tr, int tc);
int is_legal_bishop_move(const Piece b[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc);
int is_legal_queen_move(const Piece b[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc);
int is_legal_king_move(const Piece b[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc, int player_color, int check_castling);

int is_square_attacked(const Piece b[BOARD_SIZE][BOARD_SIZE], int r, int c, int attacker_color);
int is_in_check(const Piece b[BOARD_SIZE][BOARD_SIZE], int player_color);
int can_player_make_any_legal_move(int player_color);

void make_move_on_board(Piece temp_board[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc, char promoted_piece_char);

#endif // ENGINE_H

#include "engine.h"
#include <string.h>
#include <stdlib.h>

int is_valid_square(int r, int c) {
    return r>=0 && r<BOARD_SIZE && c>=0 && c<BOARD_SIZE;
}

void make_move_on_board(Piece temp_board[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc, char promoted_piece_char) {
    Piece p = temp_board[fr][fc];
    temp_board[tr][tc] = p;
    temp_board[fr][fc].type = '.';
    temp_board[fr][fc].color = 0;
    if (tolower(p.type) == 'p') {
        if ((p.color == 1 && tr == 0) || (p.color == 2 && tr == BOARD_SIZE-1)) {
            char promote_to = promoted_piece_char;
            if (promote_to == ' ') promote_to = (p.color==1) ? 'Q' : 'q';
            else promote_to = (p.color==1) ? toupper(promote_to) : tolower(promote_to);
            temp_board[tr][tc].type = promote_to;
        }
    }
}

int is_path_clear(const Piece b[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc) {
    int dr = (tr>fr)?1: (tr<fr)?-1:0;
    int dc = (tc>fc)?1: (tc<fc)?-1:0;
    int r = fr + dr, c = fc + dc;
    while (r != tr || c != tc) {
        if (!is_valid_square(r,c) || b[r][c].type != '.') return 0;
        r += dr; c += dc;
    }
    return 1;
}

int is_legal_pawn_move(const Piece b[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc, int player_color) {
    int dir = (player_color==1) ? -1 : 1;
    if (fc==tc && fr+dir==tr && b[tr][tc].type=='.') return 1;
    if (fc==tc && fr+2*dir==tr && b[tr][tc].type=='.' && b[fr+dir][fc].type=='.' &&
        ((player_color==1 && fr==6) || (player_color==2 && fr==1))) return 1;
    if (abs(fc-tc)==1 && fr+dir==tr && b[tr][tc].type!='.' && b[tr][tc].color != player_color) return 1;
    return 0;
}

int is_legal_rook_move(const Piece b[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc) {
    if (fr!=tr && fc!=tc) return 0;
    return is_path_clear(b, fr, fc, tr, tc);
}

int is_legal_knight_move(int fr, int fc, int tr, int tc) {
    int dr = abs(fr-tr), dc = abs(fc-tc);
    return (dr==2 && dc==1) || (dr==1 && dc==2);
}

int is_legal_bishop_move(const Piece b[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc) {
    if (abs(fr-tr) != abs(fc-tc)) return 0;
    return is_path_clear(b, fr, fc, tr, tc);
}

int is_legal_queen_move(const Piece b[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc) {
    return is_legal_rook_move(b, fr, fc, tr, tc) || is_legal_bishop_move(b, fr, fc, tr, tc);
}

int is_square_attacked(const Piece b[BOARD_SIZE][BOARD_SIZE], int r, int c, int attacker_color) {
    for (int i=0;i<BOARD_SIZE;i++){
        for (int j=0;j<BOARD_SIZE;j++){
            if (b[i][j].type != '.' && b[i][j].color == attacker_color) {
                char piece_type = tolower(b[i][j].type);
                int can_attack = 0;
                if (piece_type == 'p') {
                    int dir = (attacker_color==1)? -1 : 1;
                    if (i+dir==r && abs(j-c)==1) can_attack=1;
                } else if (piece_type == 'r') can_attack = is_legal_rook_move(b, i, j, r, c);
                else if (piece_type == 'n') can_attack = is_legal_knight_move(i, j, r, c);
                else if (piece_type == 'b') can_attack = is_legal_bishop_move(b, i, j, r, c);
                else if (piece_type == 'q') can_attack = is_legal_queen_move(b, i, j, r, c);
                else if (piece_type == 'k') can_attack = is_legal_king_move(b, i, j, r, c, attacker_color, 0);
                if (can_attack) return 1;
            }
        }
    }
    return 0;
}

int is_in_check(const Piece b[BOARD_SIZE][BOARD_SIZE], int player_color) {
    int kr=-1,kc=-1;
    for (int i=0;i<BOARD_SIZE;i++){
        for (int j=0;j<BOARD_SIZE;j++){
            if (b[i][j].type != '.' && b[i][j].color == player_color && tolower(b[i][j].type)=='k') {
                kr=i; kc=j; break;
            }
        }
        if (kr!=-1) break;
    }
    if (kr==-1) return 0;
    return is_square_attacked(b, kr, kc, 3-player_color);
}

int is_legal_king_move(const Piece b[BOARD_SIZE][BOARD_SIZE], int fr, int fc, int tr, int tc, int player_color, int check_castling) {
    int dr = abs(fr-tr), dc = abs(fc-tc);
    if (dr <= 1 && dc <= 1 && (dr+dc>0)) return 1;
    if (check_castling) {
        int king_row = (player_color==1)?7:0;
        if (fr==king_row && fc==4 && tr==king_row && !castling_flags[player_color-1][0]) {
            if (tc==6 && !castling_flags[player_color-1][2] &&
                b[king_row][5].type=='.' && b[king_row][6].type=='.' &&
                !is_square_attacked(b, king_row,4,3-player_color) &&
                !is_square_attacked(b, king_row,5,3-player_color) &&
                !is_square_attacked(b, king_row,6,3-player_color)) {
                return 2;
            }
            if (tc==2 && !castling_flags[player_color-1][1] &&
                b[king_row][3].type=='.' && b[king_row][2].type=='.' && b[king_row][1].type=='.' &&
                !is_square_attacked(b, king_row,4,3-player_color) &&
                !is_square_attacked(b, king_row,3,3-player_color) &&
                !is_square_attacked(b, king_row,2,3-player_color)) {
                return 3;
            }
        }
    }
    return 0;
}

int can_player_make_any_legal_move(int player_color) {
    Piece temp_board_state[BOARD_SIZE][BOARD_SIZE];
    for (int r1=0;r1<BOARD_SIZE;r1++){
        for (int c1=0;c1<BOARD_SIZE;c1++){
            if (board[r1][c1].type != '.' && board[r1][c1].color == player_color) {
                for (int r2=0;r2<BOARD_SIZE;r2++){
                    for (int c2=0;c2<BOARD_SIZE;c2++){
                        if (board[r2][c2].color == player_color) continue;
                        char piece_char = tolower(board[r1][c1].type);
                        int move_type_ok = 0;
                        if (piece_char == 'p') move_type_ok = is_legal_pawn_move(board, r1, c1, r2, c2, player_color);
                        else if (piece_char == 'r') move_type_ok = is_legal_rook_move(board, r1, c1, r2, c2);
                        else if (piece_char == 'n') move_type_ok = is_legal_knight_move(r1, c1, r2, c2);
                        else if (piece_char == 'b') move_type_ok = is_legal_bishop_move(board, r1, c1, r2, c2);
                        else if (piece_char == 'q') move_type_ok = is_legal_queen_move(board, r1, c1, r2, c2);
                        else if (piece_char == 'k') move_type_ok = is_legal_king_move(board, r1, c1, r2, c2, player_color, 1);
                        if (move_type_ok) {
                            memcpy(temp_board_state, board, sizeof(Piece)*BOARD_SIZE*BOARD_SIZE);
                            make_move_on_board(temp_board_state, r1, c1, r2, c2, ' ');
                            if (!is_in_check(temp_board_state, player_color)) return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

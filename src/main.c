#include <stdio.h>
#include <string.h>
#include "board.h"
#include "engine.h"
#include "ui.h"

// parse and mover functions (declaraciones)
int parse_algebraic_notation(const char* movida_str, int* fr, int* fc, int* tr, int* tc, char* promoted_piece_char);
char* mover(int jugador, const char* movida_str, size_t longitud_movida);

int parse_algebraic_notation(const char* movida_str, int* fr, int* fc, int* tr, int* tc, char* promoted_piece_char) {
    *promoted_piece_char = ' ';
    size_t len = strlen(movida_str);
    if (len < 4 || len > 5) return 0;
    if (movida_str[0] < 'a' || movida_str[0] > 'h' ||
        movida_str[1] < '1' || movida_str[1] > '8' ||
        movida_str[2] < 'a' || movida_str[2] > 'h' ||
        movida_str[3] < '1' || movida_str[3] > '8') return 0;
    *fc = movida_str[0] - 'a';
    *fr = 7 - (movida_str[1] - '1');
    *tc = movida_str[2] - 'a';
    *tr = 7 - (movida_str[3] - '1');
    if (len == 5) {
        *promoted_piece_char = movida_str[4];
        char pc = (char) ( (*promoted_piece_char>='a' && *promoted_piece_char<='z') ? *promoted_piece_char - 32 : *promoted_piece_char );
        if (pc != 'Q' && pc != 'R' && pc != 'B' && pc != 'N') return 0;
    }
    return 1;
}

static char message_buffer[256];

char* mover(int jugador, const char* movida_str, size_t longitud_movida) {
    int fr,fc,tr,tc;
    char promoted_piece_char_input = ' ';
    message_buffer[0] = '\\0';

    if (!parse_algebraic_notation(movida_str, &fr, &fc, &tr, &tc, &promoted_piece_char_input)) {
        snprintf(message_buffer, sizeof(message_buffer), "Error: Formato de jugada inválido ('%s'). Use e.g. 'e2e4' o 'e7e8Q'.", movida_str);
        return NULL;
    }

    Piece moving_piece = get_piece_at(fr, fc);
    if (moving_piece.type == '.') {
        snprintf(message_buffer, sizeof(message_buffer), "Error: No hay pieza en la casilla de origen (%s).", movida_str);
        return NULL;
    }
    if (moving_piece.color != jugador) {
        snprintf(message_buffer, sizeof(message_buffer), "Error: La pieza en %c%c no es tuya.", movida_str[0], movida_str[1]);
        return NULL;
    }
    Piece target_square_piece = get_piece_at(tr, tc);
    if (target_square_piece.type != '.' && target_square_piece.color == jugador) {
        snprintf(message_buffer, sizeof(message_buffer), "Error: No puedes capturar tus propias piezas en %c%c.", movida_str[2], movida_str[3]);
        return NULL;
    }

    int move_valid_for_type = 0;
    char piece_type_lower = (moving_piece.type >= 'A' && moving_piece.type <= 'Z') ? moving_piece.type + 32 : moving_piece.type;
    int castling_move_type = 0;

    if (piece_type_lower == 'p') move_valid_for_type = is_legal_pawn_move(board, fr, fc, tr, tc, jugador);
    else if (piece_type_lower == 'r') move_valid_for_type = is_legal_rook_move(board, fr, fc, tr, tc);
    else if (piece_type_lower == 'n') move_valid_for_type = is_legal_knight_move(fr, fc, tr, tc);
    else if (piece_type_lower == 'b') move_valid_for_type = is_legal_bishop_move(board, fr, fc, tr, tc);
    else if (piece_type_lower == 'q') move_valid_for_type = is_legal_queen_move(board, fr, fc, tr, tc);
    else if (piece_type_lower == 'k') {
        castling_move_type = is_legal_king_move(board, fr, fc, tr, tc, jugador, 1);
        move_valid_for_type = (castling_move_type > 0);
    }

    if (!move_valid_for_type) {
        snprintf(message_buffer, sizeof(message_buffer), "Error: Movimiento ilegal para la pieza '%c' de %s.", moving_piece.type, movida_str);
        return NULL;
    }

    Piece temp_board_state[BOARD_SIZE][BOARD_SIZE];
    memcpy(temp_board_state, board, sizeof(Piece)*BOARD_SIZE*BOARD_SIZE);
    make_move_on_board(temp_board_state, fr, fc, tr, tc, promoted_piece_char_input);
    if (is_in_check(temp_board_state, jugador)) {
        snprintf(message_buffer, sizeof(message_buffer), "Error: Este movimiento deja a tu rey en jaque.");
        return NULL;
    }

    Piece captured_piece = get_piece_at(tr, tc);
    set_piece_at(tr, tc, moving_piece);
    set_piece_at(fr, fc, (Piece){'.',0});

    if (piece_type_lower == 'p') {
        if ((jugador==1 && tr==0) || (jugador==2 && tr==BOARD_SIZE-1)) {
            char promote_to = promoted_piece_char_input;
            if (promote_to == ' ') promote_to = (jugador==1)?'Q':'q';
            else promote_to = (jugador==1)? ( (promote_to>='a'&&promote_to<='z')?promote_to-32:promote_to ) : ( (promote_to>='A'&&promote_to<='Z')?promote_to+32:promote_to );
            board[tr][tc].type = promote_to;
            snprintf(message_buffer, sizeof(message_buffer), "Movimiento %s realizado. Peón promocionado a %c.", movida_str, board[tr][tc].type);
            return message_buffer;
        }
    }

    if (castling_move_type == 2) {
        int row = (jugador==1)?7:0;
        Piece rook = get_piece_at(row,7);
        set_piece_at(row,5,rook);
        set_piece_at(row,7,(Piece){'.',0});
        snprintf(message_buffer, sizeof(message_buffer), "Movimiento %s realizado. Enroque corto.", movida_str);
    } else if (castling_move_type == 3) {
        int row = (jugador==1)?7:0;
        Piece rook = get_piece_at(row,0);
        set_piece_at(row,3,rook);
        set_piece_at(row,0,(Piece){'.',0});
        snprintf(message_buffer, sizeof(message_buffer), "Movimiento %s realizado. Enroque largo.", movida_str);
    }

    if (piece_type_lower == 'k') castling_flags[jugador-1][0] = 1;
    if (fr == 7 && fc == 0 && piece_type_lower=='r' && jugador==1) castling_flags[0][1] = 1;
    if (fr == 7 && fc == 7 && piece_type_lower=='r' && jugador==1) castling_flags[0][2] = 1;
    if (fr == 0 && fc == 0 && piece_type_lower=='r' && jugador==2) castling_flags[1][1] = 1;
    if (fr == 0 && fc == 7 && piece_type_lower=='r' && jugador==2) castling_flags[1][2] = 1;
    if (tr == 7 && tc == 0) castling_flags[0][1] = 1;
    if (tr == 7 && tc == 7) castling_flags[0][2] = 1;
    if (tr == 0 && tc == 0) castling_flags[1][1] = 1;
    if (tr == 0 && tc == 7) castling_flags[1][2] = 1;

    if (message_buffer[0] == '\\0') {
        if (captured_piece.type != '.') {
            snprintf(message_buffer, sizeof(message_buffer), "Movimiento %s realizado. Pieza %c capturada.", movida_str, captured_piece.type);
        } else {
            snprintf(message_buffer, sizeof(message_buffer), "Movimiento %s realizado.", movida_str);
        }
    }
    return message_buffer;
}

int main() {
    initialize_board();
    ui_init();

    int currentPlayer = 1;
    char movida_input[16];

    int game_over = 0;
    while (!game_over) {
        clear();
        ui_display_board();
        mvprintw(0,0,"--- Turno del Jugador %d (%s) ---", currentPlayer, (currentPlayer==1)?"BLANCAS":"NEGRAS");
        if (is_in_check(board, currentPlayer)) {
            mvprintw(1,0,"¡ESTÁS EN JAQUE!");
        }

        if (!can_player_make_any_legal_move(currentPlayer)) {
            if (is_in_check(board, currentPlayer)) {
                mvprintw(2,0,"¡JAQUE MATE! Jugador %d (%s) gana.", 3-currentPlayer, (currentPlayer==1)?"NEGRAS":"BLANCAS");
            } else {
                mvprintw(2,0,"¡AHOGADO! El juego es un empate.");
            }
            game_over = 1;
            mvprintw(4,0,"Presione cualquier tecla para salir...");
            refresh();
            getch();
            break;
        }

        ui_get_input(movida_input, sizeof(movida_input));
        if (strcmp(movida_input, "salir")==0) break;

        char* resultado = mover(currentPlayer, movida_input, strlen(movida_input));
        if (resultado != NULL) {
            ui_show_message(resultado);
            currentPlayer = (currentPlayer==1)?2:1;
        } else {
            ui_show_message("Movimiento inválido. Intente de nuevo.");
        }
        mvprintw(20,0,"Presione cualquier tecla para continuar...");
        refresh();
        getch();
    }

    ui_cleanup();
    return 0;
}

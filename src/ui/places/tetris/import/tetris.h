/**
 * Tetris Game library header file
 * @author Jacob Bokor, jacobbokor.com
 * @date 03/2024
*/

#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H



#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>         // used for malloc(), free()
#include <string.h>         // memcpy
#include <sys/time.h>       // timeval for microsecond time intervals


// TETRIS GAME LOGIC DEBUG FLAG
// all print/fprintf statements in tetris.c are gated by this flag. extremely helpful
//  for debugging game logic, but should not be present in release builds 
// #define DEBUG_T 1

#ifdef DEBUG_T
// separate game log file to prevent ncurses printing issues
extern FILE *gamelog;
#endif

#define TETRIS_ROWS 20
#define TETRIS_COLS 9

// how many different piece types and orientations
#define NUM_TETROMINOS 7
#define NUM_ORIENTATIONS 4
#define NUM_CELLS_IN_TETROMINO 4

// how many microseconds faster game tick should be after each level increase
#define GRAVITY_TICK_RATE_DELTA 10000
// initial gravity tick rate (how fast between each gravity move down in microseconds)
#define GRAVITY_TICK_RATE_INITIAL 200000
// gravity_tick minimum where we won't decrease it any further past this point
#define GRAVITY_TICK_RATE_FLOOR 20000

#ifdef __cplusplus
extern "C" {
#endif

// Points per line cleared, combos not implemented
// See: https://tetris.wiki/Scoring
static const uint16_t points_per_line_cleared[5] = {0, 100, 300, 500, 800};


// piece descriptions
// S, Z, T, L, reverse L (J), square, long bar (I)
enum piece_type {S_PIECE, Z_PIECE, T_PIECE, L_PIECE, J_PIECE, SQ_PIECE, I_PIECE};

enum piece_colors {S_CELL_COLOR, Z_CELL_COLOR, T_CELL_COLOR, L_CELL_COLOR, J_CELL_COLOR, SQ_CELL_COLOR, I_CELL_COLOR};
#define BG_COLOR -1

// Define possible moves that can be taken by player
enum player_move {T_NONE, T_UP, T_DOWN, T_LEFT, T_RIGHT, T_PLAYPAUSE, T_QUIT};


/**
 * row,col location on tetris board, from top right. 
 * Negative numbers allowed so this can be used for offsets
 * @param int8_t row
 * @param int8_t col
*/
typedef struct tetris_location {
    int8_t row;
    int8_t col;
} tetris_location;

// tetris piece descriptions - defined here to prevent multiple definition
extern const tetris_location TETROMINOS[NUM_TETROMINOS][NUM_ORIENTATIONS][NUM_CELLS_IN_TETROMINO];

/**
 * Struct representing a single Tetromino
 * @param ptype - enum type of piece
 * @param tetris_location loc - piece position [row,col]
 * @param orientation - piece orientation [0-3]
 * @param falling - bool, true if currently falling
*/
typedef struct TetrisPiece {
    enum piece_type ptype;
    tetris_location loc;
    uint8_t orientation;    // orientation, in range 0-3
    bool falling;
} TetrisPiece;

/**
 * Represents the game board
 * @param board 2D int8_t array representing board
 *  -1 means unoccupied, >0 indicates cell color by piece_colors[]
 * @param highest_occupied_row uint8_t tallest point in current stack, tracked to 
 *  avoid needless recomputation and help indicate gameover condition
*/
typedef struct TetrisBoard {
    int8_t board[TETRIS_ROWS][TETRIS_COLS];
    uint8_t highest_occupied_cell;

} TetrisBoard;

/**
 * Tetris Game Struct
 * @param board 2D struct array of set pieces on board
 * @param active_board 2D struct array representing entire board 
 *  (including falling piece)
 * @param game_over bool true if game over, false if not
 * @param gravity_tick_rate uint32_t usec between each gravity tick
 * @param score uint32_t player's current score
 * @param level uint32_t current level
 * @param lines_cleared_since_last_level - uint8_t 
 * @param last_gravity_tick_usec - `struct timeval` last time active_piece was moved down
*/
typedef struct TetrisGame {
    TetrisBoard board;
    TetrisBoard active_board;
    TetrisPiece active_piece;
    bool game_over;
    uint32_t score;
    uint32_t level;
    uint32_t gravity_tick_rate_usec;
    uint8_t lines_cleared_since_last_level;

    // uint32_t last_gravity_tick_usec;
    // this requires <sys/time.h>
    struct timeval last_gravity_tick_usec;
} TetrisGame;

////////////////////////////////////////
//////       FUNCTION DEFS        //////
////////////////////////////////////////

// init/end functions

TetrisGame* create_game(void);
void end_game(TetrisGame *tg);
TetrisBoard init_board(void);

// This is the main function for using this library; all game state is handled internally

bool tg_tick(TetrisGame *tg, enum player_move move);


TetrisBoard render_active_board(TetrisGame *tg);
bool check_and_spawn_new_piece(TetrisGame *tg);

TetrisPiece create_rand_piece(TetrisGame *tg);

// check game state conditions

bool check_valid_move(TetrisGame *tg, uint8_t player_move);
bool test_piece_offset(TetrisBoard *tb, const tetris_location global_loc, const tetris_location move_offset);
bool test_piece_rotate(TetrisBoard *tb, const TetrisPiece tp);
bool check_do_piece_gravity(TetrisGame *tg);

bool check_filled_row(TetrisGame *tg, uint8_t row);
uint8_t check_and_clear_rows(TetrisGame *tg, tetris_location *tp_cells);
void clear_rows(TetrisGame *tg, uint8_t top_row, uint8_t num_rows);

bool check_game_over(TetrisGame *tg);

// helper functions

bool val_in_arr(const uint8_t val, uint8_t arr[], const uint8_t arr_len);
int32_t get_elapsed_us(struct timeval before, struct timeval after);
uint8_t smallest_in_arr(uint8_t arr[], const uint8_t arr_size);
void int16_to_uint8_arr(int16_t *in_arr, uint8_t *out_arr, uint8_t arr_size);
void uint8_to_int16_arr(uint8_t *in_arr, int16_t *out_arr, uint8_t arr_size);

#ifdef __cplusplus
}
#endif

#endif
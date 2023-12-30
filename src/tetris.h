#pragma once

#include <stdbool.h>
#define NUM_TETROMINOS 7
#define NUM_ORIENTATION 4
#define NUM_TET_BLOCK 4

typedef struct {
    int row,col;
} point; 

typedef enum {
    TET_I, TET_J, TET_L, TET_O, TET_S, TET_T, TET_Z
} Tetromino_type;

typedef struct {
    Tetromino_type type;
    int orientation;
    point location;
} Tetromino;

#define TETRIS_VIDE 0

#define MAX_LEVEL 19
#define LINES_PER_LEVEL 10


typedef enum {
  TM_LEFT, TM_RIGHT, TM_CLOCK, TM_COUNTER, TM_DROP, TM_NONE
} tetris_movement;

typedef struct {
    int *grid;
    unsigned height, width, score, level,lines_remaining;
    int autoMove;
    Tetromino currentPiece;
    Tetromino nextPiece;

} tetris_game;


// functions prototypes
tetris_game * tetris_create(unsigned, unsigned);
void tetris_init(tetris_game *, unsigned , unsigned);
void tetris_destroy(tetris_game *);
void tetris_auto_move(tetris_game *game);
bool tetris_game_over(tetris_game *);

point tetromino_get(int nTet, int nOr, int nBl);
void tetris_hard_drop(tetris_game *game);
void tetris_soft_drop(tetris_game *game);
void tetris_move(tetris_game *game, int direction);
void tetris_rotate(tetris_game *game, int direction);
int tetris_check_lines(tetris_game *game);
void tetris_adjust_score(tetris_game *game, int lines_cleared);

#include "tetris.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

point TETROMINOS[NUM_TETROMINOS][NUM_ORIENTATION][NUM_TET_BLOCK] = {
  // I
  {{{1, 0}, {1, 1}, {1, 2}, {1, 3}},
   {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
   {{3, 0}, {3, 1}, {3, 2}, {3, 3}},
   {{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
  // J
  {{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
   {{0, 1}, {1, 1}, {2, 0}, {2, 1}}},
  // L
  {{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
   {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
  // O
  {{{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}}},
  // S
  {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
   {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
   {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
   {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
  // T
  {{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
   {{0, 1}, {1, 0}, {1, 1}, {2, 1}}},
  // Z
  {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
   {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
   {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
   {{0, 1}, {1, 0}, {1, 1}, {2, 0}}},
};

int GRAVITY_LEVEL[MAX_LEVEL + 1] = {
  50, 48, 46, 44, 42, 39, 36, 33, 30, 27,
  22, 18, 14, 10, 7, 6, 5, 4,  3,  2
};


point tetromino_get(int nTet, int nOr, int nBl) {
    return TETROMINOS[nTet][nOr][nBl];
}

int tetris_get(tetris_game *game, int row, int col) {
    return game->grid[game->width * row + col];
};

/*
    @brief modifie la valeur d'une cellule
*/
static void tetris_set(tetris_game *game, int row, int col, int value) {
    game->grid[game->width * row + col] = value;
}

bool tetris_check(tetris_game *game,int row, int col) {
    return row >= 0 && col >= 0 && row < game->height && col < game->width;
}

/*
    @brief place une piece dans la grille du jeu
*/
void tetris_put(tetris_game *game, Tetromino tetromino) {
    unsigned i;
    for (i = 0; i < NUM_TET_BLOCK; i++) {
        point cell = TETROMINOS[tetromino.type][tetromino.orientation][i];
        tetris_set(game,tetromino.location.row + cell.row,tetromino.location.col + cell.col,tetromino.type + 1);
    }
}

/*
    @brief enleve une piece de la grille du jeu
*/
static void tetris_remove(tetris_game *game, Tetromino tetromino) {
    unsigned i;
    for (i = 0; i < NUM_TET_BLOCK; i++) {
        point cell = TETROMINOS[tetromino.type][tetromino.orientation][i];
        tetris_set(game, tetromino.location.row + cell.row, tetromino.location.col + cell.col, TETRIS_VIDE);
    }
}

/*
    @brief verifie si une piece peut etre placée sur la grille du jeu
*/
static bool tetris_fits(tetris_game *game, Tetromino tetromino) {
    int i, r, c;
    for (i = 0; i < NUM_TET_BLOCK; i++) {
        point cell = TETROMINOS[tetromino.type][tetromino.orientation][i];
        r = tetromino.location.row + cell.row;
        c = tetromino.location.col + cell.col;
        if (!tetris_check(game, r, c) || tetris_get(game, r, c)) {
        return false;
        }
    }
    return true;
}


static int random_tetromino() {
    return rand() % NUM_TETROMINOS;
}

/*
    @brief modifie la piece courante et génere la piece suivante
*/
static void tetris_new_piece(tetris_game *game) {
    game->currentPiece = game->nextPiece;
    game->nextPiece.type = random_tetromino();
    game->nextPiece.orientation = 0;
    game->nextPiece.location.row = 0;
    game->nextPiece.location.col = game->width/2 - 2;
}

/*
    @brief faire un mouvement a gauche (-1) ou a droite (1) d'une piece
*/
void tetris_move(tetris_game *game, int direction) {
    tetris_remove(game, game->currentPiece);
    game->currentPiece.location.col += direction;
    if (!tetris_fits(game, game->currentPiece)) {
        game->currentPiece.location.col -= direction;
    }
    tetris_put(game, game->currentPiece);
}

void tetris_soft_drop(tetris_game *game) {
    tetris_remove(game, game->currentPiece);
    game->currentPiece.location.row++;
    if (!tetris_fits(game,game->currentPiece)) game->currentPiece.location.row--;
    tetris_put(game, game->currentPiece);
}
/*
    @brief effectue un hard drop
*/
void tetris_hard_drop(tetris_game *game) {
    tetris_remove(game, game->currentPiece);
    while (tetris_fits(game, game->currentPiece)) {
        game->currentPiece.location.row++;
    }
    game->currentPiece.location.row--;
    tetris_put(game, game->currentPiece);
    tetris_new_piece(game);
}
/*
    @brief effectuer une rotation
*/
void tetris_rotate(tetris_game *game, int direction) {
    tetris_remove(game, game->currentPiece);
    while (true) {
        game->currentPiece.orientation = (game->currentPiece.orientation + direction) % NUM_ORIENTATION;
        if (tetris_fits(game, game->currentPiece)) break; // si la piece orienté peut etre placée 
        game->currentPiece.location.col--; // sinon on essaye de la bouger a gauche 
        if (tetris_fits(game, game->currentPiece)) break;
        game->currentPiece.location.col += 2; // sinon on la bouge a droite
        if (tetris_fits(game, game->currentPiece)) break;
        game->currentPiece.location.col--; // sa place original
    }
    tetris_put(game, game->currentPiece);
}


/*
    @brief verifie si une ligne est pleine
*/
static bool tetris_line_full(tetris_game *game, int i) {
    int j;
    for (j = 0; j < game->width; j++) {
        if (!tetris_get(game, i, j))
        return false;
    }
    return true;
}

/*
    @brief 
*/
static void tetris_shift_lines(tetris_game *game, int r) {
    int i, j;
    for (i = r-1; i >= 0; i--) {
        for (j = 0; j < game->width; j++) {
            tetris_set(game, i+1, j, tetris_get(game, i, j));
            tetris_set(game, i, j, TETRIS_VIDE);
        }
    }
}

/*
    @brief supprime les lignes pleines,shift et retourne le nombre de lignes supprimées
*/
int tetris_check_lines(tetris_game *game) {
    int i, nlines = 0;
    tetris_remove(game, game->currentPiece);
    for (i = game->height-1; i >= 0; i--) {
        if (tetris_line_full(game, i)) {
            tetris_shift_lines(game, i);
            i++;
            nlines++;
        }
    }

    tetris_put(game, game->currentPiece);
    return nlines;
}

/*
    @brief modifie le score du jeu 
*/
void tetris_adjust_score(tetris_game *game, int lines_cleared) {
    static int line_multiplier[] = {0, 40, 100, 300, 1200};
    game->score += line_multiplier[lines_cleared] * (game->level + 1);
    if (lines_cleared >= game->lines_remaining) {
        game->level = MIN(MAX_LEVEL, game->level + 1);
        lines_cleared -= game->lines_remaining;
        game->lines_remaining = LINES_PER_LEVEL - lines_cleared;
    } else {
        game->lines_remaining -= lines_cleared;
    }
}
/*
    @brief retourne vrai si c'est la fin du jeu 
*/
bool tetris_game_over(tetris_game *game) {
    int i, j;
    bool over = false;
    tetris_remove(game, game->currentPiece);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < game->width; j++) {
            if (tetris_get(game, i, j)) {
                over = true;
            }
        }
    }
    tetris_put(game, game->currentPiece);
    return over;
}
/*
    @brief effectue un mouvement automatique vers le bas d'une piece
*/
void tetris_auto_move(tetris_game *game) {
    game->autoMove--;
    if (game->autoMove <= 0) {
        tetris_remove(game, game->currentPiece);
        game->currentPiece.location.row++;
        if (tetris_fits(game, game->currentPiece)) {
            game->autoMove = GRAVITY_LEVEL[game->level];
        } else {
            game->currentPiece.location.row--;
            tetris_put(game, game->currentPiece);

            tetris_new_piece(game);
        }
        tetris_put(game, game->currentPiece);
    }
}
/*
    @brief initialise le jeu
*/
void tetris_init(tetris_game *game, unsigned height, unsigned width) {
    game->height = height;
    game->width = width;
    game->grid = (int *)malloc(sizeof(int) * height * width);
    if (!game->grid) {
        perror("alloc error : ");
        exit(EXIT_FAILURE);
    }
    memset(game->grid, TETRIS_VIDE, sizeof(int) * height * width);
    game->score = 0;
    game->level = 0;
    game->autoMove = GRAVITY_LEVEL[game->level];
    game->lines_remaining = LINES_PER_LEVEL;
    srand(time(NULL));
    tetris_new_piece(game);
    tetris_new_piece(game);
    game->nextPiece.location.col = game->width/2 - 2;
}

tetris_game *tetris_create(unsigned height, unsigned width)
{
  tetris_game *game = (tetris_game *)malloc(sizeof(tetris_game));
  if (!game) {
    perror("alloc error : ");
    exit(EXIT_FAILURE);
  }
  tetris_init(game, height, width);
  return game;
}



void tetris_destroy(tetris_game *game) {
    if (!game) return;
    if (game->grid) free(game->grid);
    free(game);
}
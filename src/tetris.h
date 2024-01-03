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
    Tetromino holdPiece;
} tetris_game;


// functions prototypes

/**
 * @brief crée et initialise une structure tetris_game
 * 
 * @param height hauteur du plateau de jeu
 * @param width largeur du plateau de jeu
 * 
 * @return un pointeur sur une structure tetris_game
*/
tetris_game * tetris_create(unsigned height, unsigned width);
/**
 * @brief initialise une structure tetris_game
 *
 * @param game structure tetris_game represantant le jeu
 * @param height hauteur du plateau de jeu
 * @param width largeur du plateau de jeu 
 * 
*/
void tetris_init(tetris_game *game, unsigned height, unsigned width);
/**
 * @brief libére la mémoire d'un jeu
 * 
 * @param game est la structure represantant le jeu
 * 
*/
void tetris_destroy(tetris_game *game);
/**
 * @brief effectue un mouvement autaomatique chaque certain nombres de frames determiné par le niveau du jeu  
 * 
 * @param game est la structure represantant le jeu
*/
void tetris_auto_move(tetris_game *game);
/**
 * @brief verifie si le jeu est fini.
 * 
 * @param game est la structure represantant le jeu. 
 * 
 * @return true si le jeu est fini false sinon.
*/
bool tetris_game_over(tetris_game *game);
/**
 * @brief retourne les coordonnées d'un block d'un tetromino dans le tableau reprsantant les tetrominos
 * 
 * @param nTet est le numéro du tetromino
 * @param nOr le numéro de l'orientation
 * @param nBl numéro du Block
 * 
 * @return structure point qui contient numéro de la ligne row et numéro de la colonne col
 * 
*/
point tetromino_get(int nTet, int nOr, int nBl);
/**
 * @brief effectue un hard drop
 * 
 * @param game est la structure represantant le jeu.
*/
void tetris_hard_drop(tetris_game *game);
/**
 * @brief effectue un soft drop
 * 
 * @param game est la structure represantant le jeu.
*/
void tetris_soft_drop(tetris_game *game);
/**
 * @brief effectue un mouvement vers la gauche (-1) ou la droite (+1)
 * 
 * @param game est la structure represantant le jeu.
 * @param direction (+/- 1) indique la direction gauche ou droite
*/
void tetris_move(tetris_game *game, int direction);
/**
 * @brief effectue une rotation dans le sens positif (+1) ou negatif (-1)
 * 
 * @param game est la structure represantant le jeu.
 * @param direction (+/- 1) indique le sens de la rotation 
*/
void tetris_rotate(tetris_game *game, int direction);
/**
 * @brief effectue une action hold (stocker la piece courante ou l'echanger avec une autre deja stocké)
 * 
 * @param game est la structure represantant le jeu.
*/
void tetris_hold(tetris_game *game);
/**
 * @brief supprime les lignes pleines,fait un shift et retourne le nombre de lignes supprimées
 * 
 * @param game est la structure represantant le jeu.
 * 
 * @return un entier indiquant le nombre de lignes supprimées
*/
int tetris_check_lines(tetris_game *game);
/**
 * @brief modifie le score du jeu d'aprés le nombres lignes 
 * 
 * @param game est la structure represantant le jeu.
 * @param lines_cleared nombre de lignes supprimées
*/
void tetris_adjust_score(tetris_game *game, int lines_cleared);

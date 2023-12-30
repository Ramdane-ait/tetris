#pragma once

#include "tetris.h"
#include "viewer.h"

typedef struct {
    tetris_game *game;
    viewer *view;
} controller;

/*
 * @brief crée un controller
 *
 * @param v est la vue
 * 
 * @param game structure represantant le jeu
 * 
 * @return un pointeur sur un controller
*/
controller *make_controller(viewer *, tetris_game *);

/*
 * @brief lance le jeu tetris
 * 
 * @param un pointeur sur un controller 
 *
*/
void run(controller *);

/*
 * @ brief détruit le controller
 *
 * @param un pointeur sur un controller
*/
void destroy_controller(controller *);

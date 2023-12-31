#pragma once 

#include <ncurses.h>
#include "viewer.h"

typedef struct {
    WINDOW *grid,*score,*next;
} dataTextViewer;


/**
 * @brief Crée et initialise un viewer.
 * 
 * @param width est la largeur du jeu.
 * @param height est la hauteur du jeu.
 * 
 * @return un pointeur sur un text viewer.
 */
viewer *initTextViewer(int width, int height);

/*
 * @brief recupére un évenement 
 * 
 * @param v le viewer d'ou recuperer l'evenement
 * 
 * @return un type enuméré Event
*/
Event getTextEvent(viewer *v);

/*
 * @brief affiche le jeu (plateau,informations) en mode Text 
 *
 * @param v est la vue
 * 
 * @param game structure represantant le jeu
*/
void render_game_text(viewer *v,tetris_game *game);
/*
 * @brief affiche un message de fin du jeu
 *
 * @param v est la vue
 * 
 * @param game structure represantant le jeu
*/
void draw_game_over_text(viewer *,tetris_game *);
void stopTextViewer(viewer *v);
/**
 * @brief Detruit le viewer.
 *
 * @param v est le viewer qu'on souhaite libérer.
 */

void destroyTextViewer(viewer *v);
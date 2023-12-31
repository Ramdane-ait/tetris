#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "viewer.h"

#define BLOCK_SIZE 32
#define FONT_PATH "src/font.ttf"

typedef struct {
    int r,g,b,a;
} Color;

typedef struct viewer viewer;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
} dataSdlViewer;

/*
* @brief crée et initialise une fenetre SDL
*
* @param width est la largeur de la fenetre
* @param height est la hauteur de la fenetre
*
* @return un pointeur sur la vue SDL
*/
viewer *initSdlViewer(int width , int height);

/*
 * @brief recupére un évenement 
 * 
 * @param v le viewer d'ou recuperer l'evenement
 * 
 * @return un type enuméré Event
*/
Event getSdlEvent(struct viewer *v);

/*
 * @brief affiche le jeu (plateau,informations) en mode SDL 
 *
 * @param v est la vue
 * 
 * @param game structure represantant le jeu
*/
void render_game_sdl(viewer *v,tetris_game *game);
/*
 * @brief affiche un message de fin du jeu
 *
 * @param v est la vue
 * 
 * @param game structure represantant le jeu
*/
void draw_game_over_sdl(viewer *,tetris_game *);

void stopSdlViewer(viewer *v);
/**
 * @brief Detruit le viewer.
 *
 * @param v est le viewer qu'on souhaite libérer.
 */
void destroySdlViewer(viewer *);
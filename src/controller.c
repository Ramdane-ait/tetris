#include "controller.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

controller *make_controller(viewer *v, tetris_game *game) {
    assert(v && game);
    controller *ret = (controller *)malloc(sizeof(controller));
    if (!ret) {
        perror("alloc error controller");
        return NULL;
    }
    *ret = (controller){game,v};
    return ret;
}

/*
* @brief gestion d'un évenement
* @returns bool - false pour quitter le jeu
*/
 bool handle_event(controller *c, Event e) {
        switch (e)
        {
            case E_QUIT:
                return true;
                break;   
            case E_LEFT:
                tetris_move(c->game,-1);
                break;
            case E_RIGHT: 
                tetris_move(c->game,1);
                break;
            case E_ROTATE_ClOCK : 
                tetris_rotate(c->game,1);
                break;
            case E_ROTATE_COUNTER:
                tetris_rotate(c->game,-1);
                break;
            case E_SOFT_DROP:
                tetris_soft_drop(c->game);
                break;
            case E_HARD_DROP: 
                tetris_hard_drop(c->game);
                break;
            case E_HOLD: 
                tetris_hold(c->game);
                break;
            default:
                break;
        }
        return false;
} 

void run(controller *c) {
    assert(c);
    int lines_cleared = 0;
    bool quit = false;
    while (!tetris_game_over(c->game) && !quit) {
        Event e = c->view->getEvent(c->view);
        quit = handle_event(c,e);
        tetris_auto_move(c->game);
        lines_cleared = tetris_check_lines(c->game);
        if (lines_cleared > 0) tetris_adjust_score(c->game, lines_cleared);
        c->view->render_game(c->view,c->game);
    }
    if (tetris_game_over(c->game)) {
        c->view->render_game_over(c->view,c->game);
    }
    c->view->stop(c->view);
}

void destroy_controller(controller *c) {
    assert(c);
    c->game = NULL;
    c->view = NULL;
    free(c);
}
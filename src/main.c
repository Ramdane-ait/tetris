#include <stdio.h>
#include <stdlib.h>

#include "tetris.h"
#include "viewer.h"
#include "controller.h"

int main() {
    tetris_game *game = tetris_create(20 , 10);
    viewer *v = makeViewer("sdl",game);
    controller *control = make_controller(v,game);
    run(control);

    destroy_controller(control);
    tetris_destroy(game);
    v->destroy(v);
    return 0;
}
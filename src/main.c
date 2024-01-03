#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tetris.h"
#include "viewer.h"
#include "controller.h"

int main(int argc, char **argv) {
    if (argc != 2 || (strcmp(argv[1],"sdl") != 0 && strcmp(argv[1],"text") != 0)) {
        fprintf(stderr, "Error when parsing the parameters.\n");
        fprintf(stderr, "[USAGE] %s viewer\n", argv[0]);
        fprintf(stderr, "\t viewer can be \"text\" or \"sdl\"\n");
        return EXIT_FAILURE;
    }
    tetris_game *game = tetris_create(22 , 10);
    if (!game) exit(EXIT_FAILURE);
    viewer *v = makeViewer(argv[1],game);
    if (!v) exit(EXIT_FAILURE);
    controller *control = make_controller(v,game);
    if (!control) exit(EXIT_FAILURE);
    run(control);

    destroy_controller(control);
    v->destroy(v);
    tetris_destroy(game);
    return EXIT_SUCCESS;
}
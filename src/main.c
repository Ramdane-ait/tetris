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
/*     tetris_put(game,game->currentPiece);
    tetris_soft_drop(game);
    tetris_soft_drop(game);
    tetris_soft_drop(game);
    for (int i = 0;i < 22;i++) {
        for (int j = 0; j < 10;j++) {
            printf("%d ",game->grid[i * 10 + j]);
        }
        printf("\n");
    }
    printf("\n\n");
    printf("-1 mod 4 : %d\n\n",-1 % 4);
    tetris_rotate(game,-1);
    tetris_rotate(game,-1);

    for (int i = 0;i < 22;i++) {
        for (int j = 0; j < 10;j++) {
            printf("%d ",game->grid[i * 10 + j]);
        }
        printf("\n");
    } */
    viewer *v = makeViewer(argv[1],game);
    controller *control = make_controller(v,game);
    run(control);

    destroy_controller(control);
    v->destroy(v);
    tetris_destroy(game);
    return 0;
}
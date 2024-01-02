#pragma once

/* #include "tetris.h"

typedef struct event {
    void *e;
    tetris_movement (*getMovement)(struct event *);
    void (*destroy)(struct event *);
} Event;

Event * make_event();  */

typedef enum {
    E_QUIT = -1,
    E_NO_INPUT,
    E_LEFT,
    E_RIGHT,
    E_SOFT_DROP,
    E_HARD_DROP,
    E_ROTATE_ClOCK,
    E_ROTATE_COUNTER,
    E_HOLD,
    E_OTHER
} Event;
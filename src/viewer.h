#pragma once

#include "tetris.h"
#include "event.h"

typedef struct viewer {
  void *data;
  Event (*getEvent)(struct viewer *);
  void (*render_game)(struct viewer *,tetris_game *);
  void (*render_game_over)(struct viewer *,tetris_game *);
  void (*playMusic)(struct viewer *);
  void (*stop)(struct viewer *);
  void (*destroy)(struct viewer *);
} viewer;

/**
 * @brief Construit un viewer et le retourne.
 *
 * @param vtype est le type de viewer qu'on veut utiliser.
 * @param e est le moteur de jeu.
 * @return un pointeur sur un viewer.
 */
viewer *makeViewer(char *vtype, tetris_game *e);
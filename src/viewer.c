#include "viewer.h"

#include <stdlib.h>
#include <string.h>

#include "sdlViewer.h"
#include "textViewer.h"

/**
 * @brief viewer::makeViewer implementation.
 */
viewer *makeViewer(char *vtype, tetris_game *game) {
  viewer *ret = NULL;
  if (!strcmp(vtype, "text")) {
    ret = initTextViewer(game->width,game->height);
  } else if (!strcmp(vtype, "sdl")) {
    ret = initSdlViewer(game->width,game->height);
  }

  return ret;
}  // makeViewer
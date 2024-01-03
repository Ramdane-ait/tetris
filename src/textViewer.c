#include "textViewer.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define COLS_PER_CELL 2
#define ADD_BLOCK(w,x) waddch((w),' '|A_REVERSE|COLOR_PAIR(x));     \
                       waddch((w),' '|A_REVERSE|COLOR_PAIR(x))
#define ADD_EMPTY(w) waddch((w), ' '); waddch((w), ' ')

static void sleep_milli(int milliseconds)
{
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = milliseconds * 1000 * 1000;
  nanosleep(&ts, NULL);
}

viewer *initTextViewer(int width, int height) {
    viewer *ret = (viewer *)malloc(sizeof(viewer));
    if (!ret) {
        perror("initTextViewer viewer");
        return NULL;
    }

    dataTextViewer *data = (dataTextViewer *)malloc(sizeof(dataTextViewer));
    if (!data) {
        perror("initTextViewer data");
        free(ret);
        return NULL;
    }
    
    initscr();             // initialize curses
    cbreak();              // pass key presses to program, but not signals
    noecho();              // don't echo key presses to screen
    keypad(stdscr, TRUE);  // allow arrow keys
    timeout(0);            // no blocking on getch()
    curs_set(0);           // set the cursor to invisible
    if (has_colors() == FALSE) {
        endwin();
        free(data);
        free(ret);
        fprintf(stderr, "Colors not supported\n");
        exit(EXIT_FAILURE);
    }
    start_color();
    init_pair(TET_I, COLOR_CYAN, COLOR_BLACK);
    init_pair(TET_J, COLOR_BLUE, COLOR_BLACK);
    init_pair(TET_L, COLOR_WHITE, COLOR_BLACK);
    init_pair(TET_O, COLOR_YELLOW, COLOR_BLACK);
    init_pair(TET_S, COLOR_GREEN, COLOR_BLACK);
    init_pair(TET_T, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(TET_Z, COLOR_RED, COLOR_BLACK);

    data->grid = newwin(height + 2, 2 * width + 2, 0, 0);
    data->next  = newwin(6, 10, 0, 2 * (width + 1) + 1);
    data->hold  = newwin(6, 10, 7, 2 * (width + 1) + 1);
    data->score = newwin(6, 10, 14, 2 * (width + 1 ) + 1);
    ret->data = data;
    ret->getEvent = getTextEvent;
    ret->render_game = render_game_text;
    ret->render_game_over = draw_game_over_text;
    ret->playMusic = startTextMusic;
    ret->stop = stopTextViewer; 
    ret->destroy = destroyTextViewer;

    return ret;    
}

static void display_grid(WINDOW *w, tetris_game *game) {
    int i, j;
    box(w, 0, 0);
    for (i = 2; i < game->height; i++) {
        wmove(w, 1 + i, 1);
        for (j = 0; j < game->width; j++) {
            if (game->grid[i * game->width + j]) {
                ADD_BLOCK(w,game->grid[i * game->width + j] - 1);
            } else {
                ADD_EMPTY(w);
            }
        }
    }
    wnoutrefresh(w);
}

static void display_piece(WINDOW *w, Tetromino *piece) {
    int b;
    point c;
    wclear(w);
    box(w, 0, 0);
    for (b = 0; b < NUM_TET_BLOCK; b++) {
        c = tetromino_get(piece->type,piece->orientation,b);
        wmove(w, c.row + 1, c.col * COLS_PER_CELL + 1);
        ADD_BLOCK(w, piece->type);
    }
    wnoutrefresh(w);
}

static void display_score(WINDOW *w, tetris_game *game) {
  wclear(w);
  box(w, 0, 0);
  wprintw(w, "Score\n%d\n", game->score);
  wprintw(w, "Level\n%d\n", game->level);
  wprintw(w, "Lines\n%d\n", game->lines_remaining);
  wnoutrefresh(w);
}

void render_game_text(viewer *v,tetris_game *game) {
    dataTextViewer *data = (dataTextViewer *)v->data;
    display_grid(data->grid,game);
    display_piece(data->next,&game->nextPiece);
    display_piece(data->hold,&game->holdPiece);
    display_score(data->score,game);
    doupdate();
    sleep_milli(16);
}


void draw_game_over_text(viewer *v,tetris_game *game) {
    int x,y;
    dataTextViewer *data = (dataTextViewer *)v->data;
    wclear(data->grid);
    box(data->grid, 0, 0);
    getmaxyx(data->grid,y,x);
    mvwprintw(data->grid,y / 2, (x - 9) / 2 ,"GAME OVER");
    wrefresh(data->grid);
    sleep(2);
}

Event getTextEvent(viewer *v) {
    switch (getch()) {
    case KEY_LEFT:
    case 'q':
      return E_LEFT; 
    case KEY_RIGHT:
    case 'd':
      return E_RIGHT;  
    case KEY_UP:
    case 'z':
      return E_ROTATE_ClOCK;  
    case 'a':
      return E_ROTATE_COUNTER;
    case KEY_DOWN:
    case 's':
      return E_SOFT_DROP; 
    case 27:
      return E_QUIT;
    case ' ':
      return E_HARD_DROP;
    case 'c':
      return E_HOLD;
    default:
      return E_OTHER;
    }

}

void startTextMusic(viewer *v) { }
void stopTextViewer(viewer *v) { endwin(); } 

void destroyTextViewer(viewer *v) {
  delwin(((dataTextViewer *)v->data)->grid);
  delwin(((dataTextViewer *)v->data)->next);
  delwin(((dataTextViewer *)v->data)->hold);
  delwin(((dataTextViewer *)v->data)->score);
  free(v->data);
  free(v);
} 
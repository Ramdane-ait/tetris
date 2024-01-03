#include "sdlViewer.h"

typedef enum {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
} text_align;

static const Color block_color[] = {
    { 0x11, 0x11, 0x11, 0x11 },
    { 0x00, 0xFF, 0xFF, 0xFF },
    { 0x00, 0x00, 0xFF, 0xFF },
    { 0xFF, 0x7F, 0x00, 0xFF },
    { 0xFF, 0xFF, 0x00, 0xFF },
    { 0x00, 0xFF, 0x00, 0xFF },
    { 0x80, 0x00, 0x80, 0xFF },
    { 0xFF, 0x00, 0x00, 0xFF }
};

viewer *initSdlViewer(int width , int height) {
    viewer *ret = (viewer *)malloc(sizeof(viewer));
    if (!ret) {
        perror("initSdlViewer viewer");
        return NULL;
    }

    /* Initialise SDL */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Could not initialise SDL: %s\n", SDL_GetError());
        free(ret);
        return NULL;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "Could not initialise TTF: %s\n", TTF_GetError());
        free(ret);
        SDL_Quit();
        return NULL;
    }
    bool music = true;
    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        fprintf(stderr, "Could not initialize SDL_mixer\n");
        music = false;
    }
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
        fprintf(stderr, "Could not initialize audio\n");
        Mix_Quit();
        music = false;
    }
    dataSdlViewer *data = (dataSdlViewer *)malloc(sizeof(dataSdlViewer));
    if (!data) {
        perror("Problem when allocating the data structure.");
        free(ret);
        if (music) {
            Mix_CloseAudio();
            Mix_Quit();
        }
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }

    SDL_CreateWindowAndRenderer((width + 8) * BLOCK_SIZE, (height + 4) * BLOCK_SIZE, SDL_INIT_VIDEO, &data->window,
                                &data->renderer);
    if (!data->window) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        free(data);
        free(ret);
        if (music) {
            Mix_CloseAudio();
            Mix_Quit();
        }
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }
    data->font = TTF_OpenFont(FONT_PATH,32);
    if (!data->font) {
        fprintf(stderr, "Erreur ouverture %s: %s\n", FONT_PATH,TTF_GetError());
        free(data);
        free(ret);
        if (music) {
            Mix_CloseAudio();
            Mix_Quit();
        }
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }
    if (music) {
        Mix_AllocateChannels(1);
        data->sound = Mix_LoadMUS(MUSIC_PATH);
        if (!data->sound) {
            Mix_CloseAudio();
            Mix_Quit();
            music = false;
        }
    }
    data->music = music;
    ret->data = data;
    ret->getEvent = getSdlEvent;
    ret->render_game = render_game_sdl;
    ret->render_game_over = draw_game_over_sdl;
    ret->playMusic = startSdlMusic;
    ret->stop = stopSdlViewer;
    ret->destroy = destroySdlViewer;

    return ret;
}


static void draw_block(SDL_Renderer *renderer, int x, int y, Color color) {

    SDL_Rect outer;
    SDL_Rect inner;

    outer.x = x * BLOCK_SIZE;
    outer.y = y * BLOCK_SIZE;
    outer.w = BLOCK_SIZE;
    outer.h = BLOCK_SIZE;

    inner.x = x * BLOCK_SIZE + 1;
    inner.y = y * BLOCK_SIZE + 1;
    inner.w = BLOCK_SIZE - 2;
    inner.h = BLOCK_SIZE - 2;

    SDL_SetRenderDrawColor(renderer, 0x0c, 0x0c, 0x0c, 0xff);
    SDL_RenderFillRect(renderer, &outer);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &inner);
}

static void draw_string(SDL_Renderer *renderer,TTF_Font *font,char *text,int x, int y,text_align alignment,Color color) {
    SDL_Color sdl_color = { color.r, color.g, color.b, color.a };
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, sdl_color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;
    switch (alignment)
    {
    case TEXT_ALIGN_LEFT:
        rect.x = x;
        break;
    case TEXT_ALIGN_CENTER:
        rect.x = x - surface->w / 2;
        break;
    case TEXT_ALIGN_RIGHT:
        rect.x = x - surface->w;
        break;
    }

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


static void display_grid(SDL_Renderer *renderer, tetris_game *game) {
    unsigned x,y;
    for (y = 2;y < game->height;y++) {
        for (x = 0;x < game->width;x++) {
            draw_block(renderer,x + 1,y,block_color[game->grid[y * game->width + x]]);
        }
    }
}

Event getSdlEvent(viewer *v) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return E_QUIT;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                return E_QUIT;
            case SDLK_LEFT:
            case SDLK_q:
                return E_LEFT;
            case SDLK_RIGHT:
            case SDLK_d:
                return E_RIGHT;
            case SDLK_UP:
            case SDLK_z:
                return E_ROTATE_ClOCK;
            case SDLK_LCTRL:
            case SDLK_RCTRL:
                return E_ROTATE_COUNTER;
            case SDLK_DOWN:
            case SDLK_s:
                return E_SOFT_DROP;
            case SDLK_SPACE:
                return E_HARD_DROP;
            case SDLK_c:
                return E_HOLD;
            }
            return E_OTHER;
        } 
    }
    return E_NO_INPUT;
}

static void draw_piece(SDL_Renderer *renderer, TTF_Font *font,char * text,int x , int y ,Tetromino piece) {
    draw_string(renderer,font,text,x * BLOCK_SIZE,y * BLOCK_SIZE,TEXT_ALIGN_CENTER,(Color){0xcc,0xcc,0xcc,0xcc});
    int new_y = y + 2,new_x = x - 2,i,j;
    for (i = 0;i < NUM_TET_BLOCK;i++) {
        for (j = 0;j < NUM_TET_BLOCK;j++){
            draw_block(renderer,new_x + i,new_y + j,(Color){0x00, 0x00, 0x00, 0x00});
        }
    }
    for (i = 0;i < NUM_TET_BLOCK;i++) {
        point p = tetromino_get(piece.type,piece.orientation,i);
        draw_block(renderer,new_x + p.col,new_y + p.row,block_color[piece.type + 1]);
    }
}

void draw_game_over_sdl(viewer *v,tetris_game *game) {
    dataSdlViewer *data = (dataSdlViewer *)v->data;
    draw_string(data->renderer,data->font,"GAME OVER",(game->width / 2 + 1) * BLOCK_SIZE,game->height / 2 * BLOCK_SIZE,TEXT_ALIGN_CENTER,(Color){0xff,0xff,0xff,0xff});
    SDL_RenderPresent(data->renderer);
    SDL_Delay(2000);
}

void render_game_sdl(viewer *v,tetris_game *game) {
    dataSdlViewer *data = (dataSdlViewer *)v->data;
    char buffer[50];
    SDL_SetRenderDrawColor(data->renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(data->renderer);
    display_grid(data->renderer,game);

    draw_string(data->renderer, data->font, "SCORE", (game->width + 4) * BLOCK_SIZE, BLOCK_SIZE, TEXT_ALIGN_CENTER, (Color){0xcc,0xcc,0xcc,0xcc});
    snprintf(buffer,sizeof(buffer), "%u", game->score);
    draw_string(data->renderer, data->font, buffer, (game->width + 4) * BLOCK_SIZE, BLOCK_SIZE * 2+ 10, TEXT_ALIGN_CENTER, (Color){0xcc,0xcc,0xcc,0xcc});

    draw_string(data->renderer, data->font, "LEVEL", (game->width + 4) * BLOCK_SIZE, BLOCK_SIZE * 4, TEXT_ALIGN_CENTER, (Color){0xcc,0xcc,0xcc,0xcc});
    snprintf(buffer, sizeof(buffer), "%u", game->level);
    draw_string(data->renderer, data->font, buffer, (game->width + 4) * BLOCK_SIZE, BLOCK_SIZE * 5 + 10, TEXT_ALIGN_CENTER, (Color){0xcc,0xcc,0xcc,0xcc});

    draw_string(data->renderer, data->font, "LINES", (game->width + 4) * BLOCK_SIZE, BLOCK_SIZE * 7, TEXT_ALIGN_CENTER, (Color){0xcc,0xcc,0xcc,0xcc});
    snprintf(buffer, sizeof(buffer), "%u", game->lines_remaining);
    draw_string(data->renderer, data->font, buffer, (game->width + 4) * BLOCK_SIZE, BLOCK_SIZE * 8 + 10, TEXT_ALIGN_CENTER, (Color){0xcc,0xcc,0xcc,0xcc});

    draw_piece(data->renderer,data->font,"NEXT",game->width + 4,10,game->nextPiece);
    draw_piece(data->renderer,data->font,"HOLD",game->width + 4,17,game->holdPiece);
    SDL_RenderPresent(data->renderer);
    SDL_Delay(16);
}

void startSdlMusic(viewer *v) {
    dataSdlViewer *data = (dataSdlViewer *)v->data;
    if (data->music) Mix_PlayMusic(data->sound, -1);    
}

void stopSdlViewer(viewer *v) {} 

void destroySdlViewer(viewer *v) {
    dataSdlViewer *data = (dataSdlViewer *)v->data;
    SDL_DestroyRenderer(data->renderer);
    SDL_DestroyWindow(data->window);
    TTF_CloseFont(data->font);
    Mix_FreeMusic(data->sound);
    Mix_CloseAudio();
    free(data);
    free(v);
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}
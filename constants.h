#ifndef CONSTANTS_H
#define CONSTANTS_H

#if defined(_WIN64) || defined(_WIN32)
    #include <SDL.h>
#else
    #include <SDL2/SDL.h>
#endif



const int SCREEN_WIDTH = 880;
const int SCREEN_HEIGHT = 660;
const char WINDOW_TITLE[] = "Snake Game";

const int BOARD_WIDTH = 40;
const int BOARD_HEIGHT = 27;
const int CELL_SIZE = 22;

const SDL_Color BOARD_COLOR = {0, 0, 0};
const SDL_Color LINE_COLOR = {128, 128, 128};
const SDL_Color COLOR_KEY= {255,255,255};

const double STEP_DELAY = 0.15;

#endif

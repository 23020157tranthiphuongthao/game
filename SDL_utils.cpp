#include <iostream>
#if defined(_WIN64) || defined(_WIN32)
    #include <SDL.h>
#else
    #include <SDL2/SDL.h>
#endif


#include "Game.h"
#include "constants.h"
#include "Gallery.h"
#include "Score.h"



//*****************************************************
// Các hàm chung về khởi tạo và huỷ SDL
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer,
	int screenWidth, int screenHeight, const char* windowTitle)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
    //   SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    //Khi thông thường chạy với môi trường bình thường ở nhà
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}



// moved from main.cpp
float generateRandomNumber()
{
    return (float) rand() / RAND_MAX;
}

void renderSplashScreen()
{
    cout << "Press any key to start game" << endl;
    waitUntilKeyPressed();
}

void drawCell(SDL_Renderer* renderer, int left, int top, Position pos, SDL_Texture* texture)
{
	SDL_Rect cell;
	cell.x = left + pos.x * CELL_SIZE ;
	cell.y = top + pos.y * CELL_SIZE ;
	cell.w = CELL_SIZE;
	cell.h = CELL_SIZE;
	SDL_RenderCopy(renderer, texture, NULL, &cell);
}

void drawCherry(SDL_Renderer* renderer, int left, int top, Position pos, Gallery* gallery)
{

    SDL_Texture* texture=gallery->getImage(PIC_CHERRY);
    SDL_Rect cherry;
	cherry.x = left + pos.x * CELL_SIZE ;
	cherry.y = top + pos.y * CELL_SIZE ;
	cherry.w = CELL_SIZE;
	cherry.h = CELL_SIZE;
	SDL_RenderCopy(renderer, texture, NULL, &cherry);
}
void drawBlock(SDL_Renderer* renderer, int left, int top, vector<Position> pos, Gallery* gallery)
{
    for (int is = 0; is <pos.size(); is++) {
        SDL_Texture* texture = gallery->getImage(PIC_BLOCK);
        drawCell(renderer, left, top, pos[is], texture);
        /**Game*game;
        game->squares[pos[is].y][pos[is].x]=CELL_OFF_BOARD;**/
    }
}

void drawSnake(SDL_Renderer* renderer, int left, int top, vector<Position> pos, Gallery* gallery)
{
	// snake's head
	drawCell(renderer, left, top, pos[pos.size()-1], gallery->getImage(PIC_SNAKE_HEAD));

	// snake's body
    for (int i = pos.size() - 2; i >= 0; i--) {
        SDL_Texture* texture = gallery->getImage(
			pos[i].y == pos[i+1].y ? PIC_SNAKE_HORIZONTAL : PIC_SNAKE_VERTICAL);
        drawCell(renderer, left, top, pos[i], texture);

    }
}

/**void drawVerticalLine(SDL_Renderer* renderer, int left, int top, int cells)
{
    SDL_SetRenderDrawColor(renderer, LINE_COLOR.r, LINE_COLOR.g, LINE_COLOR.b, 0);
    SDL_RenderDrawLine(renderer, left, top, left, top + cells * CELL_SIZE);
}

void drawHorizontalLine(SDL_Renderer* renderer, int left, int top, int cells)
{
    SDL_SetRenderDrawColor(renderer, LINE_COLOR.r, LINE_COLOR.g, LINE_COLOR.b, 0);
    SDL_RenderDrawLine(renderer, left, top, left + cells * CELL_SIZE, top);
}**/

void renderGamePlay(SDL_Renderer* renderer, const Game& game, Gallery* gallery)
{
    int top = 0, left = 0;
    /**Score scr(BOARD_WIDTH);
    scr.render(renderer, 0, 0);
    scr.render_number(renderer, BOARD_WIDTH * 3, 0, game.getScore());**/
    SDL_Texture* background =gallery-> loadTexture("backgroundplay.png");
    SDL_RenderCopy( renderer, background, NULL, NULL);
    drawBlock(renderer,left,top,game.getBlockPositions(),gallery);
    Score scr(BOARD_WIDTH);
    scr.render(renderer, 350 ,615);
    scr.render_number(renderer, 480 , 615, game.getScore());


        /**drawVerticalLine(renderer, left + 0*CELL_SIZE, top, BOARD_HEIGHT);
        drawVerticalLine(renderer, left + (BOARD_WIDTH)*CELL_SIZE, top, BOARD_HEIGHT);

        drawHorizontalLine(renderer, left, top+ 0 * CELL_SIZE, BOARD_WIDTH);
        drawHorizontalLine(renderer, left, top+(BOARD_HEIGHT) * CELL_SIZE, BOARD_WIDTH);**/

    drawCherry(renderer, left, top, game.getCherryPosition(), gallery);
    drawSnake(renderer, left, top, game.getSnakePositions(), gallery);

    SDL_RenderPresent(renderer);
}

void renderGameOver(SDL_Renderer* renderer, const Game& game)
{

}

void interpretEvent(SDL_Event e, Game& game)
{
    if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
        	case SDLK_UP: game.processUserInput(UP); break;
        	case SDLK_DOWN: game.processUserInput(DOWN); break;
        	case SDLK_LEFT: game.processUserInput(LEFT); break;
        	case SDLK_RIGHT: game.processUserInput(RIGHT); break;
        }
    }
}

void updateRankingTable(const Game& game)
{

}

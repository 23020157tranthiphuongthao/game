#ifndef SCORE_H
#define SCORE_H
#include "constants.h"
#include<iostream>
#define src_score "score.png"

class Score
{
    public:
        Score(int sz_);
        ~Score();

        bool LoadImage(std::string path, SDL_Renderer* screen);

        void render(SDL_Renderer* screen, int x, int y);

        void render_number(SDL_Renderer* screen, int x, int y, int score);

    private:
        int sz;
        SDL_Texture* Tex;
        int n_width, n_height;
};

#endif // SCORE_H

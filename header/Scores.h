#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "SoundManager.h"

class Scores{
public:
    Scores(SDL_Renderer* renderer, Sound* soundManager);
    ~Scores();

    void update(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void reset(SDL_Renderer* renderer);

private:
    TTF_Font* font;
    SDL_Texture* scoreTexture;      // current score
    SDL_Texture* scoreTextTexture;     // "Score" label
    
    // rect positions for rendering
    SDL_Rect scoreRect;
    SDL_Rect scoreTextRect;

    Sound* sound;
    int scores;            // current score
    int scoresIndex;       // animation counter
    int scoresInital;      // base score
};
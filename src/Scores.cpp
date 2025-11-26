#include "Scores.h"
#include "Constants.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace std;
Scores::Scores(SDL_Renderer* renderer, Sound* soundManager):font(nullptr), sound(soundManager),scores(0), scoresIndex(0), scoresInital(0){
    // load font
    font = TTF_OpenFont("assets/Fonts/Font.ttf", 20);
    // load score textures
    update(renderer);
}

Scores::~Scores() {
    if (font) TTF_CloseFont(font);
}

void Scores::update(SDL_Renderer* renderer) {
    if (!playerDead) {
        // increase score counter
        scoresIndex++;
        // update score every 5 frames
        if (scoresIndex >= 5) {
            scores++;
            scoresIndex = 0;
            // point sound when over 100 points
            if (scores % 100 == 0) {
                sound->playPointSound();
                // update speed when over 100 points
                if (gameSpeed < 10) {
                    gameSpeed++;
                }
            }
        }
    }
    
    //score text
    SDL_Color textColor = {83, 83, 83, 255}; //gray 
    
    // current score texture
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, to_string(scores).c_str(), textColor);
    if (scoreSurface) {
        scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
        
        // Position in top right
        scoreRect.x = windowSize_x - scoreSurface->w - 20;
        scoreRect.y = 20;
        scoreRect.w = scoreSurface->w;
        scoreRect.h = scoreSurface->h;
        
        SDL_FreeSurface(scoreSurface);
    }
    // "SCORE" text
    SDL_Surface* scoreTextSurface = TTF_RenderText_Solid(font, "SCORE", textColor);
    if (scoreTextSurface) {
        scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreTextSurface);
        
        scoreTextRect.x = scoreRect.x - scoreTextSurface->w - 30;
        scoreTextRect.y = 20;
        scoreTextRect.w = scoreTextSurface->w;
        scoreTextRect.h = scoreTextSurface->h;
        
        SDL_FreeSurface(scoreTextSurface);
    }   
}

void Scores::render(SDL_Renderer* renderer) {
    // current score
    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);    
    // score label and value
    SDL_RenderCopy(renderer, scoreTextTexture, nullptr, &scoreTextRect);

}

void Scores::reset(SDL_Renderer* renderer) {
    // reset current score
    scores = 0;
    scoresIndex = 0;
    scoresInital = 0;
    
    // reset game speed
    gameSpeed = 4;
    
    // update score display
    update(renderer);
}
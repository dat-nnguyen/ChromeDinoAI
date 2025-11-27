#include "Scores.h"
#include "Constants.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

using namespace std;

Scores::Scores(SDL_Renderer* renderer, Sound* soundManager)
    : font(nullptr), sound(soundManager), scores(0), scoresIndex(0), scoresInital(0),
      scoreTexture(nullptr), scoreTextTexture(nullptr)
{
    // load font
    font = TTF_OpenFont("assets/Fonts/Font.ttf", 20);
    if (!font) {
        cout << "Failed to load font: " << TTF_GetError() << endl;
    }
    // Initial render
    update(renderer);
}

Scores::~Scores() {
    if (font) TTF_CloseFont(font);
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    if (scoreTextTexture) SDL_DestroyTexture(scoreTextTexture);
}

void Scores::update(SDL_Renderer* renderer) {
    scoresIndex++;

    // update score every 5 frames
    if (scoresIndex >= 5) {
        scores++;
        scoresIndex = 0;

        // point sound when over 100 points
        if (scores % 100 == 0) {
            if(sound) sound->playPointSound();

            // update speed when over 100 points
            // gameSpeed is a float now, so we can increment slowly or by whole numbers
            if (gameSpeed < 15.0f) {
                gameSpeed += 0.5f;
            }
        }
    }

    // --- TEXT RENDERING ---

    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    if (scoreTextTexture) SDL_DestroyTexture(scoreTextTexture);

    SDL_Color textColor = {83, 83, 83, 255}; // Gray

    // 1. Current Score Number
    // Check if font exists to avoid crash
    if (font) {
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

        // 2. "SCORE" Label
        SDL_Surface* scoreTextSurface = TTF_RenderText_Solid(font, "SCORE", textColor);
        if (scoreTextSurface) {
            scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreTextSurface);

            scoreTextRect.x = scoreRect.x - scoreTextSurface->w - 15; // Adjusted spacing
            scoreTextRect.y = 20;
            scoreTextRect.w = scoreTextSurface->w;
            scoreTextRect.h = scoreTextSurface->h;

            SDL_FreeSurface(scoreTextSurface);
        }
    }
}

void Scores::render(SDL_Renderer* renderer) {
    if (scoreTexture)
        SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);

    if (scoreTextTexture)
        SDL_RenderCopy(renderer, scoreTextTexture, nullptr, &scoreTextRect);
}

void Scores::reset(SDL_Renderer* renderer) {
    // Reset values for the new generation
    scores = 0;
    scoresIndex = 0;
    scoresInital = 0;

    // Reset game speed (IMPORTANT for AI learning)
    gameSpeed = 5.0f; // Match the starting speed in main.cpp

    // Update display immediately
    update(renderer);
}
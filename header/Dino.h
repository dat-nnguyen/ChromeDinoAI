#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "Constants.h"
#include "Brain.h"
#include "SoundManager.h"



struct Obstacle;
class Dino {
public:
    Dino(SDL_Renderer* renderer, Sound* soundManager);
    ~Dino();
    void update(Uint32 currentTime, std::vector<Obstacle>& obstacles);
    void render(SDL_Renderer* renderer);
    void reset();
    // --- AI ADDITIONS ---

    Brain* brain;
    float fitness;
    bool playerDead;
    // AI Input/Output functions

    void think(float distance, float width, float height, float speed);
    // Action functions (The brain calls these)
    void Jump();
    void Duck(bool isDucking);



private:
    SDL_Texture* dinoTexture;
    SDL_Rect srcRect, destRect, collisionRect;
    std::vector<SDL_Rect> frames;
    float posY;
    float velocityY;
    int animationCounter;
    Sound* sound;
    Uint32 deathTime;
    void walk();

};
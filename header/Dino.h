#pragma once
#include <SDL2/SDL.h>
#include <array>
#include <iostream>
#include <vector>
#include "SoundManager.h"
#include "Constants.h"
using namespace std;

class Obstacle;

const int FRAME_WIDTH = 90;
const int FRAME_HEIGHT = 95;

class Dino {
public:
    SDL_Texture* dinoTexture;
    SDL_Rect srcRect;      // source rect for sprite sheet
    SDL_Rect destRect;     // destination rect for render
    SDL_Rect collisionRect; // collision box
    
    array<SDL_Rect, 6> frames;  // frames
    int animationCounter;
    
    float posY;        // Y position
    float velocityY;   // velocity for jump 
    
    Sound* sound;      // sound manager
    Uint32 deathTime;  // time when die
    
    Dino(SDL_Renderer* renderer, Sound* soundManager);
    ~Dino();
    
    void update(Uint32 currentTime, vector<Obstacle>& obstacles);
    void render(SDL_Renderer* renderer);
    void walk();
    void reset();
};
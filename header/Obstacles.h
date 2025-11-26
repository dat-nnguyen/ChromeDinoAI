#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Constants.h"
using namespace std;
class Obstacle{
public:
    SDL_Texture* texture;
    SDL_Rect srcRect;      // source rect
    SDL_Rect destRect;     // destination rect
    SDL_Rect collisionRect; // collision box

    Obstacle(SDL_Texture* tex, SDL_Renderer* renderer);
};

class Obstacles{
public:
    vector<Obstacle> obstacles;
    Uint32 lastSpawnTime;
    SDL_Texture* obstacleTexture_1; // small cactus
    SDL_Texture* obstacleTexture_2; // medium cactus
    SDL_Texture* obstacleTexture_3; // large cactus
    int randomNumber;

    Obstacles(SDL_Renderer* renderer);
    ~Obstacles();
    void update(Uint32 currentTime);
    void render(SDL_Renderer* renderer);
    void reset();
};
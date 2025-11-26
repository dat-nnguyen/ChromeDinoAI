#include "Obstacles.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <algorithm>
using namespace std;
Obstacle::Obstacle(SDL_Texture* tex, SDL_Renderer* renderer):texture(tex) {
    // obstacle's source rectangle
    srcRect.x = 0;
    srcRect.y = 0;
    SDL_QueryTexture(tex, nullptr, nullptr, &srcRect.w, &srcRect.h);
    
    //obstacle positon
    destRect.x = windowSize_x;
    destRect.y = groundOffset;
    destRect.w = srcRect.w;
    destRect.h = srcRect.h;
    
    // collision rect
    collisionRect = destRect;
    collisionRect.w -= 10;
}

Obstacles::Obstacles(SDL_Renderer* renderer) : lastSpawnTime(0) {
    //random seed
    srand(static_cast<unsigned int>(time(nullptr)));
    
    obstacleTexture_1 = IMG_LoadTexture(renderer, "assets/Images/Cactus1.png");
    obstacleTexture_2 = IMG_LoadTexture(renderer, "assets/Images/Cactus2.png");
    obstacleTexture_3 = IMG_LoadTexture(renderer, "assets/Images/Cactus3.png");
}

Obstacles::~Obstacles() {
    SDL_DestroyTexture(obstacleTexture_1);
    SDL_DestroyTexture(obstacleTexture_2);
    SDL_DestroyTexture(obstacleTexture_3);
}

void Obstacles::update(Uint32 currentTime){
    Uint32 elapsedTime = currentTime - lastSpawnTime;

    // maintain chance of spawning obstacles
    if (elapsedTime > (1000 + gameSpeed * 100)) {  
        randomNumber = (rand() % 100);
        
        if (randomNumber < 40) {  // 40% chance for small cactus
            obstacles.emplace_back(obstacleTexture_1, nullptr);
        } else if (randomNumber < 70) {  // 30% chance for medium cactus
            obstacles.emplace_back(obstacleTexture_2, nullptr);
        } else {  // 30% chance for large cactus
            obstacles.emplace_back(obstacleTexture_3, nullptr);
        }
        lastSpawnTime = currentTime;
    }
    
    if (!playerDead) {
        for (int i = 0; i < obstacles.size(); i++) {
            // update obstacle position and collision box
            obstacles[i].destRect.x -= gameSpeed;
            obstacles[i].collisionRect = obstacles[i].destRect;
            obstacles[i].collisionRect.w -= 10;
            
            // remove obstacles out off screen
            if (obstacles[i].destRect.x < -150) {
                obstacles.erase(obstacles.begin() + i);
                i--; // adjust index after erasing
            }
        }
    }
}

void Obstacles::render(SDL_Renderer* renderer) {
    for (auto& obstacle : obstacles) {
        SDL_RenderCopy(renderer, obstacle.texture, &obstacle.srcRect, &obstacle.destRect);
    }
}

void Obstacles::reset() {
    obstacles.clear();
}
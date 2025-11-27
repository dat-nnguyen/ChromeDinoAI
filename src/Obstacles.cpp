#include "Obstacles.h"
#include "Constants.h" // Ensure this contains windowSize_x and gameSpeed
#include <SDL2/SDL_image.h>
#include <iostream>
#include <algorithm>
#include <ctime>       // for time()

using namespace std;

// --- SINGLE OBSTACLE CONSTRUCTOR ---
Obstacle::Obstacle(SDL_Texture* tex, SDL_Renderer* renderer) : texture(tex) {
    // Set up source rect based on texture size
    srcRect.x = 0;
    srcRect.y = 0;
    SDL_QueryTexture(tex, nullptr, nullptr, &srcRect.w, &srcRect.h);


    destRect.x = windowSize_x;
    destRect.y = windowSize_y - 150.0f; // Adjusted to match your Dino ground level


    destRect.w = srcRect.w;
    destRect.h = srcRect.h;

    // Set up collision rect (slightly smaller than texture)
    collisionRect = destRect;
    collisionRect.x += 5;
    collisionRect.w -= 10;
    collisionRect.y += 5;
    collisionRect.h -= 10;
}

// --- OBSTACLES MANAGER CONSTRUCTOR ---
Obstacles::Obstacles(SDL_Renderer* renderer) : lastSpawnTime(0) {
    // Seed random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Load textures
    obstacleTexture_1 = IMG_LoadTexture(renderer, "assets/Images/Cactus1.png");
    obstacleTexture_2 = IMG_LoadTexture(renderer, "assets/Images/Cactus2.png");
    obstacleTexture_3 = IMG_LoadTexture(renderer, "assets/Images/Cactus3.png");
}

Obstacles::~Obstacles() {
    SDL_DestroyTexture(obstacleTexture_1);
    SDL_DestroyTexture(obstacleTexture_2);
    SDL_DestroyTexture(obstacleTexture_3);
}

// --- UPDATE LOGIC ---
void Obstacles::update(Uint32 currentTime) {
    Uint32 elapsedTime = currentTime - lastSpawnTime;

    // 1. SPAWN LOGIC

    if (elapsedTime > (1200 + (20000 / (gameSpeed + 1)))) {
        int randomNumber = (rand() % 100);

        if (randomNumber < 40) {
            obstacles.emplace_back(obstacleTexture_1, nullptr);
        } else if (randomNumber < 70) {
            obstacles.emplace_back(obstacleTexture_2, nullptr);
        } else {
            obstacles.emplace_back(obstacleTexture_3, nullptr);
        }
        lastSpawnTime = currentTime;
    }

    // 2. MOVEMENT LOGIC

    for (int i = 0; i < obstacles.size(); i++) {
        // Move Left
        obstacles[i].destRect.x -= (int)gameSpeed;

        // Update Collision Box
        obstacles[i].collisionRect.x = obstacles[i].destRect.x + 5;
        obstacles[i].collisionRect.y = obstacles[i].destRect.y + 5;

        if (obstacles[i].destRect.x + obstacles[i].destRect.w < 0) {
            obstacles.erase(obstacles.begin() + i);
            i--;
        }
    }
}

// --- RENDER ---
void Obstacles::render(SDL_Renderer* renderer) {
    for (auto& obstacle : obstacles) {
        SDL_RenderCopy(renderer, obstacle.texture, &obstacle.srcRect, &obstacle.destRect);

    }
}

// --- RESET (For Next Generation) ---
void Obstacles::reset() {
    obstacles.clear();
    lastSpawnTime = SDL_GetTicks();
}

// --- GETTER (Needed for Dino Collision) ---
std::vector<Obstacle>& Obstacles::getObstaclesVector() {
    return obstacles;
}

// ==========================================
//           AI SENSOR FUNCTIONS
// ==========================================

// 1. Distance to the closest obstacle in front of us
float Obstacles::getDistanceToNextObstacle(float dinoXPosition) {
    for (auto& obs : obstacles) {
        // If obstacle is to the right of dino
        if (obs.destRect.x > dinoXPosition) {
            return (float)(obs.destRect.x - dinoXPosition);
        }
    }
    // If no obstacles, return a safe "far away" distance
    return 2000.0f;
}

// 2. Width of that closest obstacle
float Obstacles::getNextObstacleWidth(float dinoXPosition) {
    for (auto& obs : obstacles) {
        if (obs.destRect.x > dinoXPosition) {
            return (float)obs.destRect.w;
        }
    }
    return 0.0f;
}

// 3. Y-Position (Height) of that closest obstacle
float Obstacles::getNextObstacleHeight(float dinoXPosition) {
    for (auto& obs : obstacles) {
        if (obs.destRect.x > dinoXPosition) {
            return (float)obs.destRect.y;
        }
    }
    return 0.0f; // Return 0 if nothing is there
}
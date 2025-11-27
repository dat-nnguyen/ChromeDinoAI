#include "Dino.h"
#include "Obstacles.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Constants.h"
using namespace std;

// --- CONSTRUCTOR ---
Dino::Dino(SDL_Renderer* renderer, Sound* soundManager)
    : animationCounter(0), posY(windowSize_y - 150.0f), velocityY(0), sound(soundManager), deathTime(0)
{
    // 1. Initialize the Brain
    brain = new Brain(4, 6, 2); // 4 Inputs, 6 Hidden, 2 Outputs
    fitness = 0;
    playerDead = false;
    dinoTexture = IMG_LoadTexture(renderer, "assets/Images/PlayerSpriteSheet.png");

    // --- DEBUG CHECK ---
    if (!dinoTexture) {
        std::cout << "ERROR: Could not load Dino texture! SDL Error: "
                  << SDL_GetError() << std::endl;
    } else {
        std::cout << "SUCCESS: Dino texture loaded." << std::endl;
    }

    frames.resize(6);

    if (dinoTexture) {
        for (size_t i = 0; i < frames.size(); i++) {
            frames[i].x = i * FRAME_WIDTH;
            frames[i].y = 0;
            frames[i].w = FRAME_WIDTH;
            frames[i].h = FRAME_HEIGHT;
        }
        srcRect = frames[0];
        destRect.x = windowSize_x / 2 - windowSize_x / 4;
        destRect.y = static_cast<int>(posY);
        destRect.w = FRAME_WIDTH;
        destRect.h = FRAME_HEIGHT;

        collisionRect = destRect;
        collisionRect.h -= 15;
        collisionRect.w -= 10;
    }
}

// --- DESTRUCTOR ---
Dino::~Dino() {
    SDL_DestroyTexture(dinoTexture);
    delete brain;
}

// --- AI BRAIN ---
void Dino::think(float distance, float width, float height, float speed) {
    if (playerDead) return;

    // 1. Normalize inputs (0.0 to 1.0)
    std::vector<float> inputs = {
        distance / 800.0f,
        width / 100.0f,
        height / 100.0f,
        speed / 20.0f
    };

    // 2. Ask the brain
    std::vector<float> results = brain->feedForward(inputs);

    // 3. Output 0: Jump
    if (results[0] > 0.5f) {
        Jump();
    }

    // 4. Output 1: Duck
    if (results[1] > 0.5f) {
        Duck(true);
    } else {
        Duck(false);
    }
}

// --- ACTIONS ---
void Dino::Jump() {
    bool onGround = (posY >= windowSize_y - 150.0f);

    // Only jump if on the ground
    if (onGround) {
        animationCounter = 0;
        velocityY = JUMP_VELOCITY;
        srcRect = frames[1];
        sound->playJumpSound();
    }
}

void Dino::Duck(bool isDucking) {

    if (isDucking) {
        collisionRect.h = FRAME_HEIGHT / 2;

    } else {
        collisionRect.h = destRect.h - 15; // Reset hitbox
    }
}

// --- GAME LOOP / PHYSICS ---
void Dino::update(Uint32 currentTime, vector<Obstacle>& obstacles) {

    // 1. Update Physics (Gravity)
    destRect.y = static_cast<int>(posY);
    collisionRect.x = destRect.x;
    collisionRect.y = destRect.y;

    if (!playerDead) {
        walk(); // Run animation

        // Gravity Application
        velocityY += GRAVITY;
        posY += velocityY;

        // Ground Collision
        if (posY >= windowSize_y - 150.0f) {
            posY = windowSize_y - 150.0f;
            velocityY = 0.0f;
        }

        // Falling Frame
        if (posY < windowSize_y - 150.0f) {
            srcRect = frames[1]; // Jump frame
        }

    }
    else {
        // Death Logic
        velocityY = 0.0f;
        srcRect = frames[3]; // Death frame

        // Play sound once
        bool hasPlayedDeathSound = (currentTime - deathTime > 0); // Simplified check
        if (!hasPlayedDeathSound) {
            // sound->playDieSound(); // Logic needs a boolean flag in header to work perfectly
        }
    }

    // 2. Check Collision with Obstacles
    for (auto& obs : obstacles) {
        SDL_Rect result;
        if (SDL_IntersectRect(&collisionRect, &obs.collisionRect, &result)) {
            if (!playerDead) {
                playerDead = true;
                deathTime = currentTime;

                fitness = (float)currentTime;
            }
            break;
        }
    }
}

void Dino::walk() {
    if(frames.empty()) return; // Safety check

    // Simple animation loop
    animationCounter++;
    int frameIndex = (animationCounter / 5) % 2; // Switch every 5 frames between 0 and 1

    if (frameIndex == 0) srcRect = frames[2]; // Example walk frame 1
    else srcRect = frames[3]; // Example walk frame 2
}

void Dino::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, dinoTexture, &srcRect, &destRect);

}

void Dino::reset() {
    velocityY = 0;
    posY = windowSize_y - 150.0f;
    srcRect = frames[0];
    playerDead = false;
    fitness = 0;
}
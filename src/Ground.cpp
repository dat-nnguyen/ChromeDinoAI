#include "Ground.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Constants.h"

Ground::Ground(SDL_Renderer* renderer) : offset(0) {
    groundTexture = IMG_LoadTexture(renderer, "assets/Images/GroundImage.png");
    
    if (groundTexture) {
        SDL_QueryTexture(groundTexture, nullptr, nullptr, &textureWidth, &srcRect.h);
        
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = windowSize_x; // Width of the screen

        destRect.x = 0;
        destRect.y = windowSize_y - 70;
        destRect.w = windowSize_x;
        destRect.h = srcRect.h;
    }
}

Ground::~Ground() {
    SDL_DestroyTexture(groundTexture);
}

void Ground::update() {
    srcRect.x += (int)gameSpeed;


    if (srcRect.x >= textureWidth - windowSize_x) {
        srcRect.x = 0;
    }
}

void Ground::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, groundTexture, &srcRect, &destRect);
}

void Ground::reset() {
    srcRect.x = 0;
}
#pragma once
#include <SDL2/SDL.h>
#include "Constants.h"

class Ground {
public:
    SDL_Texture* groundTexture;
    SDL_Rect srcRect;  // source rect for ground texture
    SDL_Rect destRect; // destination rect for rendering
    int textureWidth; 
    int offset; // offset for scrolling

    Ground(SDL_Renderer* renderer);
    ~Ground();
    void updateGround();
    void render(SDL_Renderer* renderer);
    void reset();
};
// header/Ground.h
#pragma once
#include <SDL2/SDL.h>

class Ground {
public:
    Ground(SDL_Renderer* renderer);
    ~Ground();

    void update();
    void render(SDL_Renderer* renderer);
    void reset();

private:
    SDL_Texture* groundTexture;
    SDL_Rect srcRect, destRect;
    int textureWidth;
    int offset;
};
#include "RestartButton.h"

RestartButton::RestartButton(SDL_Renderer* renderer){
    SDL_Surface* surface = IMG_Load("assets/Images/RestartButton.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    destRect = {350, 200, 100, 40};
    visible = false;
}

RestartButton::~RestartButton(){
    SDL_DestroyTexture(texture);
}

void RestartButton::render(SDL_Renderer* renderer) const{
    if (visible)
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

bool RestartButton::isClicked(const int x, const int y) const{
    return visible &&
        x >= destRect.x && x <= destRect.x + destRect.w &&
        y >= destRect.y && y <= destRect.y + destRect.h;
}

void RestartButton::show(bool value){
    visible = value;
}

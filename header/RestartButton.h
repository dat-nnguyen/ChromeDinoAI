#ifndef RESTARTBUTTON_H
#define RESTARTBUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class RestartButton{
public:
    RestartButton(SDL_Renderer* renderer);
    ~RestartButton();
    void render(SDL_Renderer* renderer) const;
    bool isClicked(int x, int y) const;
    void show(bool value);
    bool visible;

private:
    SDL_Texture* texture;
    SDL_Rect destRect;
};

#endif

#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>

class Sound{
public:
    Mix_Chunk* dieSound;
    Mix_Chunk* jumpSound;
    Mix_Chunk* pointSound;

    Sound();
    ~Sound();
    
    void playDieSound();
    void playJumpSound();
    void playPointSound();
};
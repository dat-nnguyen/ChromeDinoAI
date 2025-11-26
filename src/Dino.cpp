#include "Dino.h"
#include "Obstacles.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Constants.h"
#include "SoundManager.h"
using namespace std;
Dino::Dino(SDL_Renderer* renderer, Sound* soundManager):animationCounter(0), posY(windowSize_y - 150.0f), velocityY(0), sound(soundManager), deathTime(0) {
    dinoTexture = IMG_LoadTexture(renderer, "assets/Images/PlayerSpriteSheet.png");
    if (dinoTexture) {
        // set upframes from sprite 
        for (size_t i = 0; i < frames.size(); i++) {
            frames[i].x = i * FRAME_WIDTH;
            frames[i].y = 0;
            frames[i].w = FRAME_WIDTH;
            frames[i].h = FRAME_HEIGHT;
        }
        
        // initialize source rect to first frame
        srcRect = frames[0];
        
        // set up destination rect
        destRect.x = windowSize_x / 2 - windowSize_x / 4;
        destRect.y = posY;
        destRect.w = FRAME_WIDTH;
        destRect.h = FRAME_HEIGHT;
        
        // set up collision rect 
        collisionRect = destRect;
        collisionRect.h -= 15;
        collisionRect.w -= 10;
    }
}

Dino::~Dino() {
    SDL_DestroyTexture(dinoTexture);
}

void Dino::update(Uint32 currentTime, vector<Obstacle>& obstacles) {
  // update vertical position and collision box
  destRect.y = static_cast<int>(posY);
  collisionRect.x = destRect.x;
  collisionRect.y = destRect.y;
  bool hasPlayedDeathSound = false;
  // check collision
  for (auto& obs : obstacles) {
      SDL_Rect result;
      if (SDL_IntersectRect(&collisionRect, &obs.collisionRect, &result)) {
          if (!playerDead) {
              playerDead = true;
              deathTime = currentTime;
              hasPlayedDeathSound = false;
          }
          break;
      }
  }

  if (!playerDead) {
      walk();  //  walking animation
      const Uint8* state = SDL_GetKeyboardState(nullptr);
      // jump input
      bool onGround = posY >= windowSize_y - 150.0f;
      bool jumping = state[SDL_SCANCODE_SPACE];

      if (jumping && onGround) {
        animationCounter = 0;
        velocityY = JUMP_VELOCITY;
        srcRect = frames[1]; // jump frame
        sound->playJumpSound();
    }
      //gravity
        velocityY += GRAVITY;

      //velocity, update position with velocity
        posY += velocityY;

      // touch ground
      if (posY >= windowSize_y - 150.0f) {
        posY = windowSize_y - 150.0f;
        velocityY = 0.0f;
      }

      // choose frame
      if (!onGround) {
        srcRect = frames[1];
        }


  } else {
      // death state
      velocityY = 0.0f;
      srcRect = frames[3]; // death frame

      if (!hasPlayedDeathSound && currentTime - deathTime < 170) {
          sound->playDieSound();
          hasPlayedDeathSound = true;
      }
  }
}


void Dino::walk() {
    for (int i = 0; i < frames.size() - 3; i++) {
        if (animationCounter == (i + 1)* 5) {
            srcRect = frames[i];
        }
    }
    
    if (animationCounter >= (frames.size() - 2) * 5) {
        animationCounter = 0;
    }
    
    animationCounter++;
}

void Dino::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, dinoTexture, &srcRect, &destRect);
}

void Dino::reset() {
  velocityY = 0;
  posY = groundOffset;
  srcRect = frames[0];
}


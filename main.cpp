//
// Created by Nguyen Tuan Dat on 27/11/25.
//

#include "main.h"#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <algorithm>
#include <iostream>

// Include your custom headers
#include "Constants.h"
#include "Dino.h"
#include "Obstacles.h"
#include "Brain.h"
#include "SoundManager.h"
#include "Ground.h"
// --- AI SETTINGS ---
const int POPULATION_SIZE = 50;
const float MUTATION_RATE = 0.10f; // 10% chance a brain connection changes

// --- AI GLOBALS ---
std::vector<Dino*> population;    // The current generation
std::vector<Dino*> savedDinos;    // The graveyard (used for evolution)
int generation = 1;
int aliveCount = 0;
float highScore = 0;
float currentScore = 0;           // Track time/distance as score

// --- FRAME RATE CONTROL ---
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

// --- EVOLUTION FUNCTION ---
void nextGeneration(SDL_Renderer* renderer, Sound* soundManager) {
    // 1. Sort dead dinos by fitness (Best performance first)
    // The one who lived the longest is at index 0
    std::sort(savedDinos.begin(), savedDinos.end(),
        [](Dino* a, Dino* b) {
            return a->fitness > b->fitness;
        });

    Dino* bestDino = savedDinos[0];

    // Log progress to console
    std::cout << "Generation " << generation << " ended."
              << " Best Fitness: " << bestDino->fitness
              << " High Score: " << highScore << std::endl;

    if (bestDino->fitness > highScore) {
        highScore = bestDino->fitness;
    }

    population.clear();

    // 3. Breed new population
    for (int i = 0; i < POPULATION_SIZE; i++) {
        // Create a brand new Dino
        Dino* newDino = new Dino(renderer, soundManager);

        // Copy the brain from the Champion (BestDino)
        *newDino->brain = bestDino->brain->clone();

        // 4. Mutate
        if (i != 0) {
            newDino->brain->mutate(MUTATION_RATE);
        }

        population.push_back(newDino);
    }

    // 5. Cleanup Memory (Delete the old dead dinos)
    for (Dino* d : savedDinos) {
        delete d; // This calls ~Dino(), which deletes the Brain
    }
    savedDinos.clear();

    generation++;
    currentScore = 0; // Reset score for the new run
}

// --- MAIN ---
int main(int argc, char* argv[]) {
    // 1. SDL Initialization
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL Init failed: " << SDL_GetError() << std::endl;
        return -1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "IMG Init failed: " << IMG_GetError() << std::endl;
        return -1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Mixer Init failed: " << Mix_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Dino AI - Genetic Algorithm",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        windowSize_x, windowSize_y, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 2. Game Objects Setup
    Sound* soundManager = new Sound();
    Obstacles obstacles(renderer);
    Ground ground(renderer);
    // 3. Create Initial Population (Random Brains)
    for (int i = 0; i < POPULATION_SIZE; i++) {
        population.push_back(new Dino(renderer, soundManager));
    }

    bool isRunning = true;
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;
    // --- GAME LOOP ---
    while (isRunning) {
        frameStart = SDL_GetTicks();

        // A. Event Polling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        // B. Game Logic Updates
        currentScore += 0.1f;
        obstacles.update(SDL_GetTicks());
        ground.update();
        // C. AI Vision (Get Data)
        float dist = obstacles.getDistanceToNextObstacle(100.0f);
        float width = obstacles.getNextObstacleWidth(100.0f);
        float height = obstacles.getNextObstacleHeight(100.0f);
        float speed = 5.0f;

        // D. Update Population
        aliveCount = 0;

        for (int i = 0; i < population.size(); i++) {
            Dino* dino = population[i];

            if (!dino->playerDead) {
                // 1. AI Thinks
                dino->think(dist, width, height, speed);

                // 2. Physics & Animation
                dino->update(SDL_GetTicks(), obstacles.getObstaclesVector());

                // 3. Check Life Status
                if (dino->playerDead) {
                    dino->fitness = currentScore;
                    savedDinos.push_back(dino);
                } else {
                    aliveCount++;
                }
            }
        }

        // E. Evolution Trigger
        if (aliveCount == 0) {
            obstacles.reset();
            nextGeneration(renderer, soundManager);
        }

        // F. Rendering
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);


        obstacles.render(renderer);
        ground.render(renderer);
        // Draw Dinos
        for (Dino* dino : population) {
            if (!dino->playerDead) {
                dino->render(renderer);
            }
        }

        SDL_RenderPresent(renderer);

        // G. Frame Capping
        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    // --- CLEANUP ---
    for (Dino* d : population) delete d;
    for (Dino* d : savedDinos) delete d;
    delete soundManager;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
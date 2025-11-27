// In Obstacles.h
#include <vector>
#include <SDL2/SDL.h>


struct Obstacle {
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect, collisionRect;
    Obstacle(SDL_Texture* tex, SDL_Renderer* renderer);
};

class Obstacles {
public:
    Obstacles(SDL_Renderer* renderer);
    ~Obstacles();

    void update(Uint32 currentTime);
    void render(SDL_Renderer* renderer);
    void reset();

    // AI Helpers
    std::vector<Obstacle>& getObstaclesVector();
    float getDistanceToNextObstacle(float dinoXPosition);
    float getNextObstacleWidth(float dinoXPosition);
    float getNextObstacleHeight(float dinoXPosition);
private:
    std::vector<Obstacle> obstacles;
    SDL_Texture *obstacleTexture_1, *obstacleTexture_2, *obstacleTexture_3;
    Uint32 lastSpawnTime;
};
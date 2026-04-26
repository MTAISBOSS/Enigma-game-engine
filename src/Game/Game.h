#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "../ECS/ECS.h"
#include <memory>
const int FPS = 30;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool isRunning;
    int millisecsPreviousFrame = 0;

    std::unique_ptr<Registry> registry;
public:
    Game();

    ~Game();

    void Initialize();

    void Run();

    void ProcessInput();

    void Update();

    void Render();

    void Destroy();

    int windowWidth;
    int windowHeight;

    void Setup();
};

#endif // !GAME_H

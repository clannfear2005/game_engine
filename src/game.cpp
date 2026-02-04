#include "game.hpp"
#include "ECS/components.hpp"
#include "map.hpp"
#include "texture_manager.hpp"

Map *map;

SDL_Renderer *Game::renderer = nullptr;

Manager manager;
auto &player(manager.addEntity());

Game::Game() // Constructor
{}
Game::~Game() // Deconstructor
{}

void Game::init(const char *title, int x, int y, int width, int height, bool fullscreen) {
    int flags = 0;

    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN; // Make window full screen if full screen is passed as true
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL Initialised..." << std::endl;

        window = SDL_CreateWindow(title, x, y, width, height, flags);
        renderer = SDL_CreateRenderer(window, -1, 0);

        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255); // Colour for drawing
        }

        isRunning = true; // If SDL is successfully initialised, window is running
    }

    map = new Map();

    player.addComponent<PositionComponent>();
    player.addComponent<SpriteComponent>("../Textures/sawblade.png");
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        break;
    }
}

void Game::update() {
    manager.refresh();
    manager.update();

    // Testing set texture method
    if (player.getComponent<PositionComponent>().x() > 100) {
        player.getComponent<SpriteComponent>().setTex("../Textures/ant.png");
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    map->DrawMap();

    manager.draw();
    SDL_RenderPresent(renderer); // Update the screen with any rendering done
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    std::cout << "Game clean up successful" << std::endl;
}

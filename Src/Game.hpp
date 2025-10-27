#ifndef Game_hpp
#define Game_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class Game
{
    public:

        Game(); // constructor
        ~Game(); // deconstructor

        void init(const char* title, int x, int y, int width, int height, bool fullscreen);
        void update();
        void render();
        void clean();
        void handleEvents();

        bool running() { return isRunning; }

        static SDL_Renderer* renderer;

    private:

        bool isRunning;
        SDL_Window* window;
};

#endif
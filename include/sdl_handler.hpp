#pragma once

#include <SDL.h>

class SDLHandler {

  public:
    SDLHandler(); // Constructor
    ~SDLHandler();

    void init(const char *title, int x, int y, int width, int height, bool fullscreen); // Initialising window
    void handle_events();                                                               // Handle events, such as exiting screen
    void clean();                                                                       // Clean up
    bool running() { return isRunning; }
    SDL_Window *getWindow() const { return window; }
    SDL_GLContext getGLContext() const { return gl_ctx; }

  private:
    bool isRunning = false;
    SDL_GLContext gl_ctx = nullptr;
    SDL_Window *window = nullptr;
};

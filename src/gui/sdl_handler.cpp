#include "sdl_handler.hpp"

#include "imgui_impl_sdl2.h"

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_video.h>
#include <iostream>

SDLHandler::SDLHandler() {}  // Constructor
SDLHandler::~SDLHandler() {} // Deconstructor

void SDLHandler::init(const char* title, int x, int y, int width, int height, bool fullscreen)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "SDL failed to initialise." << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }

    // Initialising flags - flags are bitmasks, so we OR them together.
    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    if (fullscreen)
    {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    // Setting GL attributes (OpenGL 3.3 Core)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Creating the window
    window = SDL_CreateWindow(title, x, y, width, height, flags);

    if (!window)
    {
        std::cerr << "Window failed to be created. SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        isRunning = false;
        return;
    }

    // Create context
    gl_ctx = SDL_GL_CreateContext(window);
    if (!gl_ctx)
    {
        std::cerr << "GL Context creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        isRunning = false;
        return;
    }

    if (SDL_GL_MakeCurrent(window, gl_ctx) != 0)
    {
        std::cerr << "Failed to make make GL context current: " << SDL_GetError() << std::endl;
        SDL_GL_DeleteContext(gl_ctx);
        SDL_DestroyWindow(window);
        SDL_Quit();
        isRunning = false;
        return;
    }

    isRunning = true; // If SDL is successfully initialised, flag as running.
    std::cout << "SDL and GL initialisation successful" << std::endl;
}

void SDLHandler::handle_events()
{

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        ImGui_ImplSDL2_ProcessEvent(&event);
    }
}

void SDLHandler::clean()
{

    if (gl_ctx)
    {
        SDL_GL_DeleteContext(gl_ctx);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();

    std::cout << "SDL cleanup successful" << std::endl;
}

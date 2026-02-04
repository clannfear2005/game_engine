#include "glad/gl.h"
#include "imguihandler.h"
#include "sdl_handler.hpp"
#include <SDL.h>
#include <stdexcept>

int main() {

    // SDL Initialisation
    SDLHandler sdl_handler;
    sdl_handler.init("NEAT Experiment", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 800, false);

    // Load GL function pointers (GLAD) after a current context exists
    if ( gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress) == 0 ) {
        throw std::runtime_error("Failed to load OpenGL with glad");
    }

    // Enable vsync
    SDL_GL_SetSwapInterval(1);

    // ImGui Initialisation
    ImGuiHandler imguihandler;
    imguihandler.Init(sdl_handler.getWindow(), sdl_handler.getGLContext(), "#version 330 core");

    bool first_update = true; // Flag for first ImGui update().

    // Main loop
    while (sdl_handler.running()) {

        sdl_handler.handle_events(); // Listen for user events to interrupt loop when window is exited.

        imguihandler.NewFrame();
        imguihandler.Update(first_update);
        first_update = false;

        // Handling screen before rendering - should make cleaner.
        int screen_w, screen_h;
        SDL_GetWindowSize(sdl_handler.getWindow(), &screen_w, &screen_h);
        glViewport(0, 0, screen_w, screen_h);
        glClear(GL_COLOR_BUFFER_BIT); // Clearing screen to remove 'ghosting'


        imguihandler.Render();

        SDL_GL_SwapWindow(sdl_handler.getWindow());
    }

    // Clean up
    imguihandler.Shutdown();
    sdl_handler.clean();

    return 0;
}

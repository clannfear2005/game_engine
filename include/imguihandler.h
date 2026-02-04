#pragma once

#include <SDL_video.h>
#include <imgui.h>

class ImGuiHandler {

  public:
    void Init(SDL_Window* window, SDL_GLContext gl_ctx, const char* glsl_version);
    void NewFrame();
    virtual void Update(bool first_update);
    void Render();
    void Shutdown();

    void initDefaultLayout();
};

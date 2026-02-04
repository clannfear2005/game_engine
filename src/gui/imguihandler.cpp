#include "imguihandler.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include "imgui_internal.h"

void ImGuiHandler::Init(SDL_Window* window, SDL_GLContext gl_ctx, const char* glsl_version)
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Not sure if i want multi view ports yet
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Platform/Render Bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_ctx); // SDL2 platform backend
    ImGui_ImplOpenGL3_Init(glsl_version);         // OpenGL3 renderer backend

    // Style
    ImGui::StyleColorsDark();
}

void ImGuiHandler::NewFrame()
{

    // Feed inputs to ImGui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void ImGuiHandler::Update(bool first_update)
{

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID); // Init Dockspace

    if (first_update)
    {
        initDefaultLayout();
    }

    ImGui::Begin("Control Panel");
    ImGui::Text("Run/pause, mutation rate, etc.");
    ImGui::End();

    ImGui::Begin("Crypto Chart");
    ImGui::Text("Insert ImPlot or graphs here.");
    ImGui::End();

    ImGui::Begin("Network Viewer");
    ImGui::Text("Draw NEAT network nodes + edges here.");
    ImGui::End();
}

void ImGuiHandler::Render()
{

    // Render ImGui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHandler::Shutdown()
{

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiHandler::initDefaultLayout()
{

    ImGuiID dockspace_id = ImGui::GetMainViewport()->ID;

    // Clear existing layout
    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

    // Split into regions
    ImGuiID dock_main_id = dockspace_id;
    ImGuiID dock_id_left =
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
    ImGuiID dock_id_bottom =
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);

    // Dock windows by title
    ImGui::DockBuilderDockWindow("Control Panel", dock_id_left);
    ImGui::DockBuilderDockWindow("Crypto Chart", dock_main_id);
    ImGui::DockBuilderDockWindow("Network Viewer", dock_main_id);

    ImGui::DockBuilderFinish(dockspace_id);
}

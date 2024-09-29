#pragma once

#include <Window.h>

#include "TaskManager.h"

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

struct UIState {
    bool show_demo_window = false;
    std::vector<Task> tasks;
};

class ImGuiTodoUI {
public:
    UIState state;

    explicit ImGuiTodoUI(TaskManager task_manager, const WindowSpecification& spec) : task_manager(
            std::move(task_manager)), window(Window(spec)) {}

    void run() {
        init();
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
        while (!window.should_close()) {
#endif
            if (window.poll_events()) continue;

            window.begin_imgui_frame();
            begin_imgui_window();
            render_ui();
            end_imgui_window();
            window.end_imgui_frame();
        }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif
    }

private:
    TaskManager task_manager;
    Window window;

    void init();
    void render_ui();

    static void begin_imgui_window() {
        // Maybe can be set in ini config
        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(viewport->Size, ImGuiCond_Always);
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::Begin("Main Window", nullptr, window_flags);
    }

    static void end_imgui_window() {
        ImGui::End();
    }
};

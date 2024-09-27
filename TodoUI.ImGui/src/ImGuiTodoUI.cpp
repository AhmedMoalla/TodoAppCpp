#include "ImGuiTodoUI.h"
#include "imgui.h"

#include <Window.h>

void display_ui(const TaskManager& task_manager) {
    constexpr WindowSpecification spec = {.width = 1280, .height = 720, .title = "TodoApp"};
    ImGuiTodoUI ui(task_manager, spec);

    ui.run();
}

void ImGuiTodoUI::init() {
    window.add_key_callback(GLFW_KEY_F12, GLFW_MOD_SHIFT, GLFW_PRESS, [&] {
        state.show_demo_window = !state.show_demo_window;
    });
}

void ImGuiTodoUI::render_imgui() const {
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    if (state.show_demo_window) {
        ImGui::ShowDemoWindow(reinterpret_cast<bool *>(true));
    }
}

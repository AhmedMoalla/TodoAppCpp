#include "ImGuiTodoUI.h"
#include "widgets.h"
#include "imgui.h"

#include <print>
#include <Window.h>
#include <magic_enum/magic_enum.hpp>

using namespace todo;
using namespace todo_imgui;

void display_ui(const TaskManager& task_manager) {
    constexpr WindowDimensions dimensions = {.width = 350, .height = 550};
    constexpr WindowSpecification spec = {.dimensions = dimensions, .title = "TodoApp", .resizable = false};
    ImGuiTodoUI ui(task_manager, spec);

    ui.run();
}

void ImGuiTodoUI::init() {
    constexpr auto event_names = magic_enum::enum_names<TaskManagerChangeEventType>();
    task_manager.subscribe_to_changes([&](const TaskManagerChangeEvent& event) {
        std::println("Event of type '{}' was received for task '{}'", event_names[event.type], event.task.title);
        switch (event.type) {
            case Create: state.tasks.push_back(event.task);
                break;
            case Update:
            case Remove: {
                state.tasks.clear();
                for (auto task: task_manager.find_all()) {
                    state.tasks.push_back(task);
                }
                break;
            }
        }
    });

    task_manager.save("Task 1");
    task_manager.save("Task 200");
    task_manager.save("Task 30");
    state.show_demo_window = false;
    window.add_key_callback(GLFW_KEY_F12, GLFW_MOD_SHIFT, GLFW_PRESS, [&] {
        state.show_demo_window = !state.show_demo_window;
    });
}

void ImGuiTodoUI::render_ui() {
    if (state.show_demo_window) {
        ImGui::ShowDemoWindow(nullptr);
    }

    for (auto& task: state.tasks) {
        const auto [checkbox_pressed, delete_pressed] = todo_widgets::task_checkbox(task.id, task.title, task.completed);
        if (checkbox_pressed) {
            task_manager.toggle_complete(task.id);
        }

        if (todo_widgets::delete_confirm_popup(delete_pressed, task)) {
            task_manager.remove(task.id);
        }
    }

    if (todo_widgets::creation_input(&state.create_task_title) && !state.create_task_title.empty()) {
        task_manager.save(state.create_task_title);
        state.create_task_title.clear();
    }
}

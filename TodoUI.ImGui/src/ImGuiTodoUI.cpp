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
            case Create: state.tasks.emplace_back(event.task);
                break;
            case Update:
            case Remove: {
                state.tasks.clear();
                for (auto task: task_manager.find_all()) {
                    state.tasks.emplace_back(task);
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
        const auto [checkbox_pressed, x_pressed, save_pressed, edit_mode_entered] =
            todo_widgets::task_checkbox(task.id, task.title, task.completed, task.edit_mode, &state.edit_task_title);

        if (edit_mode_entered) {
            state.edit_task_title = task.title;
        }

        // Only one task can be in edit mode
        if (task.edit_mode) {
            if (x_pressed) {
                task.edit_mode = false;
                state.edit_task_title = "";
                continue;
            }

            if (save_pressed && !state.edit_task_title.empty()) {
                task.title = state.edit_task_title;
                task_manager.save(static_cast<Task>(task));
                task.edit_mode = false;
                state.edit_task_title = "";
            }

            for (auto& other_task: state.tasks) {
                if (task == other_task) continue;
                other_task.edit_mode = false;
            }
        }

        if (checkbox_pressed) {
            task_manager.toggle_complete(task.id);
        }

        if (todo_widgets::delete_confirm_popup(x_pressed, task)) {
            task_manager.remove(task.id);
        }
    }

    if (todo_widgets::creation_input(&state.create_task_title) && !state.create_task_title.empty()) {
        task_manager.save(state.create_task_title);
        state.create_task_title.clear();
    }
}

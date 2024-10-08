#include "ImGuiTasksUI.h"
#include "widgets.h"
#include "imgui.h"

#include <print>
#include <Window.h>

using namespace tasks;
using namespace tasks_imgui;

void display_ui(const TaskManager& task_manager) {
    constexpr WindowDimensions dimensions = {.width = 350, .height = 550};
    constexpr WindowSpecification spec = {.dimensions = dimensions, .title = "TasksApp", .resizable = false};
    ImGuiTasksUI ui(task_manager, spec);

    ui.run();
}

void ImGuiTasksUI::init() {
    task_manager.subscribe_to_changes([&](const TaskManagerChangeEvent& event) {
        std::println("Event of type '{}' was received for task '{}'", change_event_names[static_cast<size_t>(event.type)], event.task.title);
        using enum TaskManagerChangeEventType;
        switch (event.type) {
            case Create: state.tasks.emplace_back(event.task);
                break;
            case Update:
            case Remove: {
                state.tasks.clear();
                for (const auto& task: task_manager.find_all()) {
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

void ImGuiTasksUI::render_ui() {
    if (state.show_demo_window) {
        ImGui::ShowDemoWindow(nullptr);
    }

    for (auto& task: state.tasks) {
        const auto [checkbox_pressed, x_pressed, save_pressed, edit_mode_entered] =
            tasks_widgets::task_checkbox(task.id, task.title, task.completed, task.edit_mode, &state.edit_task_title);

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

        if (tasks_widgets::delete_confirm_popup(x_pressed, task)) {
            task_manager.remove(task.id);
        }
    }

    if (tasks_widgets::creation_input(&state.create_task_title) && !state.create_task_title.empty()) {
        task_manager.save(state.create_task_title);
        state.create_task_title.clear();
    }
}

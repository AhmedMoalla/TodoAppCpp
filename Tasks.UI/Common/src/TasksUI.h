#pragma once

#include <print>
#include <TaskManager.h>

namespace tasks_ui {
    enum class TasksUIMode : std::uint8_t {
#ifdef TASKS_UI_MODE_REPL
        Repl,
#elifdef TASKS_UI_MODE_IMGUI
        ImGui,
#endif
        None
    };

    static constexpr auto ui_mode_names = magic_enum::enum_names<TasksUIMode>();

    class TasksUI {
    public:
        explicit TasksUI(tasks::TaskManager& task_manager): task_manager(task_manager) {}
        virtual ~TasksUI() = default;
        virtual void display() = 0;
    protected:
        tasks::TaskManager& task_manager;
    };

    class NoOpTasksUI final : public TasksUI {
    public:
        explicit NoOpTasksUI(tasks::TaskManager& task_manager)
            : TasksUI(task_manager) {}

        void display() override {
            std::println("No UI mode selected.");
        }
    };
}

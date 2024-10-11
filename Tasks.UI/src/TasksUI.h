#pragma once
#include <TaskManager.h>

namespace tasks_ui {
    enum class TasksUIMode : std::uint8_t {
        Repl,
        ImGui,
    };

    class TasksUI {
    public:
        explicit TasksUI(tasks::TaskManager* task_manager): task_manager(task_manager) {}
        virtual ~TasksUI() = default;
        virtual void display() = 0;

    protected:
        tasks::TaskManager* task_manager;
    };

    class ImGuiTasksUI : public TasksUI {
    public:
        explicit ImGuiTasksUI(tasks::TaskManager* task_manager)
            : TasksUI(task_manager) {}
    };
}

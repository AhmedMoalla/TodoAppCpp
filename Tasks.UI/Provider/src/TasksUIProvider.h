#pragma once
#include <memory>
#include <TasksUI.h>

namespace tasks_ui {
    class TasksUIProvider {
    public:
        explicit TasksUIProvider(tasks::TaskManager& task_manager)
            : task_manager(task_manager) {}
        std::unique_ptr<TasksUI> provide(TasksUIMode mode) const;
    private:
        tasks::TaskManager& task_manager;
    };
}

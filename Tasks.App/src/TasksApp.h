#pragma once
#include <print>
#include <TasksUI.h>
#include <TasksUIProvider.h>

namespace tasks {
    struct AppSpecification {
        std::string app_name;
        std::string app_version;
        tasks_ui::TasksUIMode ui_mode;

        static AppSpecification from_args(int argc, const char* argv[]);
    };

    class TasksApp {
    public:
        explicit TasksApp(const AppSpecification& spec) : task_manager(std::make_unique<TaskManager>()),
                                                          ui_provider(
                                                              std::make_unique<tasks_ui::TasksUIProvider>(
                                                                  *task_manager)),
                                                          ui(ui_provider->provide(spec.ui_mode)) {
            std::println("Starting up Tasks v{} in '{}' UI mode", TASKS_VERSION, TASKS_UI_MODE);
        }

        void run() const;

    private:
        std::unique_ptr<TaskManager> task_manager;
        std::unique_ptr<tasks_ui::TasksUIProvider> ui_provider;
        std::unique_ptr<tasks_ui::TasksUI> ui;
    };
}

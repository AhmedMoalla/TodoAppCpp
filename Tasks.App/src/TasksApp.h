#pragma once
#include <TasksUI.h>

namespace tasks {
    struct AppSpecification {
        tasks_ui::TasksUIMode ui_mode;

        static AppSpecification from_args(int argc, const char* argv[]);
    };

    class TasksApp {
    public:
        explicit TasksApp(AppSpecification spec) {}
        void run();
    };
}

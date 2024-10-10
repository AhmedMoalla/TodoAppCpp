#include <print>
#include <TaskManager.h>

void display_ui(const tasks::TaskManager& task_manager);

namespace tasks_ui {
    class TasksUI {
    public:
        explicit TasksUI(tasks::TaskManager *task_manager)
            : task_manager(task_manager) {
        }
        virtual ~TasksUI() = default;

        virtual void display() = 0;
    private:
        std::unique_ptr<tasks::TaskManager> task_manager;
    };

    class ImGuiTasksUI : public TasksUI {
    public:
        explicit ImGuiTasksUI(tasks::TaskManager *task_manager)
            : TasksUI(task_manager) {
        }

        void display() override {

        }
    };

    class ReplTasksUI : public TasksUI {
    public:
        explicit ReplTasksUI(tasks::TaskManager *task_manager)
            : TasksUI(task_manager) {
        }

        void display() override {

        }
    };

    class TasksUIProvider {
        public:
        std::unique_ptr<TasksUI> provide(tasks::TaskManager *task_manager) {
#ifdef TASKS_UI_MODE_REPL
            return std::make_unique<TasksReplUI>(task_manager);
#elifdef TASKS_UI_MODE_IMGUI
            return std::make_unique<ImGuiTasksUI>(task_manager);
#endif
        }
    };
}

namespace tasks {
    class TasksApp {
    public:
        explicit TasksApp(tasks_ui::TasksUI* tasks_ui)
            : tasks_ui(tasks_ui) {
        }

        void run() {
            tasks_ui->display();
        }
    private:
        std::unique_ptr<tasks_ui::TasksUI> tasks_ui;
    };
}

int main() {
    std::println("Starting up TasksApp v{}.{} in '{}' mode", VERSION_MAJOR, VERSION_MINOR, TASKS_UI_MODE);
    const auto task_manager = std::make_unique<tasks::TaskManager>();
    const auto ui_provider = std::make_unique<tasks_ui::TasksUIProvider>();
    const auto ui = ui_provider->provide(task_manager.get());
    tasks::TasksApp app(ui.get());
    app.run();
}

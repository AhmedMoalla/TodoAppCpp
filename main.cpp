#include <print>
#include <TaskManager.h>
#include <version.h>

void display_ui(const tasks::TaskManager& task_manager);
class TaskUI {
public:
    explicit TaskUI(tasks::TaskManager* task_manager) : task_manager(task_manager) {}
    virtual ~TaskUI() = default;
    virtual void display() const = 0;
protected:
    tasks::TaskManager* task_manager;
};

class NewUI : public TaskUI {
public:
    explicit NewUI(tasks::TaskManager* task_manager)
        : TaskUI(task_manager) {}

    void display() const override {
        std::println("Displaying new UI {}", task_manager->find_all().size());
    }
};

namespace tasks_ui {
    static std::unique_ptr<TaskUI> provide_ui(tasks::TaskManager* task_manager) {

    }
}


int main() {
    std::println("Starting up TasksApp v{}.{} in '{}' mode", VERSION_MAJOR, VERSION_MINOR, TASKS_UI_MODE);
    const auto task_manager = std::make_unique<tasks::TaskManager>();
    const auto ui = tasks_ui::provide_ui(task_manager.get());
    ui->display();
}

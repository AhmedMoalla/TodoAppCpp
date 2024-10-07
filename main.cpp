#include <print>
#include <TaskManager.h>
#include <version.h>

void display_ui(const todo::TaskManager& task_manager);

class TaskUI {
public:
    explicit TaskUI(todo::TaskManager* task_manager) : task_manager(task_manager) {}
    virtual ~TaskUI() = default;
    virtual void display() const = 0;
protected:
    todo::TaskManager* task_manager;
};

class NewUI : public TaskUI {
public:
    explicit NewUI(todo::TaskManager* task_manager)
        : TaskUI(task_manager) {}

    void display() const override {
        std::println("Displaying new UI {}", task_manager->find_all().size());
    }
};

int main() {
    std::println("Starting up TodoApp v{}.{} in '{}' mode", VERSION_MAJOR, VERSION_MINOR, TODO_UI_MODE);
    const auto task_manager = std::make_unique<todo::TaskManager>();
    const NewUI ui(task_manager.get());
    ui.display();
}

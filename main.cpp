#include <print>
#include <TaskManager.h>

void display_ui(const tasks::TaskManager& task_manager);

int main() {
    std::println("Starting up TasksApp v{}.{} in '{}' mode", VERSION_MAJOR, VERSION_MINOR, TASKS_UI_MODE);
    const tasks::TaskManager task_manager;
    display_ui(task_manager);
}

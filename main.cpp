#include <print>
#include <TaskManager.h>
#include <version.h>

void display_ui(const todo::TaskManager& task_manager);

int main() {
    std::println("Starting up TodoApp v{}.{} in '{}' mode", VERSION_MAJOR, VERSION_MINOR, TODO_UI_MODE);
    const todo::TaskManager task_manager;
    display_ui(task_manager);
}

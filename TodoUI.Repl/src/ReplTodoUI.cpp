#include "ReplTodoUI.h"

#include <format>
#include <iostream>

using namespace todo;
using namespace todo_repl;

void display_ui(const TaskManager& task_manager) {
    ReplTodoUI ui(task_manager);
    ui.run();
}

namespace {
    Command next_command() {
        std::cout << "> ";
        std::string command;
        std::getline(std::cin, command);
        return Command(command);
    }

    void print_invalid_command(const std::string_view command) {
        std::cout << "Invalid command: '" << command << "' (type 'help' to list possible commands)\n";
    }

    void print_help() {
        std::cout << "Possible commands:\n";
        std::cout << "\thelp: shows this help text.\n";
        std::cout << "\texit: exits the program.\n";
    }

    std::optional<int> parse_task_id(const std::string_view& str_task_id) {
        const auto str_id = static_cast<std::string>(str_task_id);
        try {
            return std::stoi(str_id);
        } catch (std::invalid_argument&) {
            std::cout << std::format("Value '{}' is not a valid task id.\n", str_id);
        } catch(std::out_of_range&) {
            std::cout << std::format("Value '{}' is too big. Maximum value is: {}.\n", str_id, std::numeric_limits<int>::max());
        }
        return std::nullopt;
    }
}

void ReplTodoUI::run() {
    Command command;
    while ((command = next_command()).type != Exit) {
        switch (command.type) {
        case Help: print_help(); break;
        case List: list_tasks(); break;
        case Complete: complete_task(command.args); break;
        case Remove: remove_task(command.args); break;
        case Create: create_task(command.args); break;
        case Update: update_task(command.args); break;
        case Unknown: print_invalid_command(command.raw_command); break;
        case Exit: break;
        }
    }
}

void ReplTodoUI::create_task(const std::vector<std::string_view>& args) {
    if (args.size() != 1) {
        std::cout << R"(Invalid number of arguments. Usage: create "task title")" << ".\n";
        return;
    }

    auto new_task = task_manager.save(Task(static_cast<std::string>(args[0])));
    std::cout << std::format("Task '{}' (id={}) created successfully.\n", new_task.title, new_task.id);
}

void ReplTodoUI::update_task(const std::vector<std::string_view>& args) {
    if (args.size() != 2) {
        std::cout << "Invalid number of arguments. Usage: update <task-id> <new-title>.\n";
        return;
    }

    if (const auto task_id = parse_task_id(args[0])) {
        if (auto task = task_manager.find_by_id(task_id.value())) {
            task->title = args[1];
            task_manager.save(*task);
        }        
    }
}

void ReplTodoUI::list_tasks() const {
    const auto tasks = task_manager.find_all();
    if (tasks.empty()) {
        std::cout << "No Tasks found.\n";
        return;
    }
    for (auto task : tasks) {
        std::cout << std::format("{}- [{}] {}\n", task.id, task.completed ? "X" : " ", task.title);
    }
}

void ReplTodoUI::complete_task(const std::vector<std::string_view>& args) {
    if (args.size() != 1) {
        std::cout << "Invalid number of arguments. Usage: complete <task-id>.\n";
        return;
    }

    const auto task_id = parse_task_id(args[0]);
    if (task_id && !task_manager.toggle_complete(task_id.value())) {
        std::cout << std::format("Task '{}' was not found.\n", *task_id);
    }
}

void ReplTodoUI::remove_task(const std::vector<std::string_view>& args) {
    if (args.size() != 1) {
        std::cout << "Invalid number of arguments. Usage: remove <task-id>.\n";
        return;
    }

    const auto task_id = parse_task_id(args[0]);
    if (task_id && !task_manager.remove(task_id.value())) {
        std::cout << std::format("Task '{}' was not found.\n", *task_id);
    }
}


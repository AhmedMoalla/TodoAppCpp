#pragma once
#include "TaskManager.h"
#include "utils.h"

namespace tasks_repl {
enum class CommandType : std::uint8_t {
    Help, Exit, List, Complete, Remove, Create, Update, Unknown = std::numeric_limits<uint8_t>::max()
};

static CommandType parse_command_type(const std::string_view command) {
    const auto trimmed = tasks_utils::string_trim(command);
    using enum CommandType;
    if (trimmed == "help") return Help;
    if (trimmed == "exit") return Exit;
    if (trimmed == "list") return List;
    if (trimmed == "complete") return Complete;
    if (trimmed == "remove") return Remove;
    if (trimmed == "create") return Create;
    if (trimmed == "update") return Update;
    return Unknown;
}

struct Command {
    CommandType type;
    std::string raw_command;
    std::vector<std::string_view> split_command;
    std::vector<std::string_view> args;

    Command() : type(CommandType::Unknown) {}

    explicit Command(std::string command) : raw_command(std::move(command)) {
        split_command = tasks_utils::string_split(raw_command, ' ', true);
        type = parse_command_type(split_command[0]);
        args = std::vector(split_command.begin() + 1, split_command.end());
    }
};

class ReplTasksUI {
public:
    explicit ReplTasksUI(tasks::TaskManager task_manager) : task_manager(std::move(task_manager)) {}

    void run();

private:
    tasks::TaskManager task_manager;

    void list_tasks() const;
    void create_task(const std::vector<std::string_view>& args);
    void update_task(const std::vector<std::string_view>& args);
    void complete_task(const std::vector<std::string_view>& args);
    void remove_task(const std::vector<std::string_view>& args);
};
}

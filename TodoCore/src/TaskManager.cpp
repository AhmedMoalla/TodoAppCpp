#include <ranges>

#include "TaskManager.h"

Task TaskManager::save(Task task) {
    if (const auto found = tasks.find(task.id); found != tasks.end()) {
        found->second = task;
    } else {
        task.id = next_id++;
        tasks.emplace(task.id, task);
    }
    return task;
}

std::optional<Task> TaskManager::find_by_id(const int id) const {
    if (const auto found = tasks.find(id); found != tasks.end()) {
        return found->second;
    }

    return std::nullopt;
}

std::ranges::values_view<std::ranges::ref_view<const std::map<int, Task>>> TaskManager::find_all() const {
    return tasks | std::views::values;
}

bool TaskManager::remove(const int id) {
    return tasks.erase(id);
}

std::optional<Task> TaskManager::toggle_complete(const int id) {
    if (auto found = find_by_id(id)) {
        found->completed = !found->completed;
        return save(found.value());
    }

    return std::nullopt;
}

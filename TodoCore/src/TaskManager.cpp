#include <ranges>

#include "TaskManager.h"

using namespace todo;

Task TaskManager::save(Task task) {
    if (const auto found = tasks.find(task.id); found != tasks.end()) {
        found->second = task;
        notify_change({ Update, task });
    } else {
        task.id = next_id++;
        tasks.emplace(task.id, task);
        notify_change({ Create, task });
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
    if (const auto task = find_by_id(id)) {
        const bool is_removed = tasks.erase(id);
        notify_change({ Remove, task.value() });
        return is_removed;
    }
    return false;
}

std::optional<Task> TaskManager::toggle_complete(const int id) {
    if (auto found = find_by_id(id)) {
        found->completed = !found->completed;
        const auto task = found.value();
        notify_change({ Update, task });
        return save(task);
    }

    return std::nullopt;
}

void TaskManager::subscribe_to_changes(const TaskManagerChangeEventCallback& callback) {
    change_listeners.push_back(callback);
}

// Should be done asynchronously maybe use std::async ?
void TaskManager::notify_change(const TaskManagerChangeEvent& event) const {
    for (auto change_listener : change_listeners) {
        change_listener(event);
    }
}


#pragma once
#include <functional>
#include <string>
#include <optional>
#include <map>
#include <ranges>

static constexpr int unsaved_id = -1;

struct Task {
    int id = unsaved_id;
    std::string title;
    bool completed = false;

    explicit Task(std::string title): title(std::move(title)) {}

    auto operator<=>(const Task&) const = default;
};

enum TaskManagerChangeEventType: char {
    Create, Update, Remove
};

struct TaskManagerChangeEvent {
    const TaskManagerChangeEventType type;
    const Task& task;
};

typedef std::function<void(const TaskManagerChangeEvent& event)> TaskManagerChangeEventCallback;

class TaskManager {
public:
    template<class... TaskArgs>
    Task save(TaskArgs&&... args) {
        const Task task(std::forward<TaskArgs>(args)...);
        return save(task);
    }

    Task save(Task task);
    [[nodiscard]] std::optional<Task> find_by_id(int id) const;
    [[nodiscard]] std::ranges::values_view<std::ranges::ref_view<const std::map<int, Task>> > find_all() const;
    bool remove(int id);
    std::optional<Task> toggle_complete(int id);
    void subscribe_to_changes(const TaskManagerChangeEventCallback& callback);

private:
    std::map<int, Task> tasks;
    std::vector<TaskManagerChangeEventCallback> change_listeners;
    int next_id = 1;

    void notify_change(const TaskManagerChangeEvent& event) const;
};

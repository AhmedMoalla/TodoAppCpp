#pragma once
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

class TaskManager {
public:
    Task save(Task task);
    [[nodiscard]] std::optional<Task> find_by_id(int id) const;
    [[nodiscard]] std::ranges::values_view<std::ranges::ref_view<const std::map<int, Task>>> find_all() const;
    bool remove(int id);
    std::optional<Task> toggle_complete(int id);

private:
    std::map<int, Task> tasks;
    int next_id = 1;
};

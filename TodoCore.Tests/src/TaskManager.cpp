#include "TaskManager.h"

#include <print>

#include "gtest/gtest.h"

using namespace todo;

TEST(Task, should_be_uncompleted_at_initialization) {
    const Task task("title");
    ASSERT_FALSE(task.completed);
}

TEST(Task, should_be_constructed_using_only_title) {
    const auto expected_title = "title";
    const Task task(expected_title);
    ASSERT_EQ(expected_title, task.title);
}

TEST(Task, should_have_minus_1_as_id_at_initialization) {
    const Task task("title");
    ASSERT_EQ(unsaved_id, task.id);
}

TEST(Task, should_be_comparable) {
    const Task task1("title");
    const Task task2("title");
    ASSERT_EQ(task1, task2);
}

TEST(TaskManager, should_find_all_tasks) {
    TaskManager task_manager;
    ASSERT_TRUE(task_manager.find_all().empty());

    const Task task("test");
    task_manager.save(task);

    const Task task1("test");
    task_manager.save(task1);

    ASSERT_EQ(2, task_manager.find_all().size());
}

TEST(TaskManager, should_assign_id_to_saved_task) {
    TaskManager task_manager;

    const Task task("test");
    const Task saved_task = task_manager.save(task);

    ASSERT_NE(unsaved_id, saved_task.id);
}

TEST(TaskManager, should_update_existing_task_on_save) {
    TaskManager task_manager;

    const Task task("test");
    Task saved_task = task_manager.save(task);

    const auto new_title = "title";
    saved_task.title = new_title;
    const Task updated_task = task_manager.save(saved_task);

    ASSERT_EQ(saved_task.id, updated_task.id);
    ASSERT_EQ(new_title, updated_task.title);

    ASSERT_EQ(1, task_manager.find_all().size());
}

TEST(TaskManager, should_find_task_by_id) {
    TaskManager task_manager;

    const Task task("test");
    Task saved_task = task_manager.save(task);

    const auto found = task_manager.find_by_id(saved_task.id);
    ASSERT_TRUE(found);
    ASSERT_EQ(saved_task, found.value());

    ASSERT_FALSE(task_manager.find_by_id(9999));
}

TEST(TaskManager, should_remove_task) {
    TaskManager task_manager;

    const Task task("test");
    Task saved_task = task_manager.save(task);

    const auto found = task_manager.find_by_id(saved_task.id);
    ASSERT_TRUE(found);
    ASSERT_EQ(saved_task, found.value());

    ASSERT_TRUE(task_manager.remove(saved_task.id));

    ASSERT_FALSE(task_manager.find_by_id(saved_task.id));

    ASSERT_FALSE(task_manager.remove(9999));
}

TEST(TaskManager, should_toggle_complete_task) {
    TaskManager task_manager;

    const Task task("test");
    const Task saved_task = task_manager.save(task);

    ASSERT_FALSE(saved_task.completed);

    const auto completed_task = task_manager.toggle_complete(saved_task.id);
    ASSERT_TRUE(completed_task);
    ASSERT_TRUE(completed_task->completed);

    const auto found = task_manager.find_by_id(saved_task.id);
    ASSERT_TRUE(found.value().completed);

    const auto uncompleted_task = task_manager.toggle_complete(completed_task->id);
    ASSERT_TRUE(uncompleted_task);
    ASSERT_FALSE(uncompleted_task->completed);
}

TEST(TaskManager, should_emplace_task) {
    TaskManager task_manager;

    std::string title = "Task 1";
    const Task created_task = task_manager.save(title);
    ASSERT_EQ(title, created_task.title);
}

TEST(TaskManager, should_emit_change_event_to_subscriber) {
    TaskManager task_manager;

    int counter[magic_enum::enum_count<TaskManagerChangeEventType>()] = {};
    std::string title = "test";
    task_manager.subscribe_to_changes([&](const TaskManagerChangeEvent& event) {
        std::println("Event of type: {} received for task '{}'",
                    change_event_names[static_cast<size_t>(event.type)] , event.task.title);
        ASSERT_EQ(title, event.task.title);
        counter[static_cast<size_t>(event.type)]++;
    });

    Task created_task = task_manager.save(title);
    title = created_task.title = "new title";
    const Task updated_task = task_manager.save(created_task);
    task_manager.remove(updated_task.id);
    task_manager.save(title = "Another title");

    using enum TaskManagerChangeEventType;
    ASSERT_EQ(2, counter[static_cast<size_t>(Create)]);
    ASSERT_EQ(1, counter[static_cast<size_t>(Update)]);
    ASSERT_EQ(1, counter[static_cast<size_t>(Remove)]);
}

TEST(TaskManager, should_emit_remove_event_after_removal) {
    TaskManager task_manager;

    task_manager.save("Task 1");
    const Task to_delete = task_manager.save("Task 2");
    task_manager.subscribe_to_changes([&](const TaskManagerChangeEvent& event) {
        ASSERT_EQ("Task 2", event.task.title);
        ASSERT_EQ(1, task_manager.find_all().size());
    });

    task_manager.remove(to_delete.id);
}

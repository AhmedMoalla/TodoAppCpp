#include "TaskManager.h"

#include "gtest/gtest.h"

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
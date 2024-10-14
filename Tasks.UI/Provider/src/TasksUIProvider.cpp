#include "TasksUIProvider.h"

#include <print>
#ifdef TASKS_UI_MODE_REPL
    #include <ReplTasksUI.h>
#elifdef TASKS_UI_MODE_IMGUI
#include <ImGuiTasksUI.h>
#endif

namespace tasks_ui {
    std::unique_ptr<TasksUI> TasksUIProvider::provide(const TasksUIMode mode) const {
        switch (mode) {
#ifdef TASKS_UI_MODE_REPL
            case TasksUIMode::Repl: return std::make_unique<ReplTasksUI>(task_manager);
#elifdef TASKS_UI_MODE_IMGUI
            case TasksUIMode::ImGui: {
                // TODO: Get from configuration
                constexpr WindowDimensions dimensions = {.width = 350, .height = 550};
                constexpr WindowSpecification spec = {
                    .dimensions = dimensions, .title = "TasksApp", .resizable = false
                };
                return std::make_unique<ImGuiTasksUI>(task_manager, spec);
            }
#endif
            case TasksUIMode::None:
                return std::make_unique<NoOpTasksUI>(task_manager);
            default:
                std::println(stderr, "UI mode '{}' is unsupported.", magic_enum::enum_name(mode));
                std::exit(1);
        }
    }
}

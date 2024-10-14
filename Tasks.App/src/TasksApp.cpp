#include <argparse/argparse.hpp>

#include <print>
#include <ranges>
#include "TasksApp.h"

#include <utils.h>

namespace tasks {
    AppSpecification AppSpecification::from_args(const int argc, const char* argv[]) {
        using namespace tasks_ui;
        using namespace tasks_utils;
        using namespace magic_enum;

        AppSpecification spec{
            .app_name = "Tasks",
            .app_version = TASKS_VERSION,
            .ui_mode = TasksUIMode::None
        };

        argparse::ArgumentParser program(spec.app_name, spec.app_version);

        // TODO: Should be added by the UI module optionally
        // TODO: choices and metavar should be generated using magic_enum
        auto ui_modes = enum_names<TasksUIMode>();
        std::string ui_mode_str;
        auto ui_mode_arg = program.add_argument("--ui-mode", "-u")
                .help("ui mode used by the application")
                .metavar(std::format("<{}>", string_join(ui_modes, '|')))
                .default_value(string_to_lowercase(ui_modes[0]))
                .nargs(1)
                .store_into(ui_mode_str);
        for (auto ui_mode : ui_modes) {
            ui_mode_arg.add_choice(ui_mode);
        }

        try {
            program.parse_args(argc, argv);
        } catch (const std::exception& err) {
            std::println("{}\n{}", err.what(), program.help().str());
            std::exit(1);
        }

        spec.ui_mode = enum_cast<TasksUIMode>(ui_mode_str, case_insensitive).value();

        return spec;
    }

    void TasksApp::run() const {
        ui->display();
    }
}

#include <argparse/argparse.hpp>

#include "TasksApp.h"

namespace tasks {
    AppSpecification AppSpecification::from_args(int argc, const char* argv[]) {
        AppSpecification spec{};
        argparse::ArgumentParser program("tasks_app", TASKS_VERSION);
        program.add_argument("--ui-mode", "-u")
            .metavar("<repl|imgui>")
            .help("ui mode used by the application")
            .default_value("repl")
            .nargs(1)
            .choices("repl", "imgui");

        try {
            program.parse_args(argc, argv);
        } catch (const std::exception& err) {
            std::cerr << err.what() << std::endl;
            std::cerr << program;
            std::exit(1);
        }

        std::cout << program;
        auto ui_mode = program.get<std::string>("--ui-mode");
        std::println("Ui mode: {}", ui_mode);
        return spec;
    }

    void TasksApp::run() {

    }
}

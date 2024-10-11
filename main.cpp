#include <print>

#include "TasksApp.h"

// App should have command line args to specify in which mode should the app start
// --help (-h): Show help
// Required Argument: Long form: --ui-mode=repl|imgui (-u)
// Application should show help if started without required args.
// Arguments should be configurable using a --configure=profile (-c) which saves the current command line args
//      to a profile on an INI file. Should be able to start with a given profile using --profile=profile (-p) which will
//      start the program with the args configured for that profile.
// Should be able to set a default profile using --default-profile (-d) flag WITH the --configure args
//      Usage: --configure -p my-profile -d => Sets the 'my-profile' profile as a default profile
// Application started without any arg will start with default profile args instead of showing help

// Compiling the application with Tasks.UI.Mode=all (Tasks.UI.Mode.All) will make all ui modes available. The user can
// enable one of the ui modes using the command line argument described above. If the application was compiled using
// only one of the ui modes the --ui-mode argument must not be available and the application should start in that ui mode
// if all required args are given.

int main(const int argc, const char* argv[]) {
    std::println("Starting up TasksApp v{} in '{}' mode", TASKS_VERSION, TASKS_UI_MODE);
    const auto spec = tasks::AppSpecification::from_args(argc, argv);
    tasks::TasksApp app(spec);
    app.run();
}

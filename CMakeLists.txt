cmake_minimum_required(VERSION 3.24)

project(TasksApp VERSION 1.0 LANGUAGES CXX)

# Must be included after project so we can use CMAKE_CXX_COMPILER_ID and others...
include(CompilerFlags.cmake)

# Define the TasksApp executable
add_executable(TasksApp main.cpp)

# Optionally link with one or the other UI libs
set(Tasks.UI.Mode.Repl "repl")
set(Tasks.UI.Mode.ImGui "imgui")
set(Tasks.UI.Mode ${Tasks.UI.Mode.Repl} CACHE STRING "Specifies the UI mode that the app will run with")
set_property(CACHE Tasks.UI.Mode PROPERTY STRINGS ${Tasks.UI.Mode.Repl} ${Tasks.UI.Mode.ImGui})
message(STATUS "Tasks.UI.Mode='${Tasks.UI.Mode}'")
target_compile_definitions(TasksApp PRIVATE TASKS_UI_MODE="${Tasks.UI.Mode}")

# Add libraries
add_subdirectory(Tasks.Core)
add_subdirectory(Tasks.Tests)
if (Tasks.UI.Mode STREQUAL ${Tasks.UI.Mode.Repl})
    add_subdirectory(Tasks.UI/Repl)
    target_link_libraries(TasksApp PRIVATE Tasks.UI.Repl)
    target_compile_definitions(TasksApp PRIVATE TASKS_UI_MODE_REPL)
elseif (Tasks.UI.Mode STREQUAL ${Tasks.UI.Mode.ImGui})
    add_subdirectory(Tasks.UI/ImGui)
    target_link_libraries(TasksApp PRIVATE Tasks.UI.ImGui)
    target_compile_definitions(TasksApp PRIVATE TASKS_UI_MODE_IMGUI)
endif ()

# Link libraries
target_link_libraries(TasksApp PRIVATE Tasks.CompilerFlags Tasks.Core)

# Expose app version as a definition
target_compile_definitions(TasksApp PRIVATE
        TASKS_VERSION=${TasksApp_VERSION_MAJOR}.${TasksApp_VERSION_MINOR})

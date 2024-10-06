cmake_minimum_required(VERSION 3.30)

project(TodoApp VERSION 1.0 LANGUAGES CXX)

# Must be included after project so we can use CMAKE_CXX_COMPILER_ID and others...
include(CompilerFlags.cmake)

# Define the TodoApp executable
add_executable(TodoApp main.cpp)

# Optionally link with one or the other UI libs
set(TodoUIMode.Repl "repl")
set(TodoUIMode.ImGui "imgui")
set(TodoUIMode ${TodoUIMode.Repl} CACHE STRING "Specifies the UI mode that the app will run with")
set_property(CACHE TodoUIMode PROPERTY STRINGS ${TodoUIMode.Repl} ${TodoUIMode.ImGui})
message(STATUS "TodoUIMode='${TodoUIMode}'")
target_compile_definitions(TodoApp PRIVATE TODO_UI_MODE="${TodoUIMode}")

# Add libraries
add_subdirectory(TodoCore)
add_subdirectory(TodoCore.Tests)
if (TodoUIMode STREQUAL ${TodoUIMode.Repl})
    add_subdirectory(TodoUI.Repl)
    target_link_libraries(TodoApp PRIVATE TodoUI.Repl)
elseif (TodoUIMode STREQUAL ${TodoUIMode.ImGui})
    add_subdirectory(TodoUI.ImGui)
    target_link_libraries(TodoApp PRIVATE TodoUI.ImGui)
endif ()

# Link libraries
target_link_libraries(TodoApp PRIVATE TodoAppCompilerFlags TodoCore)

# Generate version.h which has the version of the project
configure_file(version.h.in version.h)
# Add the binary output dir as include directory because that's were the generated version.h will be
target_include_directories(TodoApp PRIVATE "${PROJECT_BINARY_DIR}")

cmake_minimum_required(VERSION 3.24)

project(Tasks VERSION 1.0 LANGUAGES CXX)

# Must be included after project so we can use CMAKE_CXX_COMPILER_ID and others...
include(CompilerFlags.cmake)

# Define the Tasks executable
add_executable(Tasks main.cpp)

# Add libraries
add_subdirectory(Tasks.App)
add_subdirectory(Tasks.Core)
add_subdirectory(Tasks.UI)
add_subdirectory(Tasks.Tests)

# Link libraries
target_link_libraries(Tasks PRIVATE Tasks.CompilerFlags Tasks.App)

# Expose app version as a definition
target_compile_definitions(Tasks PUBLIC
        TASKS_VERSION="${Tasks_VERSION_MAJOR}.${Tasks_VERSION_MINOR}")

# Define the Core library and include all .cpp files in the src directory
file(GLOB SRC_FILES src/*.cpp)
add_library(Tasks.UI ${SRC_FILES})

# Optionally link with one or the other UI libs
set(Tasks.UI.Mode.Repl "repl")
set(Tasks.UI.Mode.ImGui "imgui")
set(Tasks.UI.Mode ${Tasks.UI.Mode.Repl} CACHE STRING "Specifies the UI mode that the app will run with")
set_property(CACHE Tasks.UI.Mode PROPERTY STRINGS ${Tasks.UI.Mode.Repl} ${Tasks.UI.Mode.ImGui})
message(STATUS "Tasks.UI.Mode='${Tasks.UI.Mode}'")
target_compile_definitions(Tasks.UI INTERFACE TASKS_UI_MODE="${Tasks.UI.Mode}")

if (Tasks.UI.Mode STREQUAL ${Tasks.UI.Mode.Repl})
    add_subdirectory(Repl)
    target_link_libraries(Tasks.UI PRIVATE Tasks.UI.Repl)
    target_compile_definitions(Tasks.UI INTERFACE TASKS_UI_MODE_REPL)
elseif (Tasks.UI.Mode STREQUAL ${Tasks.UI.Mode.ImGui})
    add_subdirectory(ImGui)
    target_link_libraries(Tasks.UI PRIVATE Tasks.UI.ImGui)
    target_compile_definitions(Tasks.UI PRIVATE TASKS_UI_MODE_IMGUI)
endif ()

# Include src directory as include dirs to all consumers of this library
target_include_directories(Tasks.UI INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link libraries
target_link_libraries(Tasks.UI PRIVATE Tasks.CompilerFlags Tasks.Core)
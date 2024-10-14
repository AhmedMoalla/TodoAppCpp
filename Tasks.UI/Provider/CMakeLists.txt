file(GLOB SRC_FILES src/*.cpp)
add_library(Tasks.UI.Provider ${SRC_FILES})

# Optionally link with one or the other UI libs
set(Tasks.UI.Mode.Repl "repl")
set(Tasks.UI.Mode.ImGui "imgui")
set(Tasks.UI.Mode ${Tasks.UI.Mode.Repl} CACHE STRING "Specifies the UI mode that the app will run with")
set_property(CACHE Tasks.UI.Mode PROPERTY STRINGS ${Tasks.UI.Mode.Repl} ${Tasks.UI.Mode.ImGui})
message(STATUS "Tasks.UI.Mode='${Tasks.UI.Mode}'")
target_compile_definitions(Tasks.UI.Provider INTERFACE TASKS_UI_MODE="${Tasks.UI.Mode}")

if (Tasks.UI.Mode STREQUAL ${Tasks.UI.Mode.Repl})
    target_link_libraries(Tasks.UI.Provider PRIVATE Tasks.UI.Repl)
    target_compile_definitions(Tasks.UI.Provider PUBLIC TASKS_UI_MODE_REPL)
elseif (Tasks.UI.Mode STREQUAL ${Tasks.UI.Mode.ImGui})
    target_link_libraries(Tasks.UI.Provider PRIVATE Tasks.UI.ImGui)
    target_compile_definitions(Tasks.UI.Provider PUBLIC TASKS_UI_MODE_IMGUI)
endif ()

target_include_directories(Tasks.UI.Provider INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(Tasks.UI.Provider PUBLIC Tasks.CompilerFlags Tasks.Core Tasks.UI.Common)
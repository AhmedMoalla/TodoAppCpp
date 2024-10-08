# Define the Tasks.UI.ImGui library and include all .cpp files in the src directory
file(GLOB SRC_FILES src/*.cpp)
add_library(Tasks.UI.ImGui ${SRC_FILES})

include(imgui.cmake)

target_include_directories(Tasks.UI.ImGui
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/third-party ${CMAKE_CURRENT_SOURCE_DIR}/resources)
target_link_libraries(Tasks.UI.ImGui PRIVATE Tasks.CompilerFlags Tasks.Core imgui)

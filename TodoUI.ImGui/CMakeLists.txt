# Define the TodoUI.ImGui library and include all .cpp files in the src directory
file(GLOB SRC_FILES src/*.cpp)
add_library(TodoUI.ImGui ${SRC_FILES})

include(imgui.cmake)

target_include_directories(TodoUI.ImGui
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/third-party ${CMAKE_CURRENT_SOURCE_DIR}/resources)
target_link_libraries(TodoUI.ImGui PRIVATE TodoAppCompilerFlags TodoCore imgui)

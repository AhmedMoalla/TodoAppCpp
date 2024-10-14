# Define the Tasks.UI.ImGui library and include all .cpp files in the src directory
file(GLOB SRC_FILES src/*.cpp)
add_library(Tasks.UI.ImGui ${SRC_FILES})

#include(imgui.cmake)

include(FetchContent)
set(GLFW_BUILD_EXAMPLES OFF)    # Disable building GLFW examples
set(GLFW_BUILD_TESTS OFF)       # Disable building GLFW tests
set(GLFW_BUILD_DOCS OFF)        # Disable building GLFW docs
set(GLFW_INSTALL OFF)           # Disable GLFW installation rules
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(glfw)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

FetchContent_Declare(
        imgui_external
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.2-docking
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(imgui_external)
add_library(imgui
        ${imgui_external_SOURCE_DIR}/imgui.cpp
        ${imgui_external_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_external_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_external_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_external_SOURCE_DIR}/imgui_demo.cpp # TODO: Remove => only for testing purposes. Disable by defining IMGUI_DISABLE_DEMO_WINDOWS. Maybe only include this on Debug builds.
        ${imgui_external_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_external_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_external_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)
target_compile_definitions(imgui INTERFACE GL_SILENCE_DEPRECATION)
target_link_libraries(imgui INTERFACE Tasks.CompilerFlags glfw OpenGL::GL)
target_include_directories(imgui
        # For imgui and to compile imgui_impl_glfw.cpp backend
        PUBLIC ${imgui_external_SOURCE_DIR} ${imgui_external_SOURCE_DIR}/backends ${glfw_SOURCE_DIR}/include
        # For consumers of this library
        INTERFACE ${imgui_external_SOURCE_DIR}
)


target_include_directories(Tasks.UI.ImGui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/third-party
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${glfw_SOURCE_DIR}/include
        ${imgui_external_SOURCE_DIR} ${imgui_external_SOURCE_DIR}/backends)

target_link_libraries(Tasks.UI.ImGui PRIVATE Tasks.CompilerFlags Tasks.Core Tasks.UI.Common imgui)

# This file declares an imgui target as a CMake library which automatically includes glfw and OpenGL
include(FetchContent)

# ============= Download GLFW and expose it as a target =============
# Set GLFW compile-time options
set(GLFW_BUILD_EXAMPLES OFF)    # Disable building GLFW examples
set(GLFW_BUILD_TESTS OFF)       # Disable building GLFW tests
set(GLFW_BUILD_DOCS OFF)        # Disable building GLFW docs
set(GLFW_INSTALL OFF)           # Disable GLFW installation rules

# Fetch glfw
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        EXCLUDE_FROM_ALL
)
# Make glfw target available
FetchContent_MakeAvailable(glfw)

# ============= Find OpenGL =============
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# ============= Create a CMake library for imgui =============
# Fetch imgui and expose it as imgui_external
FetchContent_Declare(
        imgui_external
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.2-docking
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(imgui_external)

# Create an imgui library which includes all files from imgui_external target
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

# Link with OpenGL and glfw
target_link_libraries(imgui INTERFACE Tasks.CompilerFlags glfw OpenGL::GL)

# Include dirs
target_include_directories(imgui
        # For imgui and to compile imgui_impl_glfw.cpp backend
        PUBLIC ${imgui_external_SOURCE_DIR} ${imgui_external_SOURCE_DIR}/backends ${glfw_SOURCE_DIR}/include
        # For consumers of this library
        INTERFACE ${imgui_external_SOURCE_DIR}
)

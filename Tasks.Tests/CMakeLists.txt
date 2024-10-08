# Download GoogleTest
include(FetchContent)

set(INSTALL_GTEST OFF)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB SRC_FILES src/*.cpp)
add_executable(Tasks.Tests ${SRC_FILES})

target_link_libraries(gmock PRIVATE Tasks.CompilerFlags)
target_link_libraries(gmock_main PRIVATE Tasks.CompilerFlags)
target_link_libraries(gtest PRIVATE Tasks.CompilerFlags)
target_link_libraries(gtest_main PRIVATE Tasks.CompilerFlags)
target_link_libraries(Tasks.Tests PRIVATE Tasks.CompilerFlags Tasks.Core gtest gtest_main)

# Enables running tests using ctest
enable_testing()
include(GoogleTest)
gtest_discover_tests(Tasks.Tests)
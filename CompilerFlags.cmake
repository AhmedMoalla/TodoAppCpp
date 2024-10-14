# Create an interface library which specifies the version of C++ to be used everywhere
# All projects of TasksApp should link with this library
add_library(Tasks.CompilerFlags INTERFACE)
target_compile_features(Tasks.CompilerFlags INTERFACE cxx_std_23)

# Add warning flags that are specific to compiler types
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(Tasks.CompilerFlags INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
        "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# For some reason, AppleClang v16 does not include C++ stdlib in its search path
set(apple_stdlib_location "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1")
if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16)
    target_include_directories(Tasks.CompilerFlags INTERFACE ${apple_stdlib_location})
endif()

# Expose app version as a definition
target_compile_definitions(Tasks.CompilerFlags INTERFACE
        TASKS_VERSION="${Tasks_VERSION_MAJOR}.${Tasks_VERSION_MINOR}")

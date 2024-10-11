# Define the Core library and include all .cpp files in the src directory
file(GLOB SRC_FILES src/*.cpp)
add_library(Tasks.App ${SRC_FILES})

include(argparse.cmake)

# Include src directory as include dirs to all consumers of this library
target_include_directories(Tasks.App INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link libraries
target_link_libraries(Tasks.App PUBLIC Tasks.CompilerFlags Tasks.Core Tasks.UI argparse)
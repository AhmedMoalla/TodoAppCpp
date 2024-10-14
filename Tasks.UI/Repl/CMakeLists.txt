# Define the Tasks.UI.Repl library and include all .cpp files in the src directory
file(GLOB SRC_FILES src/*.cpp)
add_library(Tasks.UI.Repl ${SRC_FILES})

# Include src directory as include dirs to all consumers of this library
target_include_directories(Tasks.UI.Repl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link libraries
target_link_libraries(Tasks.UI.Repl PRIVATE Tasks.CompilerFlags Tasks.Core Tasks.UI.Common)
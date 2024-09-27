# Define the TodoUI.Repl library and include all .cpp files in the src directory
file(GLOB SRC_FILES src/*.cpp)
add_library(TodoUI.Repl ${SRC_FILES})

# Include src directory as include dirs to all consumers of this library
target_include_directories(TodoUI.Repl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link libraries
target_link_libraries(TodoUI.Repl PRIVATE TodoAppCompilerFlags TodoCore)
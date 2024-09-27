# Define the TodoCore library and include all .cpp files in the src directory
file(GLOB SRC_FILES src/*.cpp)
add_library(TodoCore ${SRC_FILES})

# Include src directory as include dirs to all consumers of this library
target_include_directories(TodoCore INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link libraries
target_link_libraries(TodoCore PRIVATE TodoAppCompilerFlags)
# Define the Core library and include all .cpp files in the src directory
file(GLOB SRC_FILES src/*.cpp)
add_library(Tasks.Core ${SRC_FILES})

include(magic_enum.cmake)

# Include src directory as include dirs to all consumers of this library
target_include_directories(Tasks.Core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src ${magic_enum_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(Tasks.Core PRIVATE Tasks.CompilerFlags magic_enum)
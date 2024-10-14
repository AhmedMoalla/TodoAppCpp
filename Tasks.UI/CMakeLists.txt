# Define the Core library and include all .cpp files in the src directory
add_library(Tasks.UI INTERFACE)

add_subdirectory(Common)
add_subdirectory(Provider)
add_subdirectory(ImGui)
add_subdirectory(Repl)

# Link libraries
target_link_libraries(Tasks.UI INTERFACE Tasks.UI.Provider)
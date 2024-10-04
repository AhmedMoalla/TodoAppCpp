# Todo App in C++

This is a Todo App made in C++ to learn the language.

The code might be bad and the application overly complicated. 
(Hence the learning part of this project)

# Miscellaneous Information

| Key              | Value                               |
|------------------|-------------------------------------|
| Build System     | CMake                               |
| C++ Version      | 23                                  |
| Tested Compilers | MSVC 19.41.34120 (VS2022), GCC 14.2 |

# Project Structure
The project is made of multiple subproject with different purposes:
```text
├── CMakeLists.txt <-- Root CMake file
├── TodoCore <-- Library containing the API for the core logic
├── TodoCore.Tests <-- Google Tests project to test the TodoCore project
├── TodoUI.ImGui <-- GUI made with ImGUI 
├── TodoUI.Repl <-- REPL UI
└── main.cpp <-- main() entry point
```

# Build Information

Some flags can be set to customize the build. Here is a complete list:

| Name       | Possible Values | Description                                                                             |
|------------|-----------------|-----------------------------------------------------------------------------------------|
| TodoUIMode | repl, imgui     | Used to link with either [TodoUI.Repl](./TodoUI.Repl) or [TodoUI.ImGui](./TodoUI.ImGui) |


## Build Instructions
### Windows
```shell
cmake -S . -B build 
```
> You can also open the project in JetBrains's CLion

### Linux
```shell
cmake -S . -B build 
```
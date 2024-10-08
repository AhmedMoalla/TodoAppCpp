# Todo App in C++

This is a Todo App made in C++ to learn the language.

The code might be bad and the application overly complicated. 
(Hence the learning part of this project)

# Miscellaneous Information

| Key              | Value                                                                                                                 |
|------------------|-----------------------------------------------------------------------------------------------------------------------|
| Build System     | CMake                                                                                                                 |
| C++ Version      | 23                                                                                                                    |
| Tested Compilers | - **Windows:** MSVC 19.41.34120 (VS2022)<br/>- **Linux:** GCC 14.2<br/>- **MacOS (ARM):** clang 15 (clang-1500.3.9.4) |

# Project Structure
The project is made of multiple subproject with different purposes:
```text
├── CMakeLists.txt <-- Root CMake file
├── Tasks.Core <-- Library containing the API for the core logic
├── Tasks.Tests <-- Google Tests project
├── Tasks.UI
│         ├── ImGui <-- GUI with ImGUI
│         └── Repl  <-- REPL UI
└── main.cpp <-- main() entry point
```

# Build Information

Some flags can be set to customize the build. Here is a complete list:

| Name          | Possible Values | Description                                                                                     |
|---------------|-----------------|-------------------------------------------------------------------------------------------------|
| Tasks.UI.Mode | repl, imgui     | Used to link with either [Tasks.UI.Repl](./Tasks.UI/Repl) or [Tasks.UI.ImGui](./Tasks.UI/ImGui) |


## Build Instructions
### Windows
```shell
cmake -S . -B build -DTasks.UI.Mode:STRING=imgui
# Will pick up your local Visual Studio installation and generate a solution in the build directory
# Make files will be generated if no VS installation is found
```
> You can also open the project in JetBrains's CLion

### Linux & MacOS
```shell
cmake -S . -B build -DTasks.UI.Mode:STRING=imgui
cd build && make
```

### Running the tests
```shell
cd build/Tasks.Tests && ctest
```
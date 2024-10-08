include(FetchContent)

FetchContent_Declare(
        magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum
        GIT_TAG v0.9.6
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(magic_enum)

target_include_directories(magic_enum INTERFACE ${magic_enum_SOURCE_DIR}/include)
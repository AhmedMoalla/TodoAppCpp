include(FetchContent)

FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse
        GIT_TAG v3.1
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(argparse)
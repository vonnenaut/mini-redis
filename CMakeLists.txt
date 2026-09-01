# 1. Specify the minimum version of CMake required to build the project
cmake_minimum_required(VERSION 3.15)

# 2. Define the project name and explicitly set the language to C
project(MyCProject LANGUAGES C)

# 3. Specify the C Standard (e.g., C99, C11, C17, C23)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 4. Add compiler flags for common warnings (Optional but recommended)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 5. Define include directories so your code can find local header (.h) files
include_directories(include)

# 6. Declare the executable target name and its source files
add_executable(my_app 
    src/main.c 
    src/utils.c
)

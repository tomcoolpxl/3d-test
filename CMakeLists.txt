cmake_minimum_required(VERSION 3.20)
project(3d-test LANGUAGES C CXX)

# Allow FetchContent dependencies that declare cmake_minimum_required < 3.5
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

# glad2
FetchContent_Declare(
    glad2
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v2.0.4
    SOURCE_SUBDIR  cmake
)
FetchContent_MakeAvailable(glad2)
glad_add_library(glad_gl45 REPRODUCIBLE LOADER API gl:core=4.5)

# Sources
add_executable(3d-test
    src/main.cpp
    src/core/Camera.cpp
    src/platform/Window.cpp
    src/platform/Input.cpp
    src/rendering/Shader.cpp
    src/rendering/Mesh.cpp
)

target_include_directories(3d-test PRIVATE src)

target_compile_definitions(3d-test PRIVATE GLFW_INCLUDE_NONE)

target_link_libraries(3d-test PRIVATE
    glfw
    glm::glm
    glad_gl45
)

# Platform-specific linker flags
if(UNIX AND NOT APPLE)
    target_link_libraries(3d-test PRIVATE dl)
endif()

# Compiler warnings and optimizations
if(MSVC)
    target_compile_options(3d-test PRIVATE /W4 /O2)
else()
    target_compile_options(3d-test PRIVATE -Wall -Wextra -O3)
endif()

# Copy shaders next to the binary
add_custom_command(TARGET 3d-test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:3d-test>/shaders
    COMMENT "Copying shaders"
)

cmake_minimum_required (VERSION 3.10)

project(OpenGLPlayground)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Shader.cpp
        utils/NameUtils.cpp
        )

target_include_directories(${PROJECT_NAME} PRIVATE utils)

# GLFW
add_subdirectory(vendor/GLFW)
target_link_libraries(${PROJECT_NAME} glfw)

# Glad
add_subdirectory(vendor/Glad)
target_link_libraries(${PROJECT_NAME} Glad)

# glm
add_subdirectory(vendor/glm)
target_link_libraries(${PROJECT_NAME} glm)

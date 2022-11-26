cmake_minimum_required(VERSION 3.0)

set(CMAKE_TOOLCHAIN_FILE 
  "/home/aidan/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_CXX_STANDARD 20)

project(opengl-udemy)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    GLEW::GLEW
    glfw
    glm::glm
)

cmake_minimum_required(VERSION 3.30.3)

project(luna)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
  src/main.cpp 
  src/glad.c
)

target_include_directories(${PROJECT_NAME} PRIVATE dependencies/include)

find_package(glfw3 REQUIRED)

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)

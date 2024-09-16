cmake_minimum_required(VERSION 3.30.3)

project(luna)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)  # Or 17, 20 depending on your needs
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
  src/main.cpp 
  src/glad.c
  src/stb_image.cpp
  dependencies/include/tinyobjloader/tiny_obj_loader.cc
  # src/obj.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE dependencies/include)

find_package(glfw3 REQUIRED)

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)

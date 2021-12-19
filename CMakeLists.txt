cmake_minimum_required(VERSION 3.15)
project(HW5)
set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
find_package(OpenGL)
include_directories(${SFML_INCLUDE_DIRECTORIES})

add_executable(HW5 main.cpp)
target_link_libraries(HW5 PRIVATE sfml-system sfml-window sfml-graphics OpenGL)
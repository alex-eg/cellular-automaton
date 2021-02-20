cmake_minimum_required(VERSION 3.19)

project(cellular-automaton)

function(find_or_fail PACKAGE LIBRARIES)
  find_package(${PACKAGE} REQUIRED)
  target_link_libraries(${PROJECT_NAME} ${${LIBRARIES}})
endfunction()

file(GLOB source_files "*.cpp")
foreach(file ${source_files})
  list(APPEND sources ${file})
endforeach()


add_executable(${PROJECT_NAME} ${sources})

find_or_fail(SDL2 SDL2::Main)
find_or_fail(OpenGL OPENGL_LIBRARIES)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG -ggdb)
set(CMAKE_CXX_FLAGS_RELEASE -O2)

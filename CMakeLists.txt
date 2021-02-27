cmake_minimum_required(VERSION 3.19)

project(cellular-automaton)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG -ggdb)
set(CMAKE_CXX_FLAGS_RELEASE -O2)

function(find_or_fail)
  cmake_parse_arguments(find_or_fail #TODO: PARSE ARGS
)
  find_package(${PACKAGE} REQUIRED)
  foreach(lib ${LIBRARIES})
    message(STATUS "Linking with ${lib}")
    target_link_libraries(life ${lib})
  endforeach()
endfunction()

file(GLOB source_files "*.cpp")
foreach(file ${source_files})
  list(APPEND sources ${file})
endforeach()

add_executable(life ${sources})

find_or_fail(SDL2 SDL2::Main SDL2::TTF)
find_or_fail(OpenGL OpenGL::OpenGL)

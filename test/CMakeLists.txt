cmake_minimum_required(VERSION 2.8.9)
project(pomart)

include_directories(../include)
link_directories(../)

if(NOT CMAKE_VERSION VERSION_LESS 3.1)
    set(CMAKE_CXX_STANDARD 11)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(SOURCES
  src/AdjacencyRelation/Image2D/Image2DAdjacencyRelationByTranslating.cpp
  src/MorphologicalTree/MorphologicalTree.cpp
  src/MorphologicalTree/MaxTreeBuilder.cpp
  test.cpp)

include(../SetCompilerWarningAll.cmake)

add_executable(test.out ${SOURCES})
target_link_libraries(test.out pomar)
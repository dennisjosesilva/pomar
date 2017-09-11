cmake_minimum_required(VERSION 2.8.9)
project(pomar)
set(CMAKE_BUILD_TYPE Realese)

include_directories(include)

if(NOT CMAKE_VERSION VERSION_LESS 3.1)
    set(CMAKE_CXX_STANDARD 11)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()


set(SOURCES
  src/AdjacencyRelation/Image2D/Image2DAdjacencyRelationByTranslating.cpp
  src/AdjacencyRelation/AdjacencyRelation.cpp
  src/Math/Point.cpp)

include(SetCompilerWarningAll.cmake)

add_library(pomar STATIC ${SOURCES})

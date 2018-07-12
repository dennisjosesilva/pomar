cmake_minimum_required(VERSION 3.0.2)
project(pomar)
set(CMAKE_BUILD_TYPE Realese)

include_directories(include)

if(NOT CMAKE_VERSION VERSION_LESS 3.1)
    set(CMAKE_CXX_STANDARD 11)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()


set(SOURCES
  src/AdjacencyRelation/AdjacencyByTranslating.cpp
  src/AdjacencyRelation/Adjacency.cpp
  src/ComponentTree/CTBuilder.cpp
  src/Attribute/AttributeCollection.cpp
  src/ComponentTree/CTMeta.cpp
  src/Core/PixelIndexer.cpp)

include(SetCompilerWarningAll.cmake)

add_library(pomar STATIC ${SOURCES})

enable_testing()
add_subdirectory(test)

cmake_minimum_required(VERSION 3.0.2)
project(pomart)

include_directories(../include)
link_directories(../build)

if(NOT CMAKE_VERSION VERSION_LESS 3.1)
    set(CMAKE_CXX_STANDARD 11)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(SOURCES
  src/AdjacencyRelation/AdjacencyByTranslating2D.cpp
  src/ComponentTree/CTree.cpp
  src/ComponentTree/MaxTreeBuilder.cpp
  src/Math/Point.cpp
  test.cpp)

include(../SetCompilerWarningAll.cmake)
include(./ParseAndAddCatchTests.cmake)

add_executable(test.out ${SOURCES})
target_link_libraries(test.out pomar)

enable_testing()
ParseAndAddCatchTests(test.out)

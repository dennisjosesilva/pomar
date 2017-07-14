cmake_minimum_required(VERSION 2.8.9)
project(pomar)
set(CMAKE_BUILD_TYPE Realese)

include_directories(include)

file(SOURCES src/AdjacencyRelation/Image2D/Image2DAdjacencyRelationByTranslating.cpp)

add_library(pomar STATIC $(SOURCES))
#include <vector>
#include <limits>
#include <cstddef>

#ifndef ADJACENCY_RELATION_H_INCLUDED
#define ADJACENCY_RELATION_H_INCLUDED

namespace pomar
{
  /**
  * Abstract class which represents an graph adjacency
  * stored in a array such that the indices of this array are vertex
  * identification.
  */
  class Adjacency
  {
  public:
    /**
    * Special value to represent a vertex out of set (-1).
    */
    static const int NoAdjacentIndex;

    /**
    * It returns the neighbourhood of the vertex identified by id. If
    * the return has a element out of the vertex set it will be denoted
    * with value NoAdjacentIndex (-1).
    */
    virtual const std::vector<int>& neighbours(int id) = 0;
  };
}

#endif

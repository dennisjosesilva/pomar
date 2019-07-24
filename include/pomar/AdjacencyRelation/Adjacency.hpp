#include <vector>
#include <limits>
#include <cstddef>

#ifndef ADJACENCY_RELATION_H_INCLUDED
#define ADJACENCY_RELATION_H_INCLUDED

/** @file */

namespace pomar
{
  /**
  * Abstract class which represents a graph adjacency
  * stored in an array such that the indices of this array are vertex
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
    * the return has an element out of the vertex set it will be denoted
    * with value NoAdjacentIndex (-1).
    */
    virtual const std::vector<int>& neighbours(int id) = 0;


    /**
    * Virtual destructor for the pure virtual class Adjacency
    **/
    virtual ~Adjacency() {}
  };
}

#endif

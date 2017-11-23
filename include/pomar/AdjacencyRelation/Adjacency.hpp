#include <vector>
#include <limits>
#include <cstddef>

#ifndef ADJACENCY_RELATION_H_INCLUDED
#define ADJACENCY_RELATION_H_INCLUDED

namespace pomar
{
  class Adjacency
  {
  public:
    static const int NoAdjacentIndex;
    virtual const std::vector<int>& neighbours(int id) = 0;
  };
}

#endif

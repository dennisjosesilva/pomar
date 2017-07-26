#include <vector>
#include <limits>
#include <cstddef>

#ifndef ADJACENCY_RELATION_H_INCLUDED
#define ADJACENCY_RELATION_H_INCLUDED

namespace pomar
{
  class AdjacencyRelation
  {
  public:
    static const int NoAdjacentIndex = -1;
    virtual const std::vector<int>& getAdjacentElements(int curElem) = 0;
  };

  
}

#endif

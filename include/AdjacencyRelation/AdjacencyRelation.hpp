#include <vector>
#include <cstddef>

#ifndef ADJACENCY_RELATION_H_INCLUDED
#define ADJACENCY_RELATION_H_INCLUDED

namespace pomar
{
  class AdjacencyRelation
  {
  public:
    virtual const std::vector<int>& getAdjacentElements(size_t curElem) = 0;
  };
}

#endif

#include <pomar/AdjacencyRelation/Adjacency.hpp>
#include <pomar/Math/Point.hpp>
#include <memory>

#ifndef IMAGE2D_ADJACENCY_RELATION_BY_TRANSLATING_H_INCLUDED
#define IMAGE2D_ADJACENCY_RELATION_BY_TRANSLATING_H_INCLUDED

namespace pomar
{
  class AdjacencyByTranslating2D: public AdjacencyRelation
  {
  public:
    AdjacencyByTranslating(int width, int height, const std::vector<IPoint2D>& t);
    AdjacencyByTranslating(int width, int height, std::initializer_list<IPoint2D>& t);
    const std::vector<int>& neighbours(int id);

    static std::unique_ptr<AdjacencyByTranslating2D> createAdjacency4(int width, int height);
    static std::unique_ptr<AdjacencyByTranslating2D> createAdjacency8(int width, int height);

  private:
    int _width;
    int _height;
    std::vector<IPoint2D> _t;
    std::vector<int> _neighbours;
  };
}


#endif

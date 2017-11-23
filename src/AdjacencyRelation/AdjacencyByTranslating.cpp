#include <pomar/AdjacencyRelation/AdjacencyByTranslating.hpp>

namespace pomar
{
  AdjacencyByTranslating2D::
  AdjacencyByTranslating2D(int width, int height, const std::vector<IPoint2D>& t)
    :_width(width), _height(height), _t(t), _neighbours(t.size())
  {}

  AdjacencyByTranslating2D::
  AdjacencyByTranslating2D(int width, int height, std::initializer_list<IPoint2D>& t)
    :_width(width), _height(height), _t(t), _neighbours(t.size())
  {}

  const std::vector<int>&
  AdjacencyByTranslating2D::neighbours(int id)
  {
    auto px = id % _width;
    auto py = id / _width;
    IPoint2D currElem(px, py);

    for (auto& n: _neighbours)
      n = Adjacency::NoAdjacentIndex;

    auto j = 0;
    for (size_t i = 0; i < _neighbours.size(); ++i) {
      auto q = currElem + _t[i];

      if (q.x() >= 0 && q.x() < _width && q.y() >= 0 && q.y() < _height)
	      _neighbours[j++] = q.y() * _width + q.x();

    }

    return _neighbours;
  }

  std::unique_ptr<AdjacencyByTranslating2D>
  AdjacencyByTranslating2D::createAdjacency4(int width, int height)
  {
    std::vector<IPoint2D> t{{-1,0}, {0,-1}, {1, 0}, {0, 1}};
    return std::unique_ptr<AdjacencyByTranslating2D>(new AdjacencyByTranslating2D(width, height, t));
  }

  std::unique_ptr<AdjacencyByTranslating2D>
  AdjacencyByTranslating2D::createAdjacency8(int width, int height)
  {
    std::vector<IPoint2D> t{{-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}};
    return std::unique_ptr<AdjacencyByTranslating2D>(new AdjacencyByTranslating2D(width, height, t));
  }
}

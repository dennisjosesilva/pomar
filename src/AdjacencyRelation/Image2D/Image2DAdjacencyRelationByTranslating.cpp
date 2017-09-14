#include <pomar/AdjacencyRelation/Image2D/Image2DAdjacencyRelationByTranslating.hpp>

namespace pomar
{
  Image2DAdjacencyRelationByTranslating::
  Image2DAdjacencyRelationByTranslating(int width, int height, const std::vector<IPoint2D>& dt)
    :_width(width), _height(height), _dt(dt), _neighbours(dt.size())
  {}

  Image2DAdjacencyRelationByTranslating::
  Image2DAdjacencyRelationByTranslating(int width, int height, std::initializer_list<IPoint2D>& dt)
    :_width(width), _height(height), _dt(dt), _neighbours(dt.size())
  {}
    
  const std::vector<int>&
  Image2DAdjacencyRelationByTranslating::getAdjacentElements(int curElem)
  {
    auto px = curElem % _width;
    auto py = curElem / _width;
    IPoint2D currElem(px, py);
    
    for (auto& n: _neighbours)
      n = AdjacencyRelation::NoAdjacentIndex;

    auto j = 0;
    for (size_t i = 0; i < _neighbours.size(); ++i) {
      auto q = currElem + _dt[i];

      if (q.x() >= 0 && q.x() < _width && q.y() >= 0 && q.y() < _height)
	_neighbours[j++] = q.y() * _width + q.x();
      
    }

    return _neighbours;
  }

  std::unique_ptr<Image2DAdjacencyRelationByTranslating>
  Image2DAdjacencyRelationByTranslating::createFourConnectedAdjacency(int width, int height)
  {
    std::vector<IPoint2D> dt{{-1,0}, {0,-1}, {1, 0}, {0, 1}};
    return std::unique_ptr<Image2DAdjacencyRelationByTranslating>(new Image2DAdjacencyRelationByTranslating(width, height, dt));
  }
  
  std::unique_ptr<Image2DAdjacencyRelationByTranslating>
  Image2DAdjacencyRelationByTranslating::createEightConnectedAdjacency(int width, int height)
  {
    std::vector<IPoint2D> dt{{-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}};
    return std::unique_ptr<Image2DAdjacencyRelationByTranslating>(new Image2DAdjacencyRelationByTranslating(width, height, dt));
  }
}

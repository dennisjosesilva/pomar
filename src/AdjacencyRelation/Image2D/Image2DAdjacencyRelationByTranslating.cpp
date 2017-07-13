#include <AdjacencyRelation/Image2D/Image2DAdjacencyRelationByTranslating.hpp>

namespace pomar
{
  Image2DAdjacencyRelationByTranslating(int width, int height, const std::vector<int>& dx, const std::vector<int>& dy)
    :_width(width), _height(height), _dx(dx), _dy(dy), _neighbours(dx.size())
  {}

  Image2DAdjacencyRelationByTranslating(int width, int height, std::initializer_list<int> dx, std::initializer_list<int> dy)
    :_width(width), _height(height), _dx(dx), _dy(dy), _neighbours(dx.size())
  {}
    
  const std::vector<int>& getAdjacentElements(size_t curElem)
  {
    auto px = curElem % _width;
    auto py = curElem / _width;

    for (auto& n: _neighbours)
      n = -1;

    auto j = 0;
    for (auto i = 0; i < neighbours.size(); ++i) {
      auto qx = px + _dx[i];
      auto qy = py + _dy[i];

      if (qx >= 0 && qx < width && qy >= 0 && qy < height)
	neighbours[i] = qy * width + qx;
    }

    return neighbours;
  }

  std::unique_ptr<Image2DAdjacencyRelationByTranslating> createFourConnectedAdjacency(int width, int height)
  {
    std::vector<int> dx = {-1,0,1,0};
    std::vector<int> dy = {0,-1,0,1};
    return std::unique_ptr<ImageAdjacencyRelationByTranslating>(new ImageAdjacencyRelationByTranslating(width, height, dx, dy));
  }
  
  std::unique_ptr<Image2DAdjacencyRelationByTranslating> createEightConnectedAdjacency(int width, int height)
  {
    std::vector<int> dx = {-1,0,1,0,-1,1,1,-1};
    std::vector<int> dy = {0,-1,0,1,-1,-1,1,1};
    return std::unique_ptr<ImageAdjacencyRelationByTranslating>(new ImageAdjacencyRelationByTranslating(width, height, dx, dy));
  }
}

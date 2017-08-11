#include "../AdjacencyRelation.hpp"
#include <memory>

#ifndef IMAGE2D_ADJACENCY_RELATION_BY_TRANSLATING_H_INCLUDED
#define IMAGE2D_ADJACENCY_RELATION_BY_TRANSLATING_H_INCLUDED

namespace pomar
{
  class Image2DAdjacencyRelationByTranslating: public AdjacencyRelation
  {
  public:    
    Image2DAdjacencyRelationByTranslating(int width, int height, const std::vector<int>& dx, const std::vector<int>& dy);
    Image2DAdjacencyRelationByTranslating(int width, int height, std::initializer_list<int> _dx, std::initializer_list<int> dy);
    const std::vector<int>& getAdjacentElements(int curElem);

    static std::unique_ptr<Image2DAdjacencyRelationByTranslating> createFourConnectedAdjacency(int width, int height);
    static std::unique_ptr<Image2DAdjacencyRelationByTranslating> createEightConnectedAdjacency(int width, int height);
    
  private:
    size_t _width;	    			  
    size_t _height;
    std::vector<int> _dx;
    std::vector<int> _dy;
    std::vector<int> _neighbours;
  };
}


#endif


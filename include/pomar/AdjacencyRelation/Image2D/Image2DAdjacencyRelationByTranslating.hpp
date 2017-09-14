#include <pomar/AdjacencyRelation/AdjacencyRelation.hpp>
#include <pomar/Math/Point.hpp>
#include <memory>

#ifndef IMAGE2D_ADJACENCY_RELATION_BY_TRANSLATING_H_INCLUDED
#define IMAGE2D_ADJACENCY_RELATION_BY_TRANSLATING_H_INCLUDED

namespace pomar
{
  class Image2DAdjacencyRelationByTranslating: public AdjacencyRelation
  {
  public:    
    Image2DAdjacencyRelationByTranslating(int width, int height, const std::vector<IPoint2D>& dt);
    Image2DAdjacencyRelationByTranslating(int width, int height, std::initializer_list<IPoint2D>& dt);
    const std::vector<int>& getAdjacentElements(int curElem);

    static std::unique_ptr<Image2DAdjacencyRelationByTranslating> createFourConnectedAdjacency(int width, int height);
    static std::unique_ptr<Image2DAdjacencyRelationByTranslating> createEightConnectedAdjacency(int width, int height);
    
  private:
    int _width;	    			  
    int _height;
    std::vector<IPoint2D> _dt;
    std::vector<int> _neighbours;
  };
}


#endif


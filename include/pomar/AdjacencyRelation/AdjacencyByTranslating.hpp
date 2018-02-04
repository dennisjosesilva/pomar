#include <pomar/AdjacencyRelation/Adjacency.hpp>
#include <pomar/Math/Point.hpp>
#include <memory>

#ifndef IMAGE2D_ADJACENCY_RELATION_BY_TRANSLATING_H_INCLUDED
#define IMAGE2D_ADJACENCY_RELATION_BY_TRANSLATING_H_INCLUDED

namespace pomar
{
  /**
  *  Class which implements an adjacency relation over a grid of size  \f$ width \times height \f$
  *  such that a vertex neighbourhood can be found by this vertex translation
  *  over the grid (this kind of adjacency is very common to 2D images).
  */
  class AdjacencyByTranslating2D: public virtual Adjacency
  {
  public:
    /**
    * Construct an adjacency from a grid of size width x height defined by
    * the translation points in t.
    */
    AdjacencyByTranslating2D(int width, int height, const std::vector<IPoint2D>& t);


    /**
    * Construct an adjacency from a grid of size width x height defined by
    * the translation points in t.
    */
    AdjacencyByTranslating2D(int width, int height, std::initializer_list<IPoint2D>& t);
    const std::vector<int>& neighbours(int id);

    /**
    *  Create a 4-connected adjacency defined by the cross set
    (Cr = {(-1,0), (0,-1), (1, 0), (0,1)}) for a grid of size width x height.
    */
    static std::unique_ptr<Adjacency> createAdjacency4(int width, int height);

    /**
    *  Create a 8-connected adjacency defined by the square set
    (SQ = CR U {(-1,-1), (1,-1), (1,1), (-1,1)}) for a grid of size width x height.
    */
    static std::unique_ptr<Adjacency> createAdjacency8(int width, int height);

  private:
    int _width;
    int _height;
    std::vector<IPoint2D> _t;
    std::vector<int> _neighbours;
  };
}

#endif

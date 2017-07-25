#include "../AdjacencyRelation/AdjacencyRelation.hpp"
#include <vector>
#include <cstddef>
#include <limits>

namespace pomar
{
  class MorphologicalTreeBuilder
  {
  public:
    template<typename T>
    MorphologicalTree<T> build(const std::vector<T> &vertices, const AdjacencyRelation &adj);

  protected:   
    size_t findRoot(std::vector<size_t>& zpar, size_t x) const;

    template<typename T>
    virtual std::vector<size_t> sort(const std::vector<T>& vertices) const = 0;

    template<typename T>
    void canonizeTree(const std::vector<T>& vertices, std::vector<size_t> &sortedVertices, std::vector<size_t>& parent);
  };

  /* ============================ MORPHOLOGICAL TREE BUILDER MAX-TREE ================================================= */
  class MaxTreeBuilder: public MorphologicalTreeBuilder
  {
  protected:
    template<typename T>
    std::vector<size_t> sort(const std::vector<T>& vertices);
    
  private:
    template<typename T>
    std::vector<size_t> countingSort(const std::vector<T>& vertices);

    template<typename T>
    std::vector<size_t> STLsort(const std::vector<T>& vertices);
  };

  /* ====================================  IMPLEMENTATION ============================================================= */

  /*==================================== UNDEF CONST ================================================================== */
  const size_t MorphologicalTreeBuilder::UNDEF = std::numeric_limits<size_t>::max();
  
  /* ============================== MORPHOLOGICAL TREE BUILDER ======================================================== */
  template<typename T>
  MorphologicalTree<T> build(const std::vector<T> &vertices, const AdjacencyRelation &adj)
  {
    const int UNDEF = -1;
    std::vector<size_t> parent(vertices.size());
    std::vector<size_t> zpar(vertices.size());

    for (auto &p: zpar)
      p = UNDEF;

    std::vector<size_t> sortedVertices = sort(vertices);

    for (size_t i = 0; i < sortedVertices.size(); i++) {
      auto p = sortedVertices[i];
      zpar[p] = parent[p] = p;
      std::vector<size_t> neighbours = adj.getAdjacentElements(p);
    }
    
  }
  
}

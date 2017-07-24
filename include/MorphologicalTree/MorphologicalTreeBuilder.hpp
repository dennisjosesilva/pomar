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
    static const size_t UNDEF;
    
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
    std::vector<size_t> parent(vertices.size());
    std::vector<size_t> zpar(vertices.size());

    /*TODO: Implement! */
  }
  
}

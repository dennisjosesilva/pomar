#include "../AdjacencyRelation/AdjacencyRelation.hpp"
#include "MorphologicalTree.hpp"
#include <type_traits>
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
    void canonizeTree(const std::vector<T>& vertices, std::vector<size_t> &sortedVertices, std::vector<size_t>& parent) const;
  };
  
  
  /* ============================ MORPHOLOGICAL TREE BUILDER MAX-TREE ================================================= */
  class MaxTreeBuilder: public MorphologicalTreeBuilder
  {
  protected:
    template<typename T>
    std::vector<int> sort(const std::vector<T>& vertices);
    
  private:
    template<typename T>
    std::vector<int> countingSort(const std::vector<T>& vertices);

    template<typename T>
    std::vector<int> STLsort(const std::vector<T>& vertices);
  };

  /* ====================================  IMPLEMENTATION ============================================================= */
 
  /* ============================== MORPHOLOGICAL TREE BUILDER ======================================================== */
  /* ======================================== BUILD =================================================================== */
  template<typename T>
  MorphologicalTree<T> MorphologicalTreeBuilder::build(const std::vector<T> &vertices, const AdjacencyRelation &adj)
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
      std::vector<int> neighbours = adj.getAdjacentElements(p);
      for (auto n: neighbours) {
	if (n != AdjacencyRelation::NoAdjacentIndex && zpar[n] != UNDEF) {
	  auto r = findRoot(zpar, n);
	  if (r != p)
	    zpar[r] = parent[r] = p;
	}
      }
    }

    canonizeTree(vertices, sortedVertices, parent);
    return MorphologicalTree<T>(parent, sortedVertices, vertices);
  }

  /* ======================================== FIND ROOT ======================================================================= */
  int MorphologicalTree::findRoot(std::vector<int>& zpar, int x) 
  {
    if (zpar[x] == x)
      return x;
    else {
      zpar[x] = findRoot(zpar, zpar[x]);
      return zpar[x];
    }
  }

  /* ================================== CANONIZE TREE ========================================================================== */
  template<typename T>
  void MorphologicalTreeBuilder::canonizeTree(std::vector<T>& vertices, const std::vector<int>& sortedIndex, std::vector<int> parent) {
    for (int i = sortedIndex.size()-1; i >= 0; i--) {
      auto v = sortedIndex[i];
      auto q = parent[v];
      if (vertices[parent[q]] == vertices[q])
	parent[v] = parent[q];
    }
  }

  /* ======================================== MAX-TREE BUILDER ======================================================== */
  /* ======================================== SORT ==================================================================== */
  template<typename T>
  std::vector<size_t> MaxTreeBuilder::sort(const std::vector<T>& vertices)
  {
    if (std::is_same<T, int>::value || std::is_same<T, unsigned int>::value || std::is_same<T, char>::value ||
	std::is_same<T, unsigned char>::value || std::is_same<T, unsigned short>::value || std::is_same<T, short>::value)
      return std::move(countingSort(vertices));
   else
     return std::move(STLsort(vertices));		       
  }  

  /* ========================================== COUNTING SORT ======================================================== */
  template<typename T>
  std::vector<int> countingSort(const std::vector<T>& vertices)
  {
    T maxValue = std::numeric_limits<T>::max();
    std::vector<int> counter(max_value + 1);
    std::vector<int> idx(vertices.size());

    for (auto &c: counter)
      c = 0;

    for (auto i = 0; i < vertices.size(); i++)
      counter[maxValue - vertices[i]]++;

    for (auto i = vertices.size() - 1; i >= 0; --i)
      idx[--counter[maxValue - vertices[i]]] = i;

    return std::move(idx);
  }
  
  /* ====================================== STL SORT ================================================================= */
  template<typename T>
  std::vector<int> MaxTreeBuilder::STLsort(const std::vector<T>& vertices)
  {
    std::vector<int> idx(vertices.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(), [&vertices](int i1, int i2) { return vertices[i1] < vertices[i2]; });
    return std::move(idx);
  }
  
}

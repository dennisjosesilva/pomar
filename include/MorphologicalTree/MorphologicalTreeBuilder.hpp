#include "../AdjacencyRelation/AdjacencyRelation.hpp"
#include "MorphologicalTree.hpp"
#include <type_traits>
#include <vector>
#include <memory>
#include <cstddef>
#include <limits>

#include <iostream>

namespace pomar
{

/* ============================ MORPHOLOGICAL TREE SORTER ======================================= */
  template<class T>
  class MorphologicalTreeBuilderSorter
  {
  public:
    virtual std::vector<int> sort(const std::vector<T>& vertices) const = 0;
    
  protected:
    std::vector<int> STLsort(const std::vector<T>& vertices, std::function<bool(const T&, const T&)>) const;
  };
  
  
  /* ============================ MAX TREE SORTER ================================================= */
  template<class T>
  class MaxTreeSorter: public MorphologicalTreeBuilderSorter<T>
  {
  public:
    std::vector<int> sort(const std::vector<T>& vertices) const;
    
  protected:
    std::vector<int> countingSort(const std::vector<T>& vertices) const;
  };


  /* ================================= MORPHOLOGICAL TREE BUILDER ================================== */
  class MorphologicalTreeBuilder
  {
  public:
    enum class TreeType { MaxTree = 0, MinTree = 1 };

    template<typename T>
    MorphologicalTree<T> build(const std::vector<T> &vertices, std::unique_ptr<AdjacencyRelation> adj,
			        TreeType TreeType);
    
    template<typename T>
    MorphologicalTree<T> buildWithSorter(const std::vector<T> &vertices, std::unique_ptr<AdjacencyRelation> adj,
			        std::unique_ptr<MorphologicalTreeBuilderSorter<T>> sorter);
    
  protected:   
    int findRoot(std::vector<int>& zpar, int x) const;

    template<typename T>
    void canonizeTree(const std::vector<T>& vertices, const std::vector<int> &sortedVertices, std::vector<int>& parent) const;
  };
  

  /* ====================================  IMPLEMENTATION ============================================================= */
 
  /* ============================== MORPHOLOGICAL TREE BUILDER ======================================================== */
  /* ======================================== BUILD FROM TREE TYPE ==================================================== */
  template<typename T>
  MorphologicalTree<T> MorphologicalTreeBuilder::build(const std::vector<T> &vertices, std::unique_ptr<AdjacencyRelation> adj,
			        TreeType treeType)
  {
    switch(treeType) {
    case MorphologicalTreeBuilder::TreeType::MaxTree:
      return buildWithSorter(vertices, std::move(adj), std::unique_ptr<MorphologicalTreeBuilderSorter<T>>(new MaxTreeSorter<T>()));
    case MorphologicalTreeBuilder::TreeType::MinTree:
      return buildWithSorter(vertices, std::move(adj), std::unique_ptr<MorphologicalTreeBuilderSorter<T>>(new MaxTreeSorter<T>()));		   
    }
  }

  /* ======================================== BUILD FROM SORTER ====================================================== */
  template<typename T>
  MorphologicalTree<T> MorphologicalTreeBuilder::buildWithSorter(const std::vector<T> &vertices, std::unique_ptr<AdjacencyRelation> adj,
						        std::unique_ptr<MorphologicalTreeBuilderSorter<T>> sorter)
  {
    const int UNDEF = -1;
    std::vector<int> parent(vertices.size());
    std::vector<int> zpar(vertices.size());

    for (auto &p: zpar)
      p = UNDEF;

    std::vector<int> sortedVertices = sorter->sort(vertices);

    for (size_t i = 0; i < sortedVertices.size(); i++) {
      auto p = sortedVertices[i];
      zpar[p] = parent[p] = p;
      std::vector<int> neighbours = adj->getAdjacentElements(p);
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
    return MorphologicalTree<T>();
  }

  /* ======================================== FIND ROOT ======================================================================= */
  int MorphologicalTreeBuilder::findRoot(std::vector<int>& zpar, int x) const
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
  void MorphologicalTreeBuilder::canonizeTree(const std::vector<T>& vertices, const std::vector<int>& sortedIndex,
					      std::vector<int>& parent) const
  {
    for (int i = sortedIndex.size()-1; i >= 0; i--) {
      auto v = sortedIndex[i];
      auto q = parent[v];
      if (vertices[parent[q]] == vertices[q])
	parent[v] = parent[q];
    }
  }

  /* ======================================== MAX-TREE BUILDER ======================================================== */
  /* ======================================== SORT ==================================================================== */
  template<class T>
  std::vector<int> MaxTreeSorter<T>::sort(const std::vector<T>& vertices) const
  {
    if (std::is_same<T, int>::value || std::is_same<T, unsigned int>::value || std::is_same<T, char>::value ||
	std::is_same<T, unsigned char>::value || std::is_same<T, unsigned short>::value || std::is_same<T, short>::value)
      return std::move(countingSort(vertices));
    else                        
     return std::move(MorphologicalTreeBuilderSorter<T>::STLsort(vertices, [](const T& v1, const T& v2) { return v1 < v2; }));		       
  }  

  /* ========================================== COUNTING SORT ======================================================== */
  template<class T>
  std::vector<int> MaxTreeSorter<T>::countingSort(const std::vector<T>& vertices) const
  {
    T maxValue = std::numeric_limits<T>::max();
    std::vector<int> counter(maxValue + 1);
    std::vector<int> idx(vertices.size());

    for (auto &c: counter)
      c = 0;

    for (int i = 0; i < vertices.size(); i++)
      counter[maxValue - vertices[i]]++;
    

    for (int i = 1; i <= maxValue; i++) 
      counter[i] += counter[i - 1];
    
    for (int i = vertices.size() - 1; i >= 0; --i)
      idx[--counter[maxValue - vertices[i]]] = i;

    return std::move(idx);
  }
  
  /* ====================================== STL SORT ================================================================= */
  template<class T>
  std::vector<int> MorphologicalTreeBuilderSorter<T>::STLsort(const std::vector<T>& vertices,
							      std::function<bool(const T&, const T&)> cmp) const
  {
    std::vector<int> idx(vertices.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(), [&vertices,cmp](int i1, int i2) { return cmp(vertices[i1], vertices[i2]); });
    return std::move(idx);
  }
}

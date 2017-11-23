#include <pomar/AdjacencyRelation/Adjacency.hpp>
#include <pomar/ComponenentTree/ComponenentTree.hpp>
#include <type_traits>
#include <vector>
#include <memory>
#include <cstddef>
#include <limits>
#include <algorithm>
#include <numeric>

namespace pomar
{

/* ============================ MORPHOLOGICAL TREE SORTER ======================================= */
  template<class T>
  class CTSorter
  {
  public:
    virtual std::vector<int> sort(const std::vector<T>& elements) const = 0;

  protected:
    std::vector<int> STLsort(const std::vector<T>& elements, std::function<bool(const T&, const T&)>) const;
  };


  /* ============================ MAX TREE SORTER ================================================= */
  template<class T>
  class MaxTreeSorter: public CTSorter<T>
  {
  public:
    std::vector<int> sort(const std::vector<T>& elements) const;

  protected:
    std::vector<int> countingSort(const std::vector<T>& elements) const;
  };


  /* ================================= MORPHOLOGICAL TREE BUILDER ================================== */
  class CTBuilder
  {
  public:
    enum class TreeType { MaxTree = 0, MinTree = 1 };

    template<typename T>
    CTree<T> build(const std::vector<T> &elements, std::unique_ptr<Adjacency> adj,
			        TreeType treeType);

    template<typename T>
    CTree<T> build(const std::vector<T> &elements, std::shared_ptr<Adjacency> adj,
			       TreeType treeType);

    template<typename T>
    CTree<T> build(const std::vector<T> &elements, std::unique_ptr<Adjacency> adj,
			        std::unique_ptr<CTSorter<T>> sorter);

    template<typename T>
    MorphologicalTree<T> build(const std::vector<T> &elements, std::shared_ptr<Adjacency> adj,
			        std::unique_ptr<CTSorter<T>> sorter);

  protected:
    template<typename T>
    MorphologicalTree<T> build(const std::vector<T> &elements, Adjacency *adj,
			       CTSorter<T> *sorter);

    template<typename T>
    MorphologicalTree<T> build(const std::vector<T> &vertices, AdjacencyRelation *adj,
			       TreeType treeType);

    int findRoot(std::vector<int>& zpar, int x) const;

    template<typename T>
    void canonizeTree(const std::vector<T>& elements, const std::vector<int> &sortedIndices, std::vector<int>& parent) const;
  };


  /* ====================================  IMPLEMENTATION ============================================================= */

  /* ============================== MORPHOLOGICAL TREE BUILDER ======================================================== */

  /* ============================== BUILD FROM TREE TYPE  ============================================================== */
  template<typename T>
  MorphologicalTree<T> MorphologicalTreeBuilder::build(const std::vector<T> &elements, Adjacency *adj,
			        TreeType treeType)
  {
    switch(treeType) {
      case CTBuilder::TreeType::MaxTree:
      {
      	std::unique_ptr<CTSorter<T>> maxTreeSorter(new MaxTreeSorter<T>());
      	return build(elements, adj, maxTreeSorter.get());
      }
      case CTBuilder::TreeType::MinTree:
      {
      	std::unique_ptr<CTBuilder<T>> minTreeSorter(new MaxTreeSorter<T>());
      	return buildWithSorter(elements, adj, minTreeSorter.get());
      }
    }
    throw std::invalid_argument("invalid tree type: treeType must be a valid value of the enumeration TreeType");
  }

  template<typename T>
  MorphologicalTree<T> CTBuilder::build(const std::vector<T> &elements, std::unique_ptr<Adjacency> adj,
			        TreeType treeType)
  {
    return build(elements, adj.get(), treeType);
  }

  template<typename T>
  MorphologicalTree<T> CTBuilder::build(const std::vector<T> &elements, std::shared_ptr<Adjacency> adj,
			       TreeType treeType)
  {
    return build(elements, adj.get(), treeType);
  }

  /* ======================================= BUILD FROM SORTER OVERLOADS =============================================== */
  template<typename T>
  MorphologicalTree<T> CTBuilder::build(const std::vector<T> &elements, std::unique_ptr<Adjacency> adj,
			        std::unique_ptr<CTBuilder<T>> sorter)
  {
    return build(elements, adj.get(), sorter.get());
  }

  template<typename T>
  MorphologicalTree<T> CTBuilder::build(const std::vector<T> &elements, std::shared_ptr<Adjacency> adj,
			        std::unique_ptr<CTSorter<T>> sorter)
  {
    return build(elements, adj.get(), sorter.get());
  }

  /* ======================================== BUILDING ALGORITHM  ====================================================== */
  template<typename T>
  MorphologicalTree<T> CTBuilder::build(const std::vector<T> &elements, Adjacency *adj,
						        CTSorter<T> *sorter)
  {
    const int UNDEF = -1;
    std::vector<int> parent(elements.size());
    std::vector<int> zpar(elements.size());

    for (auto &p: zpar)
      p = UNDEF;

    std::vector<int> sortedIndices = sorter->sort(elements);

    for (size_t i = 0; i < sortedIndices.size(); i++) {
      auto p = sortedIndices[i];
      zpar[p] = parent[p] = p;
      std::vector<int> neighbours = adj->neighbours(p);
      for (auto n: neighbours) {
	       if (n != Adjacency::NoAdjacentIndex && zpar[n] != UNDEF) {
	          auto r = findRoot(zpar, n);
	          if (r != p)
	            zpar[r] = parent[r] = p;
	       }
      }
    }

    canonizeTree(elements, sortedIndices, parent);

    return CTree<T>(parent, sortedIndices, elements);
  }

  /* ======================================== FIND ROOT ======================================================================= */
  int CTBuilder::findRoot(std::vector<int>& zpar, int x) const
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
  void CTBuilder::canonizeTree(const std::vector<T>& elements, const std::vector<int>& sortedIndices,
					      std::vector<int>& parent) const
  {
    for (int i = sortedIndices.size()-1; i >= 0; i--) {
      auto v = sortedIndices[i];
      auto q = parent[v];
      if (elements[parent[q]] == elements[q])
	      parent[v] = parent[q];
    }
  }

  /* ======================================== MAX-TREE BUILDER ======================================================== */
  /* ======================================== SORT ==================================================================== */
  template<class T>
  std::vector<int> MaxTreeSorter<T>::sort(const std::vector<T>& elements) const
  {
    if (std::is_same<T, int>::value || std::is_same<T, unsigned int>::value || std::is_same<T, char>::value ||
	      std::is_same<T, unsigned char>::value || std::is_same<T, unsigned short>::value || std::is_same<T, short>::value)
      return std::move(countingSort(elements));
    else
     return std::move(CTSorter<T>::STLsort(elements, [](const T& v1, const T& v2) { return v1 < v2; }));
  }

  /* ========================================== COUNTING SORT ======================================================== */
  template<class T>
  std::vector<int> MaxTreeSorter<T>::countingSort(const std::vector<T>& elements) const
  {
    T maxValue = std::numeric_limits<T>::max();
    std::vector<int> counter(maxValue + 1);
    std::vector<int> idx(elements.size());

    for (auto &c: counter)
      c = 0;

    for (size_t i = 0; i < elements.size(); i++)
      counter[maxValue - elements[i]]++;


    for (int i = 1; i <= maxValue; i++)
      counter[i] += counter[i - 1];

    for (int i = elements.size() - 1; i >= 0; --i)
      idx[--counter[maxValue - elements[i]]] = i;

    return std::move(idx);
  }

  /* ====================================== STL SORT ================================================================= */
  template<class T>
  std::vector<int> CTSorter<T>::STLsort(const std::vector<T>& elements,
							      std::function<bool(const T&, const T&)> cmp) const
  {
    std::vector<int> idx(elements.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(), [&elements,cmp](int i1, int i2) { return cmp(vertices[i1], vertices[i2]); });
    return std::move(idx);
  }
}

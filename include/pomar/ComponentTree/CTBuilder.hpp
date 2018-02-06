#include <pomar/AdjacencyRelation/Adjacency.hpp>
#include <pomar/ComponentTree/CTree.hpp>
#include <type_traits>
#include <vector>
#include <memory>
#include <cstddef>
#include <limits>
#include <algorithm>
#include <numeric>

namespace pomar
{
  /**
  *  Abstract class which represents the sorter strategy in the component tree
  *  build algorithm.
  */
  template<class T>
  class CTSorter
  {
  public:
    /** Abstract method to be implemented by the sorter strategy. */
    virtual std::vector<int> sort(const std::vector<T>& elements) const = 0;

  protected:
    /** Useful method to encapsulate the STL sort algorithm. */
    std::vector<int> STLsort(const std::vector<T>& elements, std::function<bool(const T&, const T&)>) const;
  };

  /**
  *  Max-tree sort strategy.
  */
  template<class T>
  class MaxTreeSorter: public virtual CTSorter<T>
  {
  public:
    /**
    *   Return an array of indices orderered from the lowest element value to
    *   the highest one.
    */
    std::vector<int> sort(const std::vector<T>& elements) const;

  protected:
    /**
    *  Create a sorted array of indices orderered from the lowest element value
    *  to the highest one using the counting sort algorithm.
    */
    std::vector<int> countingSort(const std::vector<T>& elements) const;
  };

  /**
  *   Min-tree sort strategy
  */
  template<class T>
  class MinTreeSorter: public virtual CTSorter<T>
  {
  public:
    /**
    *   Return an of indices orderered fom the highest element value to the
    *   lowest one.
    */
    std::vector<int> sort(const std::vector<T>& elements) const;

  protected:
    /**
    * Create a sorted array of indices orderered from the highest element value
    * to the lowest one using the counting sort algorithm.
    */
    std::vector<int> countingSort(const std::vector<T>& elements) const;
  };

  /**
  *  Component Tree Builder.
  */
  class CTBuilder
  {
  public:
    /** Component tree types enumeration. */
    enum class TreeType {
      MaxTree = 0, /**< Max-tree */
      MinTree = 1  /**< Min-tree. */
    };

    /**
    * Build a component tree of the type treeType and the graph with the
    *   vertices equal to elements and the edges defined by the adjacency
    *   relation adj.
    */
    template<typename T>
    CTree<T> build(const std::vector<T> &elements, std::unique_ptr<Adjacency> adj,
			        TreeType treeType);

    /**
    *   Build a component tree of the type treeType and the graph with the
    *   vertices equal to elements and the edges defined by the adjacency
    *   relation adj.
    */
    template<typename T>
    CTree<T> build(const std::vector<T> &elements, std::shared_ptr<Adjacency> adj,
			       TreeType treeType);

    /**
    *   Build a component tree of the graph with the
    *   vertices equal to elements and the edges defined by the adjacency
    *   relation adj using a sort strategy.
    */
    template<typename T>
    CTree<T> build(const std::vector<T> &elements, std::unique_ptr<Adjacency> adj,
			        std::unique_ptr<CTSorter<T>> sorter);

    /**
    *   Build a component tree of the graph with the
    *   vertices equal to elements and the edges defined by the adjacency
    *   relation adj using a sort strategy.
    */
    template<typename T>
    CTree<T> build(const std::vector<T> &elements, std::shared_ptr<Adjacency> adj,
			        std::unique_ptr<CTSorter<T>> sorter);

  protected:
    template<typename T>
    CTree<T> build(const std::vector<T> &elements, Adjacency *adj,
			       CTSorter<T> *sorter);

    template<typename T>
    CTree<T> build(const std::vector<T> &elements, Adjacency *adj,
			       TreeType treeType);

    /** Algorithm find from Union-find data structure with path compression. */
    int findRoot(std::vector<int>& zpar, int x) const;

    /** Make all elements of a node point to exactly one canonical element. */
    template<typename T>
    void canonizeTree(const std::vector<T>& elements, const std::vector<int> &sortedIndices, std::vector<int>& parent) const;
  };


  /* ====================================  IMPLEMENTATION ============================================================= */

  /* ============================== COMPONENT TREE BUILDER ======================================================== */

  /* ============================== BUILD FROM TREE TYPE  ============================================================== */
  template<typename T>
  CTree<T> CTBuilder::build(const std::vector<T> &elements, Adjacency *adj,
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
      	std::unique_ptr<CTSorter<T>> minTreeSorter(new MinTreeSorter<T>());
      	return build(elements, adj, minTreeSorter.get());
      }
    }
    throw std::invalid_argument("invalid tree type: treeType must be a valid value of the enumeration TreeType");
  }

  template<typename T>
  CTree<T> CTBuilder::build(const std::vector<T> &elements, std::unique_ptr<Adjacency> adj,
			        TreeType treeType)
  {
    return build(elements, adj.get(), treeType);
  }

  template<typename T>
  CTree<T> CTBuilder::build(const std::vector<T> &elements, std::shared_ptr<Adjacency> adj,
			       TreeType treeType)
  {
    return build(elements, adj.get(), treeType);
  }

  /* ======================================= BUILD FROM SORTER OVERLOADS =============================================== */
  template<typename T>
  CTree<T> CTBuilder::build(const std::vector<T> &elements, std::unique_ptr<Adjacency> adj,
			        std::unique_ptr<CTSorter<T>> sorter)
  {
    return build(elements, adj.get(), sorter.get());
  }

  template<typename T>
  CTree<T> CTBuilder::build(const std::vector<T> &elements, std::shared_ptr<Adjacency> adj,
			        std::unique_ptr<CTSorter<T>> sorter)
  {
    return build(elements, adj.get(), sorter.get());
  }

  /* ======================================== BUILDING ALGORITHM  ====================================================== */
  template<typename T>
  CTree<T> CTBuilder::build(const std::vector<T> &elements, Adjacency *adj,
						        CTSorter<T> *sorter)
  {
    const int UNDEF = -1;
    std::vector<int> parent(elements.size(), UNDEF);
    std::vector<int> zpar(elements.size());

    auto sortedIndices = sorter->sort(elements);

    for (int i = sortedIndices.size() - 1; i >= 0; i--) {
      auto p = sortedIndices[i];
      parent[p] = zpar[p] = p;
      auto neighbours = adj->neighbours(p);
      for (auto n: neighbours) {
        if (n != Adjacency::NoAdjacentIndex && parent[n] != UNDEF) {
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
  int CTBuilder::findRoot(std::vector<int>& zpar, int p) const
  {
    if (zpar[p] != p)
      zpar[p] = findRoot(zpar, zpar[p]);
    return zpar[p];
  }

  /* ================================== CANONIZE TREE ========================================================================== */
  template<typename T>
  void CTBuilder::canonizeTree(const std::vector<T>& elements, const std::vector<int>& sortedIndices,
					      std::vector<int>& parent) const
  {
    for (size_t i = 0; i < sortedIndices.size(); i++) {
      auto p = sortedIndices[i];
      auto q = parent[p];
      if (elements[q] == elements[parent[q]])
        parent[p] = parent[q];
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
      counter[elements[i]]++;

    for (int i = 1; i <= maxValue; i++)
      counter[i] += counter[i - 1];

    for (int i = elements.size() - 1; i >= 0; --i)
      idx[--counter[elements[i]]] = i;

    return std::move(idx);
  }

  /* ====================================== STL SORT ================================================================= */
  template<class T>
  std::vector<int> CTSorter<T>::STLsort(const std::vector<T>& elements,
							      std::function<bool(const T&, const T&)> cmp) const
  {
    std::vector<int> idx(elements.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(), [&elements,cmp](int i1, int i2) { return cmp(elements[i1], elements[i2]); });
    return std::move(idx);
  }

  /* ============================== MIN-TREE SORTER ==================================================================== */
  template<class T>
  std::vector<int> MinTreeSorter<T>::sort(const std::vector<T>& elements) const
  {
    if (std::is_same<T, int>::value || std::is_same<T, unsigned int>::value || std::is_same<T, char>::value ||
	      std::is_same<T, unsigned char>::value || std::is_same<T, unsigned short>::value || std::is_same<T, short>::value)
      return std::move(countingSort(elements));
    else
     return std::move(CTSorter<T>::STLsort(elements, [](const T& v1, const T& v2) { return v1 > v2; }));
  }

  /* ========================================== COUNTING SORT ======================================================== */
  template<class T>
  std::vector<int> MinTreeSorter<T>::countingSort(const std::vector<T>& elements) const
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
}

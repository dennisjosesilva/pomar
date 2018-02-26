#include <pomar/AdjacencyRelation/Adjacency.hpp>
#include <pomar/ComponentTree/CTree.hpp>
#include <pomar/ComponentTree/CTSorter.hpp>
#include <pomar/ComponentTree/CTMeta.hpp>
#include <type_traits>
#include <vector>
#include <memory>
#include <cstddef>
#include <limits>
#include <algorithm>
#include <numeric>

#ifndef CTBUILDED_HPP_INCLUDED
#define CTBUILDED_HPP_INCLUDED

/** @file */

namespace pomar
{
  /**
  * Component Tree Builder. This class represents a general algorithm to build 
  * component tree. This class provides the build method and its overloads, such that,
  * an user can build the component tree by providing an adjacency relation and 
  * and sorting method (or a TreeType).
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
    CTree<T> build(std::shared_ptr<CTMeta> pmeta, const std::vector<T> &elements, 
      std::unique_ptr<Adjacency> adj, TreeType treeType);

    /**
    *   Build a component tree of the type treeType and the graph with the
    *   vertices equal to elements and the edges defined by the adjacency
    *   relation adj.
    */
    template<typename T>
    CTree<T> build(std::shared_ptr<CTMeta> pmeta, const std::vector<T> &elements, 
      std::shared_ptr<Adjacency> adj, TreeType treeType);

    /**
    *   Build a component tree of the graph with the
    *   vertices equal to elements and the edges defined by the adjacency
    *   relation adj using a sort strategy.
    */
    template<typename T>
    CTree<T> build(std::shared_ptr<CTMeta> pmeta, const std::vector<T> &elements, 
      std::unique_ptr<Adjacency> adj, std::function<std::vector<int>(const std::vector<T> &)> sort);

    /**
    *   Build a component tree of the graph with the
    *   vertices equal to elements and the edges defined by the adjacency
    *   relation adj using a sort strategy.
    */
    template<typename T>
    CTree<T> build(std::shared_ptr<CTMeta> pmeta, const std::vector<T> &elements, 
      std::shared_ptr<Adjacency> adj, std::function<std::vector<int>(const std::vector<T> &)> sort);

  protected:
    /** 
     * Build overload which receives an adjacency relation pointer and function for
     * sorting. 
    */
    template<typename T>
    CTree<T> build(std::shared_ptr<CTMeta> pmeta, const std::vector<T> &elements, Adjacency *adj,
			       std::function<std::vector<int>(const std::vector<T> &)> sort);

    /** Build overload which receives an adjacency relation pointer and a TreeType. */
    template<typename T>
    CTree<T> build(std::shared_ptr<CTMeta> pmeta, const std::vector<T> &elements, Adjacency *adj,
			       TreeType treeType);

    /** Algorithm find from Union-find data structure with path compression. */
    int findRoot(std::vector<int>& zpar, int x) const;

    /** Make all elements of a node point to exactly one canonical element. */
    template<typename T>
    void canonizeTree(const std::vector<T>& elements, const std::vector<int> &sortedIndices, std::vector<int>& parent) const;
  };


  /* ====================================[ IMPLEMENTATION ]============================================================= */

  /* ==============================[ COMPONENT TREE BUILDER ]======================================================== */

  /* ==============================[ BUILD FROM TREE TYPE ]============================================================== */
  template<typename T>
  CTree<T> CTBuilder::build(std::shared_ptr<CTMeta> pmeta, const std::vector<T> &elements, Adjacency *adj,
			        TreeType treeType)
  {
    switch(treeType) {
      case CTBuilder::TreeType::MaxTree:
      {      	
      	return build(pmeta, elements, adj, 
          static_cast<std::function<std::vector<int>(const std::vector<T>&)>>(&maxTreeSort<T>));
      }
      case CTBuilder::TreeType::MinTree:
      {      	
      	return build(pmeta, elements, adj, 
          static_cast<std::function<std::vector<int>(const std::vector<T>&)>>(&minTreeSort<T>));
      }
    }
    throw std::invalid_argument("invalid tree type: treeType must be a valid value of the enumeration TreeType");
  }

  template<typename T>
  CTree<T> CTBuilder::build(std::shared_ptr<CTMeta> pmeta, const std::vector<T> &elements, 
    std::unique_ptr<Adjacency> adj, TreeType treeType)
  {
    return build(pmeta, elements, adj.get(), treeType);
  }

  template<typename T>
  CTree<T> CTBuilder::build(std::shared_ptr<CTMeta> pmeta, const std::vector<T> &elements,
    std::shared_ptr<Adjacency> adj, TreeType treeType)
  {
    return build(pmeta, elements, adj.get(), treeType);
  }

  /* =======================================[ BUILD FROM SORTER OVERLOADS ]=============================================== */
  template<typename T>
  CTree<T> CTBuilder::build(std::shared_ptr<CTMeta> pmeta, const std::vector<T> &elements, 
    std::unique_ptr<Adjacency> adj, std::function<std::vector<int>(const std::vector<T> &)> sort)
  {
    return build(pmeta, elements, adj.get(), sort);
  }

  template<typename T>
  CTree<T> CTBuilder::build(std::shared_ptr<CTMeta> pmeta, const std::vector<T> &elements,
    std::shared_ptr<Adjacency> adj, std::function<std::vector<int>(const std::vector<T> &)> sort)
  {
    return build(pmeta, elements, adj.get(), sort);
  }

  /* ========================================[ BUILDING ALGORITHM ]====================================================== */
  template<typename T>
  CTree<T> CTBuilder::build(std::shared_ptr<CTMeta> pmeta, const std::vector<T> &elements, Adjacency *adj,
						        std::function<std::vector<int>(const std::vector<T> &)> sort)
  {
    const int UNDEF = -1;
    std::vector<int> parent(elements.size(), UNDEF);
    std::vector<int> zpar(elements.size());

    auto sortedIndices = sort(elements);

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

    return CTree<T>(pmeta, parent, sortedIndices, elements);
  }

  /* ========================================[ FIND ROOT ]======================================================================= */
  int CTBuilder::findRoot(std::vector<int>& zpar, int p) const
  {
    if (zpar[p] != p)
      zpar[p] = findRoot(zpar, zpar[p]);
    return zpar[p];
  }

  /* ==================================[ CANONIZE TREE ]========================================================================== */
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
}

#endif
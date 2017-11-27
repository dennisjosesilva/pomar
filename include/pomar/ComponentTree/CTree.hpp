#include <iostream>
#include <vector>
#include <limits>
#include <functional>
#include <cstddef>

#ifndef MORPHOLOGICAL_TREE_H_INCLUDED
#define MORPHOLOGICAL_TREE_H_INCLUDED

namespace pomar
{

  /**
  * This class represents a component tree node in its compact representation.
  * It stores a list of identification of the elements which
  * represent its node and the full component tree node can be reconstructed using
  * the elements of this node and the full representation of its children nodes.
  * This class is not meant to be used outside the class CTree.
  */
  template<class NT>
  class CTNode
  {
  public:
    /** Default constructor. */
    CTNode();
    /** Construct a component tree node with an id and level. */
    CTNode(int id, NT level);

    inline int id() const { return _id; } /**< Get node's id. */
    inline void id(int id) { _id = id; } /**< Set node's id.  */

    inline const NT& level() const { return _level; } /**< Get node's level. */
    inline void level(const NT& level) { _level = level; } /**< Set node's level. */

    /** Get parent node id. */
    inline int parent() const { return _parent; }
    /** Set parent node id. */
    inline void parent(int parent) { _parent = parent; }

    /** Get an array with the id for each child node. */
    inline const std::vector<int>& children() const { return _children; }
    /** Add a child node id. */
    inline void addChild(int child) { _children.push_back(child); }

    /** Get the array with the id for each element stored in this node.  */
    inline const std::vector<int>& elementIndices() const { return _elementIndices; }
    /** Add a id to the element set of this node.*/
    inline void addElementIndex(int elementIndex) { _elementIndices.push_back(elementIndex); }
  private:
    int _id;
    NT _level;
    int _parent;

    std::vector<int> _children;
    std::vector<int> _elementIndices;
  };

  /* =============== CTNODE CONTRUCTORS ========================================= */
  template<class NT>
  CTNode<NT>::CTNode()
  {}

  template<class NT>
  CTNode<NT>::CTNode(int id, NT level)
    :_id(id), _level(level)
  {}



  /**
  * This class represents a component tree using its compact representation.
  */
  template<class T>
  class CTree
  {
  public:
    /** Default constructor */
    CTree() {}

    /** Construct a component tree using the parent array, an elements set and
    *   the indices of the elements set ordered (this order define the type of
    *   the tree such as max-tree or min-tree).
    */
    CTree(const std::vector<int>& parent, const std::vector<int>& sortedIndices, const std::vector<T>& elements);

    /** Transverse the tree from the leaves to the node calling the visit callback
    *   for each node. This transverse guarantees that all children nodes are
    *   visited before it visits theirs parent node.
    */
    void transverse(std::function<void(const CTNode<T>&)> visit);

    /** Get the number of nodes of the tree. */
    inline size_t numberOfNodes() const { return _nodes.size(); }

    /** Get the level of the node identified by id. */
    inline const T& nodeLevel(int id) const { return _nodes[id].level(); }
    /** Get parent id of the node identified by the id. */
    inline int nodeParent(int id) const { return _nodes[id].parent(); }
    /** Get the children nodes id of the node identified by id. */
    inline const std::vector<int>& nodeChildren(int id) const { return _nodes[id].children(); }
    /** Returns an array with the identification of each element stored in this node. */
    inline const std::vector<int>& nodeElementIndices(int id) const { return _nodes[id].elementIndices(); }
    /** Return the id of the node which the element is stored */
    inline int nodeByElement(int element) const { return _cmap[element]; }

    /** Reconstruct the full component tree node identified by id. */
    std::vector<int> reconstructNode(int id);

    /** Prune nodes which 'shouldPrune' is true. This function removes all nodes
    *   which shouldPrune is true and all theirs decendents. This function
    *   modifies the component tree object that calls it.
    *   TODO: Cite the Lotufo's paper about Node array and Node Index
    */
    void prune(std::function<void(const CTNode<T>&)> shouldPrune);

    /** Convert component tree to the representation of a array */
    std::vector<T> convetToVector();

  private:
    void createNodes(const std::vector<int>& parent, const std::vector<int>& sortedIndices, const std::vector<T>& elements);
    void _reconstructNode(int id, std::vector<int>& rec);

  protected:
    std::vector<CTNode<T>> _nodes;
    std::vector<int> _cmap;
  };

  /* ========================= MORPHOLOGICAL TREE - TRANSVERSAL ================================ */
  template<class T>
  CTree<T>::CTree(const std::vector<int>& parent, const std::vector<int>& sortedIndices,
					       const std::vector<T>& elements)
  {
    createNodes(parent, sortedIndices, elements);
  }

  /* ========================== MORPHOLOGICAL TREE - TRANSVERSAL ================================ */
  template<class T>
  void CTree<T>::transverse(std::function<void(const CTNode<T>&)> visit)
  {
    for (int i = _nodes.size()-1; i >= 0; --i)
      visit(_nodes[i]);
  }

  /* ========================== MORPHOLOGICAL TREE - CREATE NODES =============================== */
  template<class T>
  void CTree<T>::createNodes(const std::vector<int> &parent, const std::vector<int> &sortedIndices,
					  const std::vector<T> &elements)
  {
    const int UNDEF = -1;
    std::vector<int> sortedLevelRoots;
    _cmap.resize(elements.size());

    for (auto &p: _cmap)
      p = UNDEF;

    for (int i = sortedIndices.size()-1; i >= 0; --i) {
      auto p = sortedIndices[i];
      if (elements[parent[p]] != elements[p] || parent[p] == p)
	      sortedLevelRoots.push_back(p);
    }

    _nodes.resize(sortedLevelRoots.size());

    auto& root = _nodes.front();
    auto rootCanonicalPixel = sortedLevelRoots.front();
    root.id(0);
    root.parent(-1);
    root.level(elements[rootCanonicalPixel]);
    root.addElementIndex(rootCanonicalPixel);
    _cmap[rootCanonicalPixel] = root.id();

    for (size_t i = 1; i < sortedLevelRoots.size(); ++i) {
      auto p = sortedLevelRoots[i];
      _cmap[p] = i;

      auto& node = _nodes[i];
      auto& parentNode = _nodes[_cmap[parent[p]]];
      node.parent(parentNode.id());
      node.id(i);
      node.level(elements[p]);
      node.addElementIndex(p);
      parentNode.addChild(node.id());
    }

    for (size_t i = 0; i < elements.size(); i++) {
      if (_cmap[i] == UNDEF) {
      	_cmap[i] = _cmap[parent[i]];
      	_nodes[_cmap[i]].addElementIndex(i);
      }
    }
  }

  /* ==================== COMPONENT TREE - RECONSTRUCT NODE ========================== */
  template<class T>
  std::vector<int> CTree<T>::reconstructNode(int id)
  {
    std::vector<int> rec;
    this->_reconstructNode(id, rec);
    return std::move(rec);
  }

  template<class T>
  void CTree<T>::_reconstructNode(int id, std::vector<int>& rec)
  {
    auto nodeElements = nodeElementIndices(id);
    auto children = nodeChildren(id);
    rec.insert(std::end(rec), std::begin(nodeElements), std::end(nodeElements));

    for (auto c: children)
      this->_reconstructNode(c, rec);
  }

  /* ================== COMPONENT TREE - CONVERT TO VECTOR =============================== */
  template<class T>
  std::vector<T> CTree<T>::convetToVector()
  {
    std::vector<T> v(_cmap.size());
    for (auto node: _nodes) {
      for(auto e : node.elementIndices())
        v[e] = node.level();
    }

    return v;
  }

  /* =================== PRUNNING ===================================================== */
  template<class T>
  void CTree<T>::prune(std::function<void(const CTNode<T>&)> shouldPrune)
  {
    std::vector<int> lut(_nodes.size());
    std::vector<int> nearestAncestorKept(_nodes.size());

    for (auto i = 0; i < lut.size(); i++) {
      lut[i] = i;
      nearestAncestorKept[i] = 0;
    }

    for (auto node : _nodes) {
      if (shouldPrune(node)) {
        int temp = nearestAncestorKept[node.parent()];
        nearestAncestorKept[node.id()] = temp;
        lut[node.id()] = lut[temp];
      }
      else {
        nearestAncestorKept[node.id()] = node.id();
        node.parent(nearestAncestorKept[node.parent()]);
      }
    }

  }

}

#endif

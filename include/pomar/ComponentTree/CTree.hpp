#include <iostream>
#include <vector>
#include <limits>
#include <functional>
#include <cstddef>
#include <algorithm>
#include <iostream>

#ifndef MORPHOLOGICAL_TREE_H_INCLUDED
#define MORPHOLOGICAL_TREE_H_INCLUDED

/** @file */

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
    /** Remove a child node id*/
    inline void removeChild(int child) { _children.erase(std::remove(_children.begin(), _children.end(), child), _children.end()); }
		/** Change the id of the child at position cpos */
		inline void child(int cpos, int id) { _children[cpos] = id; }

    /** Get the array with the id for each element stored in this node.  */
    inline const std::vector<int>& elementIndices() const { return _elementIndices; }
    /** Add an id to the element set of this node.*/
    inline void addElementIndex(int elementIndex) { _elementIndices.push_back(elementIndex); }
    /** Insert a range of element indices to the elements set. */
    void insertElementIndices(const std::vector<int>& indices);
  private:
    int _id;
    NT _level;
    int _parent;

    std::vector<int> _children;
    std::vector<int> _elementIndices;
  };

  /* ================ ALIASES ================================================== */
  using UCCTNode = CTNode<unsigned char>;
  using CCTNode = CTNode<char>;
  using UICTNode = CTNode<unsigned int>;
  using ICTNode = CTNode<int>;
  using ULCTNode = CTNode<unsigned long>;
  using LCTNode = CTNode<long>;
  using FCTNode = CTNode<float>;
  using DCTNode = CTNode<double>;


  /* =============== CTNODE CONTRUCTORS ========================================= */
  template<class NT>
  CTNode<NT>::CTNode()
  {}

  template<class NT>
  CTNode<NT>::CTNode(int id, NT level)
    :_id(id), _level(level)
  {}

  template<class NT>
  void CTNode<NT>::insertElementIndices(const std::vector<int>& indices)
  {
		_elementIndices.insert(_elementIndices.end(), indices.begin(), indices.end());
  }


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
    /** Get the children node ids of the node identified by id. */
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
    */
    void prune(std::function<bool(const CTNode<T>&)> shouldPrune);

    /** Convert the component tree to the array representation. */
    std::vector<T> convertToVector();

  private:
    void createNodes(const std::vector<int>& parent, const std::vector<int>& sortedIndices, const std::vector<T>& elements);
    void _reconstructNode(int id, std::vector<int>& rec);


    std::vector<bool> removeChildrenAndReturnsPrunnedNodeMap(
    		std::function<bool(const CTNode<T>&)> shouldPrune);
	 void _prune(CTNode<T>& node, std::vector<bool>& prunnedNodes);
	 void _rprune(CTNode<T>& keptNode, CTNode<T>& nodeToPrune, std::vector<bool>& prunnedNodes);
    void removePrunnedNodes(const std::vector<bool> &prunnedNodes);
    void updateChildrenIdFromPrune(const std::vector<int> &lut);
    std::vector<int> updateParentIdAndCreateLut(const std::vector<bool> &prunnedNodes);
	 void updateCmap(const std::vector<int> lut);
	 
  protected:
    std::vector<CTNode<T>> _nodes;
    std::vector<int> _cmap;
  };

  /* ============================[ ALIASES ]====================================================== */
  using UCCTree = CTree<unsigned char>;
  using CCTree = CTree<char>;
  using UICTree = CTree<unsigned int>;
  using ICTree = CTree<int>;
  using ULCTree = CTree<unsigned long>;
  using LCTree =  CTree<long>;
  using FCTree = CTree<float>;
  using DCTree = CTree<double>;

  /* =========================[ MORPHOLOGICAL TREE - TRANSVERSAL ]================================ */
  template<class T>
  CTree<T>::CTree(const std::vector<int>& parent, const std::vector<int>& sortedIndices,
					       const std::vector<T>& elements)
  {
    createNodes(parent, sortedIndices, elements);
  }

  /* ==========================[ MORPHOLOGICAL TREE - TRANSVERSAL ]================================ */
  template<class T>
  void CTree<T>::transverse(std::function<void(const CTNode<T>&)> visit)
  {
    for (int i = _nodes.size()-1; i >= 0; --i)
      visit(_nodes[i]);
  }

  /* ==========================[ MORPHOLOGICAL TREE - CREATE NODES ]=============================== */
  template<class T>
  void CTree<T>::createNodes(const std::vector<int> &parent, const std::vector<int> &sortedIndices,
					  const std::vector<T> &elements)
  {
    const int UNDEF = -1;
    std::vector<int> sortedLevelRoots;
    _cmap.resize(elements.size(), UNDEF);

    for (size_t i = 0; i < sortedIndices.size(); ++i) {
      auto p = sortedIndices[i];
      if (elements[parent[p]] != elements[p] || parent[p] == p)
	      sortedLevelRoots.push_back(p);
    }

    _nodes.resize(sortedLevelRoots.size());

    auto p = sortedLevelRoots.front();
    auto& root = _nodes[0];
    _cmap[p] = 0;
    root.id(0);
    root.level(elements[p]);
    root.addElementIndex(p);
    root.parent(UNDEF);


    for (size_t i = 1; i < sortedLevelRoots.size(); i++) {
      auto p = sortedLevelRoots[i];
      _cmap[p] = i;
      auto& node = _nodes[i];
      auto& parentNode = _nodes[_cmap[parent[p]]];
      node.id(i);
      node.parent(parentNode.id());
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

  /* ====================[ COMPONENT TREE - RECONSTRUCT NODE ]========================== */
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

  /* ==================[ COMPONENT TREE - CONVERT TO VECTOR ]=============================== */
  template<class T>
  std::vector<T> CTree<T>::convertToVector()
  {
    std::vector<T> v(_cmap.size());
    for (auto& node: _nodes) {
      for(auto e : node.elementIndices())
        v[e] = node.level();
    }

    return v;
  }

  /* ===================[ PRUNNING ]===================================================== */
  template<class T>
  void CTree<T>::prune(std::function<bool(const CTNode<T>&)> shouldPrune)
  {
    auto prunnedNodes = removeChildrenAndReturnsPrunnedNodeMap(shouldPrune);
    auto lut = updateParentIdAndCreateLut(prunnedNodes);
    removePrunnedNodes(prunnedNodes);
    updateChildrenIdFromPrune(lut);
    updateCmap(lut);
  }

  template<class T>
  std::vector<bool> CTree<T>::removeChildrenAndReturnsPrunnedNodeMap(
    std::function<bool(const CTNode<T>&)> shouldPrune)
  {
    std::vector<bool> prunnedNodes(_nodes.size(), false);
    for (auto& node : _nodes) {
	    if (!prunnedNodes[node.id()] && shouldPrune(node))
	    	_prune(node, prunnedNodes);
		}

    return prunnedNodes;
  }

  template<class T>
  void CTree<T>::_prune(CTNode<T>& node, std::vector<bool>& prunnedNodes)
  {
    auto& parent = _nodes[node.parent()];
    _rprune(parent, node, prunnedNodes);
    parent.removeChild(node.id());
  }

  template<class T>
  void CTree<T>::_rprune(CTNode<T>& keptNode, CTNode<T>& nodeToPrune, std::vector<bool>& prunnedNodes)
  {
    keptNode.insertElementIndices(nodeToPrune.elementIndices());

	 for (auto elem: nodeToPrune.elementIndices())
		_cmap[elem] = keptNode.id();

    prunnedNodes[nodeToPrune.id()] = true;
    for (auto c : nodeToPrune.children()) {
      _rprune(keptNode, _nodes[c], prunnedNodes);
    }
  }

  template<class T>
  void CTree<T>::removePrunnedNodes(const std::vector<bool> &prunnedNodes)
  {
    _nodes.erase(std::remove_if(_nodes.begin(), _nodes.end(),
        [&prunnedNodes](CTNode<T>& node) {
          return prunnedNodes[node.id()];
        }), _nodes.end());
  }

  template<class T>
  void CTree<T>::updateChildrenIdFromPrune(const std::vector<int> &lut)
  {
	 auto& root = _nodes.front();    
    auto children = root.children();
    for(size_t c = 0; c < children.size(); c++) { 
	    root.child(c, lut[children[c]]);
    }

    for (size_t i = 1; i < _nodes.size(); i++) {
        auto& node = _nodes[i];
        node.id(lut[node.id()]);
        node.parent(lut[node.parent()]);
        auto children = node.children();
		  for(size_t c = 0; c < children.size(); c++) { 
	        node.child(c, lut[children[c]]);
        }
		}
  }

  template<class T>
  std::vector<int> CTree<T>::updateParentIdAndCreateLut(
    const std::vector<bool> &prunnedNodes)
  {
    std::vector<int> lut(prunnedNodes.size());
    int count = 0;
    for (size_t i = 1; i < _nodes.size(); i++) {
      auto& node = _nodes[i];
      if (prunnedNodes[i]) {
        count++;
        lut[i] = -1;
      }
	   else
		  lut[i] = i - count;
    }
    return lut;
  }

  template<class T>
  void CTree<T>::updateCmap(const std::vector<int> lut)
  {
     for(auto &c: _cmap)
        c = lut[c];
  }

  //END PRUNE ALGORITHM
}

#endif

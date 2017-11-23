#include <iostream>
#include <vector>
#include <limits>
#include <functional>
#include <cstddef>

#ifndef MORPHOLOGICAL_TREE_H_INCLUDED
#define MORPHOLOGICAL_TREE_H_INCLUDED

namespace pomar
{
  template<class NT>
  class CTNode
  {
  public:
    CTNode();
    CTNode(int id, NT level);

    inline int id() const { return _id; }
    inline void id(int id) { _id = id; }

    inline const NT& level() const { return _level; }
    inline void level(const NT& level) { _level = level; }

    inline int parent() const { return _parent; }
    inline void parent(int parent) { _parent = parent; }

    inline const std::vector<int>& children() const { return _children; }
    inline void addChild(int child) { _children.push_back(child); }

    inline const std::vector<int>& elementIndices() const { return _elementIndices; }
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


  /* =================== ComponentTree TREE ==================================== */
  template<class T>
  class CTree
  {
  public:
    CTree() {}
    CTree(const std::vector<int>& parent, const std::vector<int>& sortedIndices, const std::vector<T>& elements);

    void transverse(std::function<void(const CTNode<T>&)> visit);

    inline size_t numberOfNodes() const { return _nodes.size(); }
    inline const T& nodeLevel(int id) const { return _nodes[id].level(); }
    inline int nodeParent(int id) const { return _nodes[id].parent(); }
    inline const std::vector<int>& nodeChildren(int id) const { return _nodes[id].children(); }
    inline const std::vector<int>& nodeElementIndices(int id) const { return _nodes[id].elementIndices(); }

    std::vector<int> reconstructNode(int id);

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

  /* ==================== MORPHOLOGICAL TREE - RECONSTRUCT NODE ========================== */
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
}

#endif

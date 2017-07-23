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
  class MTNode
  {
  public:
    MTNode();
    MTNode(size_t id, NT level);

    inline size_t id() const { return _id; }
    inline void id(size_t id) { _id = id; }
    
    inline const NT& level() const { return _level; }
    inline void level(const NT& level) { _level = level; }

    inline size_t parent() const { return _parent; }
    inline void parent(size_t parent) { _parent = parent; }

    inline const std::vector<size_t>& children() const { return _children; }
    inline void addChild(size_t child) { _children.push_back(child); }
    
    inline const std::vector<size_t>& vertexIndexes() const { return _vertexIndexes; }
    inline void addVertexIndex(size_t vertexIndex) { _vertexIndexes.push_back(vertexIndex); }
  private:
    size_t _id;
    NT _level;
    size_t _parent;
    
    std::vector<size_t> _children;
    std::vector<size_t> _vertexIndexes;
  };

  /* =============== MTNODE CONTRUCTORS ========================================= */
  template<class NT>
  MTNode<NT>::MTNode()
  {}

  template<class NT>
  MTNode<NT>::MTNode(size_t id, NT level)
    :_id(id), _level(level)
  {}


  /* =================== MORPHOLOGICAL TREE ==================================== */
  template<class T>
  class MorphologicalTree
  {
  public:
    MorphologicalTree(const std::vector<size_t>& parent, const std::vector<size_t>& sortedIndex, const std::vector<T>& vertices);

    void transverseFromLeavesToRoot(std::function<void(const MTNode<T>&)> visit);
    inline size_t getNumberofNodes() const { return _nodes.size(); }
    
  private:
    void createNodes(const std::vector<size_t>& parent, const std::vector<size_t>& sortedIndex, const std::vector<T>& vertices); 
  protected:
    std::vector<MTNode<T>> _nodes;
    std::vector<size_t> _cmap;
  };

  /* ========================= MORPHOLOGICAL TREE - TRANSVERSAL ================================ */
  template<class T>
  MorphologicalTree<T>::MorphologicalTree(const std::vector<size_t>& parent, const std::vector<size_t>& sortedIndex,
					       const std::vector<T>& vertices)
  {
    createNodes(parent, sortedIndex, vertices);
  }
  
  /* ========================== MORPHOLOGICAL TREE - TRANSVERSAL ================================ */
  template<class T>
  void MorphologicalTree<T>::transverseFromLeavesToRoot(std::function<void(const MTNode<T>&)> visit)
  {
    for (auto i = _nodes.size(); i > 0; --i)
      visit(_nodes[i-1]);
  }
						       
  /* ========================== MORPHOLOGICAL TREE - CREATE NODES =============================== */
  template<class T>
  void MorphologicalTree<T>::createNodes(const std::vector<size_t> &parent, const std::vector<size_t> &sortedIndex,
					  const std::vector<T> &vertices)
  {
    const size_t UNDEF = std::numeric_limits<size_t>::max();
    std::vector<size_t> sortedLevelRoots;
    _cmap.resize(vertices.size());
   
    for (auto &p: _cmap)
      p = UNDEF;

    for (int i = sortedIndex.size(); i > 0; --i) {
      auto p = sortedIndex[i-1];
      if (vertices[parent[p]] != vertices[p] || parent[p] == p)
	sortedLevelRoots.push_back(p);
    }

   
    _nodes.resize(sortedLevelRoots.size());

    auto& root = _nodes.front();
    auto rootCanonicalPixel = sortedLevelRoots.front();
    root.id(0);
    root.level(vertices[rootCanonicalPixel]);
    root.addVertexIndex(rootCanonicalPixel);
    _cmap[rootCanonicalPixel] = root.id();

    auto k = 1;
    for (size_t i = sortedLevelRoots.size(); i > 0; --i) {
      auto p = sortedLevelRoots[i-1];
      _cmap[p] = i-1;
      
      auto& node = _nodes[i-1];
      auto& parentNode = _nodes[_cmap[parent[p]]];
      node.parent(parentNode.id());
      node.id(k++);
      node.level(vertices[p]);
      node.addVertexIndex(p);
      parentNode.addChild(node.id());
    }

    for (size_t i = 0; i < vertices.size(); i++) {
      if (_cmap[i] == UNDEF) {
	_cmap[i] = _cmap[parent[i]];
	_nodes[_cmap[i]].addVertexIndex(i);
      }
    }
  }
  
}

#endif

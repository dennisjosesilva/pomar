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
    MTNode(int id, NT level);

    inline int id() const { return _id; }
    inline void id(int id) { _id = id; }
    
    inline const NT& level() const { return _level; }
    inline void level(const NT& level) { _level = level; }

    inline int parent() const { return _parent; }
    inline void parent(int parent) { _parent = parent; }

    inline const std::vector<int>& children() const { return _children; }
    inline void addChild(int child) { _children.push_back(child); }
    
    inline const std::vector<int>& vertexIndexes() const { return _vertexIndexes; }
    inline void addVertexIndex(int vertexIndex) { _vertexIndexes.push_back(vertexIndex); }
  private:
    int _id;
    NT _level;
    int _parent;
    
    std::vector<int> _children;
    std::vector<int> _vertexIndexes;
  };

  /* =============== MTNODE CONTRUCTORS ========================================= */
  template<class NT>
  MTNode<NT>::MTNode()
  {}

  template<class NT>
  MTNode<NT>::MTNode(int id, NT level)
    :_id(id), _level(level)
  {}


  /* =================== MORPHOLOGICAL TREE ==================================== */
  template<class T>
  class MorphologicalTree
  {
  public:
    MorphologicalTree() {}
    MorphologicalTree(const std::vector<int>& parent, const std::vector<int>& sortedIndex, const std::vector<T>& vertices);

    void transverseFromLeavesToRoot(std::function<void(const MTNode<T>&)> visit);
    inline size_t getNumberofNodes() const { return _nodes.size(); }
    
  private:
    void createNodes(const std::vector<int>& parent, const std::vector<int>& sortedIndex, const std::vector<T>& vertices); 
  protected:
    std::vector<MTNode<T>> _nodes;
    std::vector<size_t> _cmap;
  };

  /* ========================= MORPHOLOGICAL TREE - TRANSVERSAL ================================ */
  template<class T>
  MorphologicalTree<T>::MorphologicalTree(const std::vector<int>& parent, const std::vector<int>& sortedIndex,
					       const std::vector<T>& vertices)
  {
    createNodes(parent, sortedIndex, vertices);
  }
  
  /* ========================== MORPHOLOGICAL TREE - TRANSVERSAL ================================ */
  template<class T>
  void MorphologicalTree<T>::transverseFromLeavesToRoot(std::function<void(const MTNode<T>&)> visit)
  {
    for (int i = _nodes.size()-1; i >= 0; --i)
      visit(_nodes[i]);
  }
						       
  /* ========================== MORPHOLOGICAL TREE - CREATE NODES =============================== */
  template<class T>
  void MorphologicalTree<T>::createNodes(const std::vector<int> &parent, const std::vector<int> &sortedIndex,
					  const std::vector<T> &vertices)
  {
    const int UNDEF = -1;
    std::vector<int> sortedLevelRoots;
    _cmap.resize(vertices.size());
   
    for (auto &p: _cmap)
      p = UNDEF;

    for (int i = sortedIndex.size()-1; i >= 0; --i) {
      auto p = sortedIndex[i];
      if (vertices[parent[p]] != vertices[p] || parent[p] == p)
	sortedLevelRoots.push_back(p);
    }

   
    _nodes.resize(sortedLevelRoots.size());

    auto& root = _nodes.front();
    auto rootCanonicalPixel = sortedLevelRoots.front();
    root.id(0);
    root.parent(-1);
    root.level(vertices[rootCanonicalPixel]);
    root.addVertexIndex(rootCanonicalPixel);
    _cmap[rootCanonicalPixel] = root.id();

    for (size_t i = sortedLevelRoots.size()-1; i > 0; --i) {
      auto p = sortedLevelRoots[i];
      _cmap[p] = i;
      
      auto& node = _nodes[i];
      auto& parentNode = _nodes[_cmap[parent[p]]];
      node.parent(parentNode.id());
      node.id(i);
      node.level(vertices[p]);
      node.addVertexIndex(p);
      parentNode.addChild(node.id());
    }

    for (int i = 0; i < vertices.size(); i++) {
      if (_cmap[i] == UNDEF) {
	_cmap[i] = _cmap[parent[i]];
	_nodes[_cmap[i]].addVertexIndex(i);
      }
    }
  }  
}

#endif

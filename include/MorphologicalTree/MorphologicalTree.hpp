#include <vector>
#include <limits>

#ifndef MORPHOLOGICAL_TREE_H_INCLUDED
#define MORPHOLOGICAL_TREE_H_INCLUDED

namespace pomar
{
  template<class T>
  class MTNode
  {
  public:
    MTNode();
    MTNode(int id, T level);

    inline int id() const { return _id; }
    inline void id(size_t id) { _id = id; }
    
    inline const T& level() const { return _level; }
    inline void level(const T& level) { _level = level; }

    inline int parent() const { return _parent; }
    inline void parent(size_t parent) { _parent = parent);

    inline const std::vector<size_t>& children() const { return _children; }
    inline void addChild(size_t child) { _children.push_back(child); }
    
    inline const std::vector<size_t>& vertexIndexes() const { return _vertexIndexes; }
    inline void addVertexIndex(int size_t) { _vertexIndexes.push_back(vertexIndex); }
  private:
    size_t _id;
    T _level;
    size_t _parent;
    
    std::vector<size_t> _children;
    std::vector<size_t> _vertexIndexes;
  };

  /* =================== MORPHOLOGICAL TREE ==================================== */
  template<class T>
  class MorphologicalTree
  {
  public:
    MorphologicalTree(const std::vector<size_t>& parent, const std::vector<size_t>& sortedIndex, const std::vector<T>& vertices);

    void transverseFromLeavesToRoot(std::function<void(const MTNode<T>&)> visit);
    inline size_t getNumberofNodes() { return nodes.size(); }
    
  private:
    void createNodes(const std::vector<size_t>& parent, const std::vector<size_t>& sortedIndex, const std::vector<T>& vertices); 
  protected:
    std::vector<MTNode<T>> _nodes;
    std::vector<size_t> _cmap;
  };

  /* ========================== MORPHOLOGICAL TREE - TRANSVERSAL ================================ */
  template<class T>
  void MorphologicalTree<T>::transverseFromLeavesToRoot(std::function<void(const MTNode<T>&> visit))
  {
    for (i = nodes.size(); i > 0; --i)
      visit(nodes[i-1]);
  }
						       
  /* ========================== MORPHOLOGICAL TREE - CREATE NODES =============================== */
  template<class T>
  void MorphologicalTr0ee<T>::createNodes(const std::vector<size_t> &parent, const std::vector<size_t> &sortedIndex,
					  const std::vector<T> &vertices)
  {
    const size_t UNDEF = std::numeric_limits<size_t>::max();
    std::vector<size_t> sortedLevelRoots;
    cmap.resize(vertices.size());

    for (auto &p: cmap)
      p = UNDEF;

    for (int i = sortedIndex.size(); i > 0; --i) {
      auto p = sortedIndex[i-1];
      if (vertices[parent[p]] != vertices[p] || parent[p] == p)
	sortedLevelRoots.push_back(p);
    }

    nodes.resize(sortedLevelRoots.size());

    auto& root = nodes.front();
    auto rootCanonicalPixel = sortedLevelRoots;
    root.id(0);
    root.level(vertices[rootCanonicalPixel]);
    root.addVertexIndex(rootCanonicalPixel);
    cmap[rootCanonicalPixel] = root.id;

    for (size_t i = 1; i < sortedLevelRoots.size(); ++i) {
      auto p = sortedLevelRoots[i];
      cmap[p] = i;

      auto& node = nodes[i];
      auto& parentNode = nodes[cmap[parent[p]]];
      node.parent(parentNode.id);
      node.id(i);
      node.level(vertices[p]);
      node.addVertexIndex(p);
      parentNodew.addChild(node.id);
    }

    for (size_t i = 0; i < vertices.size(); i++) {
      if (cmap[i] == UNDEF) {
	cmap[i] = cmap[parent[i]];
	nodes[cmap[i]].addVertexIndex(i);
      }
    }
  }
  
}

#endif

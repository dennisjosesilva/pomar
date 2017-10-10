#ifndef COMPONENT_GRAPH_H_INCLUDED
#define COMPONENT_GRAPH_H_INCLUDED

namespace pomar{
  template<class NT>
  class CGNode
  {
  public:
    CGNode();
    CGNode(int id, int adjIndex, NT level);

    inline int id() const { return _id; }
    inline void id(int id) { _id = id; }

    inline int adjIndex() const { return _adjIndex; }
    inline void adjIndex(int adjIndex) { _adjIndex = adjIndex; }

    inline const NT& level() const { return _level; }
    inline void level(const NT& level) { _level = level; }

    inline const std::vector<int>& parents() const { return _parents; }
    inline void addParent(int parent) { _parents.push_back(parent); }

    inline const std::vector<int>& children() const { return _children; }
    inline void addChild(int child) { _children.push_back(child); }

    inline const std::vector<int>& composites() const { return _composites; }
    inline void addComposite(int composite) { _composites.push_back(composite); }

    inline const std::vector<int>& partials() const { return _partials; }
    inline void addPartial(int partial) { _partials.push_back(partial); }

    inline const std::vector<int>& pixels() const { return _pixels; }
    inline void addPixel(int pixel) { _pixels.push_back(pixel); }

  private:
    int _id;
    int _adjIndex;
    NT _level;

    std::vector<int> _parents;
    std::vector<int> _children;
    std::vector<int> _composites;
    std::vector<int> _partials;

    std::vector<int> _pixels;
  };

  /* =============== CGNODE CONTRUCTORS ==================================== */
  template<class NT>
  CGNode<NT>::CGNode()
  {}

  template<class NT>
  CGNode<NT>::CGNode(int id, int adjIndex, NT level)
    :_id(id), _adjIndex(adjIndex), _level(level)
  {}

  /* =================== COMPONENT GRAPH =================================== */

}

#endif

namespace pomar{
  template<class NT>
  class CGNode
  {
  public:
    CGNode();
    CGNode(int id, int adjIndex, NT level);

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

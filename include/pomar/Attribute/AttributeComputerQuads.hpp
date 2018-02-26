#include <pomar/Core/PixelIndexer.hpp>
#include <pomar/Attribute/AttributeComputer.hpp>

#include <vector>

namespace pomar
{
  template<class T>
  class AttributeComputerQuads
  {
  public:
    AttributeComputerQuads(std::string resource = "./resource/pomar/");
    void setUp(AttributeCollection &attrs, const CTree<T> &ct);
    void preProcess(AttributeCollection &attrs, const CTNode<T> &node);
    void merge(AttributeCollection &attrs, const CTNode<T> &node, const CTNode<T> &parent);
    void postProcess(AttributeCollection &attrs, const CTNode<T> &node);

    AttributeCollection compute(const CTree<T> &ct);

    std::unique_ptr<IncrementalAttributeComputer<T>> toIncrementalAttributeComputer();
  private:
    bool isLower(const IPoint2D &p, const IPoint2D &q);
    bool isGreater(const IPoint2D &p, const IPoint2D &q);
    bool isEqual(const IPoint2D &p, const IPoint2D &q);

  private:
    
    int _p1, _p2, _p3, _pd, _p4, _p1t, _p2t, _p3t, _ptd;
    std::vector<IPoint2D> _window;
    std::vector<std::vector<unsigned char>> _dt;
    std::vector<T> _f;

  };

  /* ------------------------- [ ATTRIBUTE COMPUTER QUADS ] ------------------------------- */
  template<class T>
  AttributeComputerQuads::AttributeComputerQuads(std::string resource)
  {
    _window.insert(_window.end(),{IPoint2D{-1,-1}, IPoint2D{0,-1}, IPoint2D{1,-1}, 
      IPoint2D{-1,0}, IPoint2D{1,0}, IPoint2D{-1,1}, IPoint2D{0,1}, IPoint2D{1,1}};
    /* _dt = read decision tree; */

  }

  template<class T>
  void AttributeComputerQuads<T>::setUp(AttributeCollection &attrs, const CTree<T> &ct)
  {
    auto n = ct.numberOfNodes();
    _f = ct.convertToVector();
    attrs.push(AttrType::QUADS_Q1, n); attrs.push(AttrType::QUADS_Q2, n); attrs.push(AttrType::QUADS_QD);
    attrs.push(AttrType::QUADS_Q3, n); attrs.push(AttrType::QUADS_Q4, n);

    _p1 = attrs.attrIndex(AttrType::QUADS_Q1); _p2 = attrs.attrIndex(AttrType::QUADS_Q2);
    _pd = attrs.attrIndex(AttrType::QUADS_QD); _p3 = attrs.attrIndex(AttrType::QUADS_Q3);
    _p4 = attrs.attrIndex(AttrType::QUADS_Q4);
  }

  template<class T>
  void AttributeComputerQuads<T>::preProcess(AttributeCollection &attrs, const CTNode<T> &node)
  {}

  template<class T>
  void AttributeComputerQuads<T>::merge(AttributeCollection &attrs, const CTNode<T> &node, 
    const CTNode<T> &parent)
  {}

  template<class T>
  void AttributeComputerQuads<T>::postProcess(AttributeCollection &attrs, const CTNode<T> &node)
  {}

  template<class T>
  AttributeCollection AttributeComputerQuads<T>::compute(const CTree<T> &ct)
  {}

  template<class T>
  std::unique_ptr<IncrementalAttributeComputer<T>> toIncrementalAttributeComputer()
  {}
}
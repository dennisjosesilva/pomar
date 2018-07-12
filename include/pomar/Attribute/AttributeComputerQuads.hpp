#include <pomar/Core/PixelIndexer.hpp>
#include <pomar/Attribute/AttributeComputer.hpp>

#include <fstream>
#include <vector>

#ifndef ATTRIBUTE_COMPUTER_QUADS_HPP_INCLUDED
#define ATTRIBUTE_COMPUTER_QUADS_HPP_INCLUDED

namespace pomar
{
  enum QTreeType { MaxTree, MinTree };
  enum QConnectivity { Four, Eight };

  /* Atribute From Quads Computer */
  class AttributeFromQuadsComputer
  {
  public:
    void setUp(AttributeCollection &attrs, QConnectivity con);
    virtual void compute(size_t nodeId, AttributeCollection &attrs) = 0;
    virtual AttrType attrType() = 0;

  protected:
    int _p1, _p2, _pd, _p3, _p4;
    int _attrIdx; 
    QConnectivity _con;
  };

  /* AttributeFromQuadsComputer subclasses.*/
  class QArea: public AttributeFromQuadsComputer
  {
  public:
    inline AttrType attrType() { return AttrType::QUADS_AREA; }
    void compute(size_t nodeId, AttributeCollection &attrs);
  };

  class QCArea: public AttributeFromQuadsComputer
  {
  public:
    inline AttrType attrType() { return AttrType::QUADS_CONTINUOS_AREA; }
    void compute(size_t nodeId, AttributeCollection &attrs);
  };

  class QPerimeter: public AttributeFromQuadsComputer
  {
  public:
    inline AttrType attrType() { return AttrType::QUADS_PERIMETER; }
    void compute(size_t nodeId, AttributeCollection &attrs);
  };

  class QCPerimeter: public AttributeFromQuadsComputer
  {
  public:
    inline AttrType attrType() { return AttrType::QUADS_CONTINUOS_PERIMETER; }
    void compute(size_t nodeId, AttributeCollection &attrs);
  };

  class QEulerNumber: public AttributeFromQuadsComputer
  {
  public:
    inline AttrType attrType() { return AttrType::QUADS_EULER_NUMBER; }
    void compute(size_t nodeId, AttributeCollection &attrs);
  };

  template<class T>
  class AttributeComputerQuads
  {
  public:
    AttributeComputerQuads(QTreeType qTreeType, QConnectivity qConnectivity,
      const std::string &resource = "./resource/pomar/",
      const std::vector<std::shared_ptr<AttributeFromQuadsComputer>>& qattrComputers = {});
    void setUp(AttributeCollection &attrs, const CTree<T> &ct);
    void preProcess(AttributeCollection &attrs, const CTNode<T> &node);
    void merge(AttributeCollection &attrs, const CTNode<T> &node, const CTNode<T> &parent);
    void postProcess(AttributeCollection &attrs, const CTNode<T> &node);

    AttributeCollection compute(const CTree<T> &ct);
    std::unique_ptr<IncrementalAttributeComputer<T>> toIncrementalAttributeComputer();
  private:
    static const int P1; static const int P2; static const int P3;
    static const int P4; static const int PD;

    static const int P1T; static const int P2T;
    static const int P3T;  static const int PDT;

    static const int NUM_DT_LEAVES;

    bool isOutOfDomain(int index) const;
    bool isLower(int ip, int iq);
    bool isGreater(int ip, int iq);

    const std::vector<unsigned char>& counting(int ip);
    void readDT(const std::string &resource);
  private:
    int _p1, _p2, _p3, _pd, _p4;
    std::vector<std::shared_ptr<AttributeFromQuadsComputer>> _qattrComputers;
    std::vector<IPoint2D> _window;
    std::vector<std::vector<unsigned char>> _dt;
    std::vector<T> _f;
    std::unique_ptr<PixelIndexer> _pixelIndexer;
    QTreeType _qTreeType;
    QConnectivity _qconn;
  };

  /* ------------------------- [ ATTRIBUTE COMPUTER QUADS ] ------------------------------- */
  template<class T> const int AttributeComputerQuads<T>::P1 = 0;  
  template<class T> const int AttributeComputerQuads<T>::P2 = 1;  
  template<class T> const int AttributeComputerQuads<T>::P3 = 2;
  template<class T> const int AttributeComputerQuads<T>::P4 = 3;
  template<class T> const int AttributeComputerQuads<T>::PD = 4; 

  template<class T> const int AttributeComputerQuads<T>::P1T = 5;  
  template<class T> const int AttributeComputerQuads<T>::P2T = 6;
  template<class T> const int AttributeComputerQuads<T>::P3T = 7;
  template<class T> const int AttributeComputerQuads<T>::PDT = 8; 
  
  template<class T> const int AttributeComputerQuads<T>::NUM_DT_LEAVES = 6561;

  template<class T>
  AttributeComputerQuads<T>::AttributeComputerQuads(QTreeType qTreeType, 
    QConnectivity qConnectivity, const std::string &resource, 
    const std::vector<std::shared_ptr<AttributeFromQuadsComputer>>& qattrComputers)
  {
    _window.insert(_window.end(),{IPoint2D{-1,-1}, IPoint2D{0,-1}, IPoint2D{1,-1}, 
      IPoint2D{-1,0}, IPoint2D{1,0}, IPoint2D{-1,1}, IPoint2D{0,1}, IPoint2D{1,1}});
    
    _qattrComputers = qattrComputers;
    _qconn = qConnectivity;
    _qTreeType = qTreeType;
    if (_qTreeType == QTreeType::MaxTree) {
      if (qConnectivity == QConnectivity::Eight)
        readDT(resource + "/dt-max-tree-8c.dat");
      else
        readDT(resource + "/dt-max-tree-4c.dat");
    }
    else {
      if (qConnectivity == QConnectivity::Eight)
        readDT(resource + "/dt-min-tree-8c.dat");
      else
        readDT(resource + "/dt-min-tree-4c.dat");
    }
  }

  template<class T>
  void AttributeComputerQuads<T>::setUp(AttributeCollection &attrs, const CTree<T> &ct)
  {
    auto n = ct.numberOfNodes();
    auto meta = std::dynamic_pointer_cast<CTMetaImage2D>(ct.meta());
    _f = ct.convertToVector();
    attrs.push(AttrType::QUADS_Q1, n); attrs.push(AttrType::QUADS_Q2, n); attrs.push(AttrType::QUADS_QD, n);
    attrs.push(AttrType::QUADS_Q3, n); attrs.push(AttrType::QUADS_Q4, n);

    _p1 = attrs.attrIndex(AttrType::QUADS_Q1); _p2 = attrs.attrIndex(AttrType::QUADS_Q2);
    _pd = attrs.attrIndex(AttrType::QUADS_QD); _p3 = attrs.attrIndex(AttrType::QUADS_Q3);
    _p4 = attrs.attrIndex(AttrType::QUADS_Q4);

    for (auto q : _qattrComputers) { 
      attrs.push(q->attrType(), n);
      q->setUp(attrs, _qconn);
    }

    std::unique_ptr<PixelIndexer> tpixelIndexer{new PixelIndexerDefaultValue{meta->width(), meta->height()}};
    _pixelIndexer = std::move(tpixelIndexer);
  }

  template<class T>
  void AttributeComputerQuads<T>::preProcess(AttributeCollection &attrs, const CTNode<T> &node)
  {
    for (auto& elem : node.elementIndices()) {
      auto c = counting(elem); 
      attrs[_p1][node.id()] += c[P1] - c[P1T]; attrs[_p2][node.id()] += c[P2] - c[P2T]; 
      attrs[_p3][node.id()] += c[P3] - c[P3T]; attrs[_pd][node.id()] += c[PD] - c[PDT] ;
      attrs[_p4][node.id()] += c[P4]; 
    }
  }

  template<class T>
  void AttributeComputerQuads<T>::merge(AttributeCollection &attrs, const CTNode<T> &node, 
    const CTNode<T> &parent)
  {
    attrs[_p1][parent.id()] += attrs[_p1][node.id()]; attrs[_p2][parent.id()] += attrs[_p2][node.id()];
    attrs[_p3][parent.id()] += attrs[_p3][node.id()]; attrs[_p4][parent.id()] += attrs[_p4][node.id()];
    attrs[_pd][parent.id()] += attrs[_pd][node.id()];
  }

  template<class T>
  void AttributeComputerQuads<T>::postProcess(AttributeCollection &attrs, const CTNode<T> &node)
  {
    for (auto q: _qattrComputers)
      q->compute(node.id(), attrs);
  }

  template<class T>
  AttributeCollection AttributeComputerQuads<T>::compute(const CTree<T> &ct)
  {
    auto computer = toIncrementalAttributeComputer();
    AttributeCollection attrs;
    return computer->doCompute(attrs, ct);
  }

  template<class T>
  std::unique_ptr<IncrementalAttributeComputer<T>> 
  AttributeComputerQuads<T>::toIncrementalAttributeComputer()
  {
    auto mySetup = [this](AttributeCollection &attrs, const CTree<T> &ct) { this->setUp(attrs, ct); };
    auto myPreProcess = [this](AttributeCollection &attrs, const CTNode<T> &node) { 
      this->preProcess(attrs, node); 
    };
    auto myMerge = [this](AttributeCollection &attrs, const CTNode<T> &node, const CTNode<T> &parent) {
      this->merge(attrs, node, parent);
    };
    auto myPostProcess = [this](AttributeCollection &attrs, const CTNode<T> &node) {
      this->postProcess(attrs, node);
    };

    return std::unique_ptr<IncrementalAttributeComputer<T>>(
      new IncrementalAttributeComputer<T>(myPreProcess, myMerge, myPostProcess, mySetup));
  }

  template<class T>
  const std::vector<unsigned char>& AttributeComputerQuads<T>::counting(int ip)
  {
    auto p = _pixelIndexer->pixel(ip);
    std::string coding = {"11111111"};
    for (size_t i = 0; i < _window.size(); i++) {
      const auto& n = _window[i];
      auto iq = _pixelIndexer->index(p + n);
      if (isLower(ip, iq))
        coding[i] = '0';
      else if (isGreater(ip, iq))
        coding[i] = '2';      
    }
    
    auto idt = std::stoi(coding, 0, 3);
    return _dt[idt];
  }

  template<class T>
  bool AttributeComputerQuads<T>::isLower(int ip, int iq) 
  {
    if (isOutOfDomain(iq)) 
      return _qTreeType == QTreeType::MaxTree;
    return _f[iq] < _f[ip];
  }

  template<class T>
  bool AttributeComputerQuads<T>::isGreater(int ip, int iq)
  {
    if (isOutOfDomain(iq))
      return _qTreeType == QTreeType::MinTree;
    return _f[iq] > _f[ip];
  }

  template<class T>  
  bool AttributeComputerQuads<T>::isOutOfDomain(int index) const
  {
    return index < 0;
  }

  template<class T>
  void AttributeComputerQuads<T>::readDT(const std::string &resource)
  {
    std::ifstream in{resource, std::ios::binary};
    _dt.resize(NUM_DT_LEAVES);
    char buffer[9];

    for (auto i = 0; i < NUM_DT_LEAVES; i++) {
      in.read(buffer, 9);
      _dt[i].insert(_dt[i].end(), buffer, buffer + 9);
    }
  }

  /* --------------------- [ AttibuteFromQuadsComputers ] ---------------------------------- */
  void AttributeFromQuadsComputer::setUp(AttributeCollection &attrs, QConnectivity con)
  {
    _p1 = attrs.attrIndex(AttrType::QUADS_Q1);
    _p2 = attrs.attrIndex(AttrType::QUADS_Q2);
    _pd = attrs.attrIndex(AttrType::QUADS_QD); 
    _p3 = attrs.attrIndex(AttrType::QUADS_Q3); 
    _p4 = attrs.attrIndex(AttrType::QUADS_Q4);
    _con = con;
    _attrIdx = attrs.attrIndex(attrType());
  }

  /* ------------------- [ AttibuteFromQuadsComputers subclasses ] ------------------------------- */
  void QArea::compute(size_t nodeId, AttributeCollection &attrs)
  {
    auto area = (attrs[_p1][nodeId] + (2.0*attrs[_p2][nodeId]) + (2.0*attrs[_pd][nodeId]) + 
      (3.0*attrs[_p3][nodeId]) + (4.0*attrs[_p4][nodeId])) / 4.0;    
    attrs[_attrIdx][nodeId] = area;
  }

  void QCArea::compute(size_t nodeId, AttributeCollection &attrs)
  {
    auto area = 0.25*((attrs[_p1][nodeId]/2.0) + attrs[_p2][nodeId] + attrs[_pd][nodeId] +
      + ((7.0/2.0)*attrs[_p3][nodeId]) + (4.0*attrs[_p4][nodeId]));
    attrs[_attrIdx][nodeId] = area;
  }

  void QPerimeter::compute(size_t nodeId, AttributeCollection &attrs)
  {
    auto perimeter = attrs[_p1][nodeId] + attrs[_p2][nodeId] + (2.0*attrs[_pd][nodeId]) + 
      attrs[_p3][nodeId];
    attrs[_attrIdx][nodeId] = perimeter;
  }

  void QCPerimeter::compute(size_t nodeId, AttributeCollection &attrs)
  {
    auto perimeter = attrs[_p2][nodeId] + ((attrs[_p1][nodeId] + attrs[_p3][nodeId]) / 1.41);
    attrs[_attrIdx][nodeId] = perimeter;
  }

  void QEulerNumber::compute(size_t nodeId, AttributeCollection &attrs)
  {
    auto e = 0.0;
    if (_con == Four)
      e = (attrs[_p1][nodeId] - attrs[_p3][nodeId] + (2.0 * attrs[_pd][nodeId])) / 4.0;
    else
      e = (attrs[_p1][nodeId] - attrs[_p3][nodeId] - (2.0 * attrs[_pd][nodeId])) / 4.0;
    
    attrs[_attrIdx][nodeId] = e;
  }
}
#endif
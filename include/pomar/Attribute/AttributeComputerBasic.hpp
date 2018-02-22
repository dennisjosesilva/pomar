#include <pomar/Attribute/AttributeComputer.hpp>
#include <pomar/Attribute/AttributeCollection.hpp>
#include <pomar/ComponentTree/CTree.hpp>

#include <memory>
#include <functional>

#ifndef ATTRIBUTE_COMPUTER_BASIC_HPP_INCLUDED
#define ATTRIBUTE_COMPUTER_BASIC_HPP_INCLUDED

/** @file */

namespace pomar
{
  /** Class which computes area for component tree incrementally. */
  template<class T>
  class AreaAttributeComputer
  {
  public:
    /** 
     * Set up AttributeCollection 'attrs' to store area for each node of the 
     * component tree 'ct'. */
    void setUp(AttributeCollection &attrs, const CTree<T> &ct);
    /** 'PreProcess' function of the incremental algorithm to compute area.  */
    void preProcess(AttributeCollection& attrs, const CTNode<T> &node);
    /** 'merge' function of the incremental algorithm to compute area.  */
    void merge(AttributeCollection &attrs, const CTNode<T> &node, 
      const CTNode<T> &parent);
    /** 'postProcess' function of the incremental algorithm to compute area.  */
    void postProcess(AttributeCollection &attrs, const CTNode<T> &node);

    /** Compute area for each component tree 'ct' node. */
    AttributeCollection compute(const CTree<T> &ct);
    /** Convert this instance to an incremental attribute computer.*/
    std::unique_ptr<IncrementalAttributeComputer<T>> toIncrementalAttributeComputer();
  private:
    int _myIndex;
  };

  /* ===================== [IMPLEMENTATION ] ================================================= */
  /* ================ [ AREA ATTRIBUTE COMPUTER ] ============================================ */
  template<class T>
  void AreaAttributeComputer<T>::setUp(AttributeCollection &attrs, const CTree<T> &ct)
  { 
    attrs.push(AttrType::AREA, ct.numberOfNodes());
    _myIndex = attrs.attrIndex(AttrType::AREA);
  }

  template<class T>
  void AreaAttributeComputer<T>::preProcess(AttributeCollection& attrs, 
    const CTNode<T> &node)
  {
    attrs[_myIndex][node.id()] += node.elementIndices().size();
  }

  template<class T>
  void AreaAttributeComputer<T>::merge(AttributeCollection &attrs, const CTNode<T> &node, 
    const CTNode<T> &parent)
  {
    attrs[_myIndex][parent.id()] += attrs[_myIndex][node.id()];
  }

  template<class T>
  void AreaAttributeComputer<T>::postProcess(AttributeCollection &attrs, const CTNode<T> &node)
  {
    /*This method is kept empty. */
  }

  template<class T>
  AttributeCollection AreaAttributeComputer<T>::compute(const CTree<T> &ct)
  {
    auto computer = toIncrementalAttributeComputer();
    AttributeCollection attrs;
    return computer->doCompute(attrs, ct);
  }

  template<class T>
  std::unique_ptr<IncrementalAttributeComputer<T>> 
    AreaAttributeComputer<T>::toIncrementalAttributeComputer()
  {
    auto mySetup = [this](AttributeCollection &attrs, const CTree<T> &ct) { this->setUp(attrs, ct); };
    auto myPreProcess = [this](AttributeCollection& attrs, const CTNode<T> &node) {
      this->preProcess(attrs, node);
    };
    auto myMerge = [this](AttributeCollection &attrs, const CTNode<T> &node, 
      const CTNode<T> &parent) {
      this->merge(attrs, node, parent);
    };
    auto myPostProcess = [this](AttributeCollection &attrs, const CTNode<T> &node) {
      this->postProcess(attrs, node);
    };

    return std::unique_ptr<IncrementalAttributeComputer<T>>(
      new IncrementalAttributeComputer<T>(myPreProcess, myMerge, myPostProcess, mySetup));
  }
}
#endif  
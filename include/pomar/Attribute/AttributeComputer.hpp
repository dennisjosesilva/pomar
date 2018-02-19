#include <pomar/Core/CRTP.hpp>
#include <pomar/Attribute/AttributeCollection.hpp>
#include <pomar/ComponentTree/CTree.hpp>

#include <memory>
#include <functional>

#ifndef ATTRIBUTE_COMPUTER_HPP_INCLUDED
#define ATTRIBUTE_COMPUTER_HPP_INCLUDED

/** @file */

namespace pomar
{
  /** 
   * CRTP base class which provides an interface to algorithm which computes 
   * attributes. */
  template<class T>
  class AttributeComputer : public virtual CRTP<T, AttributeComputer>
  {
  public:
    /** 
     * Use the method 'doProcess' from the derived class to compute a collection of attributes 
     * for a component tree 'ct'.*/
    AttributeCollection compute(const CTree<T> &ct);    
  };

  /**
   * Class which encapsulates the general incremental algorithm to compute attributes. 
   * One should provide functions: 'preProcess' for a initial computation, 'merge' which 
   * merges the computation of a node with its parent, 'postProcess' for some attributes
   * which need computation after merging, and setup which initialises AttributeCollection 
   * for the computation of the attribute. */
  template<class T>
  class IncrementalAttributeComputer : 
    public virtual AttributeComputer<IncrementalAttributeComputer<T>>
  {
  public:
    IncrementalAttributeComputer() = delete;    

    /**
     * Constructor which receives the needed functions to implement incremental attribute computation.
     */
    IncrementalAttributeComputer(
      std::function<void(AttributeCollection&, const CTNode<T>&)> pPreProcess,
      std::function<void(AttributeCollection&, const CTNode<T>&, const CTNode<T>&)> pMerge,
      std::function<void(AttributeCollection&, const CTNode<T>&)> pPostProcess,
      std::function<void(AttributeCollection&, const CTree<T>&)> setUp = 
        [](AttributeCollection& attrs, const CTree<T>& ct) { });

    /** General incremental algorithm to compute attributes in an component tree 'ct'. */
    AttributeCollection doCompute(AttributeCollection &attrs, const CTree<T> &ct);    

    /** Calls function 'setup' set up in the constructor. */
    void setUp(AttributeCollection &attrs, const CTree<T> &ct);
    /** Calls function 'preProcess' set up in the constructor. */
    void preProcess(AttributeCollection &attrs, const CTNode<T> &node);
    /** Calls function 'merge' set up in the constructor. */
    void merge(AttributeCollection &attrs, const CTNode<T> &node, const CTNode<T> &parent);
    /** Calls function 'postProcess' set up in the constructor. */
    void postProcess(AttributeCollection &attrs, const CTNode<T> &node);

  private:       
    std::function<void(AttributeCollection&, const CTNode<T>&)> _preProcess;
    std::function<void(AttributeCollection&, const CTNode<T>&, const CTNode<T>&)> _merge;
    std::function<void(AttributeCollection&, const CTNode<T>&)> _postProcess;
    std::function<void(AttributeCollection&, const CTree<T>&)> _setUp;
  };

  /** This class is used to compute a collection of attributes which can be computed incrementally. */
  template<class T>
  class IncrementalAttributeComputerCollection
  {
  public:
    /** Blank attribute computer collection constructor. */ 
    IncrementalAttributeComputerCollection();
    /** 
     * Initialise an incremental attribute computer collection with a list of incremental attribute computer
     * 'comps'. */
    IncrementalAttributeComputerCollection(
      std::initializer_list<std::unique_ptr<IncrementalAttributeComputer<T>>> comps);

    /** Push an incremental attribute computer at the collection. */
    void push(std::unique_ptr<IncrementalAttributeComputer<T>> attrComputer);
    /** clear collection. */
    void clear();

    /** Call funtion 'setUp' for each incremental attribute computer. */
    void setUp(AttributeCollection &attrs, const CTree<T> &ct);
    /** Call function 'preProcess' for each incremental attribute computer. */
    void preProcess(AttributeCollection &attrs, const CTNode<T> &node);
    /** Call function 'merge' for each incremental attribute computer. */
    void merge(AttributeCollection &attrs, const CTNode<T> &node, const CTNode<T> &parent);
    /** Call function 'postProcess' for each incremental attribute computer. */
    void postProcess(AttributeCollection &attrs, const CTNode<T> &node);

    /** Compute incrementally all incremental attribute computers pushed in this instance's collection.*/
    AttributeCollection compute(const CTree<T> &ctree);
    /** Convert this instance to an Incremental Attribute Collection instance. */
    std::unique_ptr<IncrementalAttributeComputer<T>> toIncrementalAttributeComputer();
  private:
    std::vector<std::unique_ptr<IncrementalAttributeComputer<T>>> _attrComputers;
  };
    

  /* ============================= [IMPLEMENTATION ] ======================================== */

  /* ========================== [Attribute Computer ] ======================================= */
  template<class T>
  AttributeCollection AttributeComputer<T>::compute(const CTree<T> &ct)
  {
    AttributeCollection attrs;
    return this->underlying().doCompute(attrs, ct);
  }  

  /* ======================= [Incremental Attribute Computer ] ================================== */
  template<class T>
  IncrementalAttributeComputer<T>::IncrementalAttributeComputer(
      std::function<void(AttributeCollection&, const CTNode<T>&)> pPreProcess,
      std::function<void(AttributeCollection&, const CTNode<T>&, const CTNode<T>&)> pMerge,
      std::function<void(AttributeCollection&, const CTNode<T>&)> pPostProcess,
      std::function<void(AttributeCollection&, const CTree<T>&)> pSetUp)
      :_preProcess{pPreProcess}, _merge{pMerge}, _postProcess{pPostProcess}, _setUp{pSetUp}
  {}

  template<class T>
  void IncrementalAttributeComputer<T>::setUp(AttributeCollection &attrs, const CTree<T> &ct)
  {
    return this->_setUp(attrs, ct);
  }

  template<class T>
  void IncrementalAttributeComputer<T>::preProcess(AttributeCollection &attrs, const CTNode<T> &node)
  {
    this->_preProcess(attrs, node);
  }

  template<class T>
  void IncrementalAttributeComputer<T>::merge(AttributeCollection &attrs, const CTNode<T> &node,
   const CTNode<T> &parent)
  {
    this->_merge(attrs, node, parent);
  }

  template<class T>
  void IncrementalAttributeComputer<T>::postProcess(AttributeCollection &attrs, const CTNode<T> &node)
  {
    this->_postProcess(attrs, node);
  }

  template<class T>
  AttributeCollection IncrementalAttributeComputer<T>::doCompute(AttributeCollection &attrs, const CTree<T> &ct)
  {
    this->setUp(attrs, ct);
    ct.transverse([this, &ct, &attrs](const CTNode<T> &node) {
      this->preProcess(attrs, node);
      if (node.parent() != -1) {
        const auto& nparent = ct.node(node.parent());
        this->merge(attrs, node, nparent);
      }
      this->postProcess(attrs, node);
    });
    return attrs;
  }

  /* ====================== [ INCREMENTAL ATTRIBUTE COMPUTER ] ======================================  */
  template<class T>
  IncrementalAttributeComputerCollection<T>::IncrementalAttributeComputerCollection()
  {}

  template<class T>
  IncrementalAttributeComputerCollection<T>::IncrementalAttributeComputerCollection(
      std::initializer_list<std::unique_ptr<IncrementalAttributeComputer<T>>> comps)
  {}

  template<class T>
  void IncrementalAttributeComputerCollection<T>::push(
    std::unique_ptr<IncrementalAttributeComputer<T>> attrComputer)
  {
    _attrComputers.push_back(std::move(attrComputer));
  }

  template<class T>
  void IncrementalAttributeComputerCollection<T>::clear()
  {
    _attrComputers.clear();
  }

  template<class T>
  void IncrementalAttributeComputerCollection<T>::setUp(AttributeCollection& attrs, const CTree<T> &ct)
  {
    for (auto &c : _attrComputers)
      c->setUp(attrs, ct);
  }

  template<class T>
  void IncrementalAttributeComputerCollection<T>::preProcess(AttributeCollection &attrs, 
    const CTNode<T> &node)
  {
    for (auto &c : _attrComputers)
      c->preProcess(attrs, node);
  }

  template<class T>
  void IncrementalAttributeComputerCollection<T>::merge(AttributeCollection &attrs, 
    const CTNode<T> &node, const CTNode<T> &parent)
  {
    for (auto &c: _attrComputers)
      c->merge(attrs, node, parent);
  }

  template<class T>
  void IncrementalAttributeComputerCollection<T>::postProcess(AttributeCollection &attrs,
    const CTNode<T> &node)
  {
    for (auto &c : _attrComputers)
      c->postProcess(attrs, node);
  }

  template<class T>
  AttributeCollection 
  IncrementalAttributeComputerCollection<T>::compute(const CTree<T> &ctree)
  {
    AttributeCollection attrs;
    auto computer = toIncrementalAttributeComputer();
    return computer->doCompute(attrs, ctree);
  }

  template<class T>
  std::unique_ptr<IncrementalAttributeComputer<T>> 
  IncrementalAttributeComputerCollection<T>::toIncrementalAttributeComputer()
  {
    auto mySetup = [this] (AttributeCollection &attrs, const CTree<T> &ct) {  this->setUp(attrs, ct); };
    auto myPreProcess = [this](AttributeCollection &attrs, const CTNode<T> &node) {
      this->preProcess(attrs, node);
    };
    auto myMerge = [this](AttributeCollection &attrs, const CTNode<T> &node, 
      const CTNode<T>& parent) {
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
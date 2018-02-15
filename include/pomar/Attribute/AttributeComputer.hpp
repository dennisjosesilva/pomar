#include <pomar/Core/CRTP.hpp>
#include <pomar/Attribute/AttributeCollection.hpp>
#include <pomar/ComponentTree/CTree.hpp>

#include <memory>
#include <functional>

#ifndef ATTRIBUTE_COMPUTER_HPP_INCLUDED
#define ATTRIBUTE_COMPUTER_HPP_INCLUDED

namespace pomar
{
  template<class T>
  class AttributeComputer : public virtual CRTP<T, AttributeComputer>
  {
  public:
    AttributeCollection compute(const CTree<T> &ct);    
  };

  template<class T>
  class IncrementalAttributeComputer : 
    public virtual AttributeComputer<IncrementalAttributeComputer<T>>
  {
  public:
    IncrementalAttributeComputer() = delete;    
    IncrementalAttributeComputer(
      std::function<void(AttributeCollection&, const CTNode<T>&)> pPreProcess,
      std::function<void(AttributeCollection&, const CTNode<T>&, const CTNode<T>&)> pMerge,
      std::function<void(AttributeCollection&, const CTNode<T>&)> pPostProcess,
      std::function<AttributeCollection(const CTree<T>&)> setUp = 
        [](const CTree<T>& ct) { return AttributeCollection(); });

    AttributeCollection doCompute(const CTree<T> &ct);    

    AttributeCollection setUp(const CTree<T> &ct);
    void preProcess(AttributeCollection &attrs, const CTNode<T> &node);
    void merge(AttributeCollection &attrs, const CTNode<T> &node, const CTNode<T> &parent);
    void postProcess(AttributeCollection &attrs, const CTNode<T> &node);

  private:   
    std::function<AttributeCollection(const CTree<T>&)> _setUp;
    std::function<void(AttributeCollection&, const CTNode<T>&)> _preProcess;
    std::function<void(AttributeCollection&, const CTNode<T>&, const CTNode<T>&)> _merge;
    std::function<void(AttributeCollection&, const CTNode<T>&)> _postProcess;
  };

  template<class T>
  class IncrementalAttributeComputerCollection
  {
  public:
    IncrementalAttributeComputerCollection();
    IncrementalAttributeComputerCollection(
      std::initializer_list<std::unique_ptr<IncrementalAttributeComputer<T>>> comps);

    void push(std::unique_ptr<IncrementalAttributeComputer<T>> attrComputer);
    void clear();

    void setUp(const CTree<T> &ct);
    void preProcess(AttributeCollection &attrs, const CTNode<T> &node);
    void merge(AttributeCollection &attrs, const CTNode<T> &node, const CTNode<T> &parent);
    void postProcess(AttributeCollection &attrs, const CTNode<T> &node);

    AttributeCollection compute(const CTree<T> &ctree);
    std::unique_ptr<IncrementalAttributeComputer<T>> toIncrementalAttributeComputer();
  private:
    std::vector<std::unique_ptr<IncrementalAttributeComputer<T>>> _attrComputers;
  };
    

  /* ============================= [IMPLEMENTATION ] ======================================== */

  /* ========================== [Attribute Computer ] ======================================= */
  template<class T>
  AttributeCollection AttributeComputer<T>::compute(const CTree<T> &ct)
  {
    return this->underlying().doCompute(ct);
  }  

  /* ======================= [Incremental Attribute Computer ] ================================== */
  template<class T>
  IncrementalAttributeComputer<T>::IncrementalAttributeComputer(
      std::function<void(AttributeCollection&, const CTNode<T>&)> pPreProcess,
      std::function<void(AttributeCollection&, const CTNode<T>&, const CTNode<T>&)> pMerge,
      std::function<void(AttributeCollection&, const CTNode<T>&)> pPostProcess,
      std::function<AttributeCollection(const CTree<T>&)> pSetUp)
      :_preProcess{pPreProcess}, _merge{pMerge}, _postProcess{pPostProcess}, _setUp{pSetUp}
  {}

  template<class T>
  AttributeCollection IncrementalAttributeComputer<T>::setUp(const CTree<T> &ct)
  {
    return this->_setUp(ct);
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
  AttributeCollection IncrementalAttributeComputer<T>::doCompute(const CTree<T> &ct)
  {
    auto attrCollection = this->setUp(ct);
    ct.transverse([this, &ct, &attrCollection](const CTNode<T> &node) {
      this->preProcess(attrCollection, node);
      if (node.parent() != -1) {
        const auto& nparent = ct.node(node.parent());
        this->merge(attrCollection, node, nparent);
      }
      this->postProcess(attrCollection, node);
    });
    return attrCollection;
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
  void IncrementalAttributeComputerCollection<T>::setUp(const CTree<T> &ct)
  {
    for (auto c : _attrComputers)
      c->setUp(ct);
  }

  template<class T>
  void IncrementalAttributeComputerCollection<T>::preProcess(AttributeCollection &attrs, 
    const CTNode<T> &node)
  {
    for (auto c : _attrComputers)
      c->preProcess(attrs, node);
  }

  template<class T>
  void IncrementalAttributeComputerCollection<T>::merge(AttributeCollection &attrs, 
    const CTNode<T> &node, const CTNode<T> &parent)
  {
    for (auto c: _attrComputers)
      c->merge(attrs, node, parent);
  }

  template<class T>
  void IncrementalAttributeComputerCollection<T>::postProcess(AttributeCollection &attrs,
    const CTNode<T> &node)
  {
    for (auto c : _attrComputers)
      c->postProcess(attrs, node);
  }

  template<class T>
  AttributeCollection 
  IncrementalAttributeComputerCollection<T>::compute(const CTree<T> &ctree)
  {
    auto computer = toIncrementalAttributeComputer(ctree);
    return computer->doCompute(ctree);
  }

  template<class T>
  std::unique_ptr<IncrementalAttributeComputer<T>> 
  IncrementalAttributeComputerCollection<T>::toIncrementalAttributeComputer()
  {
    auto mySetup = [this] (const CTree<T> &ct) { return this->setUp(ct); };
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
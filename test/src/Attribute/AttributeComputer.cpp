#include "../../catch.hpp"
#include <pomar/Attribute/AttributeComputer.hpp>
#include <pomar/Attribute/AttributeComputerBasic.hpp>
#include <numeric>
#include <algorithm>

#include <iostream>

using namespace pomar;

template<class T>
class IncrementalAttributeComputerTest
{
public:
  void setUp(AttributeCollection &attrs, const CTree<T> &ct);
  void preProcess(AttributeCollection &attrs, const CTNode<T> &node);
  void merge(AttributeCollection &attrs, const CTNode<T> &node, const CTNode<T> &parent);
  void postProcess(AttributeCollection &attrs, const CTNode<T> &node);

  std::unique_ptr<IncrementalAttributeComputer<T>> toIncrementalAttributeComputer();
private:
  int _index;
};

template<class T>
void IncrementalAttributeComputerTest<T>::setUp(AttributeCollection &attrs, const CTree<T> &ct) {
  attrs.push(AttrType::TEST_ATTRIBUTE, ct.numberOfNodes());
  _index = attrs.attrIndex(AttrType::TEST_ATTRIBUTE);
}

template<class T>
void IncrementalAttributeComputerTest<T>::preProcess(AttributeCollection &attrs, 
  const CTNode<T> &node) 
{
  attrs[_index][node.id()] += 1;
}

template<class T>
void IncrementalAttributeComputerTest<T>::merge(AttributeCollection &attrs, const CTNode<T> &node,
  const CTNode<T> &parent)
{  
  attrs[_index][parent.id()] += attrs[_index][node.id()];
}

template<class T>
void IncrementalAttributeComputerTest<T>::postProcess(AttributeCollection &attrs, 
  const CTNode<T> &node)
{ }

template<class T>
std::unique_ptr<IncrementalAttributeComputer<T>> 
IncrementalAttributeComputerTest<T>::toIncrementalAttributeComputer()
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

SCENARIO("IncrementalAttributeComputerCollection should compute attributes in order.") {
  GIVEN("A component tree 'ct' area and test attribute computers.") {
    std::vector<unsigned char> elements = { 2,0,3,2,1,3,7,0,3 };
    std::vector<int> parent = { 4,1,4,0,1,2,0,1,2 };
    std::vector<int> sortedIndices(elements.size());
    std::iota(sortedIndices.begin(), sortedIndices.end(), 0);
    std::sort(sortedIndices.begin(), sortedIndices.end(), [&elements](int i1, int i2) { 
      return elements[i1] < elements[i2]; });

    CTree<unsigned char> ct(parent, sortedIndices, elements);
    IncrementalAttributeComputerTest<unsigned char> attrTest;
    AreaAttributeComputer<unsigned char> attrArea;
    IncrementalAttributeComputerCollection<unsigned char> attrComputerCollection;
    WHEN("Attribute computer collection has pushed and compute attrTest and attrArea in order.") {
      attrComputerCollection.push(attrTest.toIncrementalAttributeComputer());
      attrComputerCollection.push(attrArea.toIncrementalAttributeComputer());
      auto attrs = attrComputerCollection.compute(ct);
      THEN("attrs should contains area and test attribute computed in order.") {
        REQUIRE(attrs.attrIndex(AttrType::TEST_ATTRIBUTE) == 0);
        REQUIRE(attrs.attrIndex(AttrType::AREA) == 1);
      }
      THEN("attrs should contains test attribute computed at first position") {      
        REQUIRE(attrs[0] == std::vector<double>{5.0, 4.0, 2.0, 1.0, 1.0});
      }
      THEN("attrs should contains area attribute computed at second position") {      
        REQUIRE(attrs[1] == std::vector<double>{9.0, 7.0, 3.0, 3.0, 1.0});
      }
    }    
  }
}
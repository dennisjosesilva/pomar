#include <pomar/Attribute/AttributeCollection.hpp>

namespace pomar
{
  AttributeCollection::AttributeCollection()
    :_nextIndex{0}
  {}

  std::vector<double>& AttributeCollection::operator[](int attrIndex)
  {
    return _values[attrIndex];
  }

  double AttributeCollection::get(int attrIndex, int nodeId)
  {
    return _values[attrIndex][nodeId];
  }

  void AttributeCollection::push(AttrType type, int nNodes)
  {
    _attrIndex[type] = _nextIndex;
    _values.push_back(std::vector<double>(nNodes, 0.0));
    _nextIndex++;
  }

  void AttributeCollection::set(int attrIndex, int nodeId, double value)
  {
    _values[attrIndex][nodeId] = value;
  }

  void AttributeCollection::clear()
  {
    _attrIndex.clear();
    _values.clear();
  }
}
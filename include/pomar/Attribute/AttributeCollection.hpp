#include <vector>
#include <map>

#ifndef ATTRIBUTE_COLLECTION_HPP_INCLUDED
#define ATTRIBUTE_COLLECTION_HPP_INCLUDED

namespace pomar 
{
  enum class AttrType 
  {
    AREA,
    PERIMETER,
    QUADS_Q1, QUADS_Q2, QUADS_Q3, QUADS_QD, QUADS_Q4, QUADS_Q1DASH,
    QUADS_AREA, QUADS_PERIMETER, QUADS_EULER_NUMBER,
    QUADS_CONTINUOS_AREA, QUADS_CONTINUOS_PERIMETER
  };

  class AttributeCollection 
  { 
  public:
    AttributeCollection();

    /* ==================== INLINE METHOD ================================================ */    
    int attrIndex(AttrType type) { return _attrIndex[type]; }

    /* ==================== METHODS ====================================================== */
    std::vector<double>& operator[](int attrIndex);
    double get(int attrIndex, int nodeId);
    void push(AttrType type, int nNodes);
    void set(int attrIndex, int nodeId, double value);        
    void clear();
    
  private:   
    int _nextIndex;
    std::map<AttrType, int> _attrIndex; 
    std::vector<std::vector<double>> _values;
  };
}
#endif
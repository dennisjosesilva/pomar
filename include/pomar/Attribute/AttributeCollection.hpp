#include <vector>
#include <map>

#ifndef ATTRIBUTE_COLLECTION_HPP_INCLUDED
#define ATTRIBUTE_COLLECTION_HPP_INCLUDED

/** @file */

namespace pomar 
{
  /** Enum used to denote the different attributes. */
  enum class AttrType 
  {
    TEST_ATTRIBUTE,
    AREA, PERIMETER,
    QUADS_Q1, QUADS_Q2, QUADS_Q3, QUADS_QD, QUADS_Q4, QUADS_Q1DASH,
    QUADS_AREA, QUADS_PERIMETER, QUADS_EULER_NUMBER,
    QUADS_CONTINUOUS_AREA, QUADS_CONTINUOUS_PERIMETER,
  };

  /** Class which represent a collection of attributes computed for a component tree. The attributes
   *  are stored in an array of arrays, such that, the first pushed attribute is stored at position 0,
   *  the second pushed attribute is stored at position 1 and so on. One may get the attribute index
   *  by calling the method attrIndex.
   */
  class AttributeCollection 
  { 
  public:
    AttributeCollection(); /**< Default constructor. */

    /* ==================== INLINE METHOD ================================================ */    
    /** 
     * Get the index of the attribute 'type' within the collection of attributes of the 
     * this instance. */
    int attrIndex(AttrType type) { return _attrIndex[type]; }

    /* ==================== METHODS ====================================================== */
    /** Get the attribute values stored in the index 'attrIndex'. */
    std::vector<double>& operator[](int attrIndex);
    
    /** 
     * Get the value of the attribute stored at index 'attrIndex' from the node with id 
     * 'nodeId'. */
    double get(int attrIndex, int nodeId);

    /** 
     * Reserve space to store values of attribute for the attribute identified by 'type' for a 
     * component tree with 'nNodes' nodes. */
    void push(AttrType type, int nNodes);

    /** Set the 'value' for the node with id 'nodeId' of the attribute stored in the 'attrIndex'. */
    void set(int attrIndex, int nodeId, double value);        

    /** Clear attributes of the collection. */
    void clear();
    
  private:   
    int _nextIndex;
    std::map<AttrType, int> _attrIndex; 
    std::vector<std::vector<double>> _values;
  };
}
#endif
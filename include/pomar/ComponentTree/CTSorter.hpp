#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <type_traits>

#include <pomar/Core/Sort.hpp>

#ifndef CTSORTER_HPP_INCLUDED
#define CTSORTER_HPP_INCLUDED

/** @file */

namespace pomar
{
  /** 
   * Function used to sort the pixels of an image in the component tree building algorithm, 
   * such that, the resulting tree is the max-tree. */
  template<typename T>
  std::vector<int> maxTreeSort(const std::vector<T> &elements);

  /**
   * Function used to sort the pixels of an image in the component tree building algorithm,
   * such that, the resulting tree is the min-tree. */
  template<typename T>
  std::vector<int> minTreeSort(const std::vector<T> &elements);


  /* ====================[ IMPLEMENTATION ]============================================= */
  template<typename T>
  std::vector<int> maxTreeSort(const std::vector<T> &elements)
  {    
    if (isLowSizeType<T>())      
      return incresingCountingSortIndex(elements);
    return STLsortIndex<T>(elements, [](const T& v1, const T& v2) { return v1 < v2; });           
  }


  template<typename T>
  std::vector<int> minTreeSort(const std::vector<T> &elements)
  {
    if (isLowSizeType<T>())
      return decreasingCountingSortIndex(elements);
    return STLsortIndex<T>(elements, [](const T& v1, const T& v2){ return v1 > v2; });
  }
}
#endif
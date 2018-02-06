#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <type_traits>
#include <pomar/Core/CRTP.hpp>

#ifndef CTSORTER_HPP_INCLUDED
#define CTSORTER_HPP_INCLUDED

namespace pomar
{
  class SelectSorterAlgorithm
  {
  private:
    template<typename T>
    std::vector<int> sort(const std::vector<T> &elements) const;    
  };  

  class MaxTreeSorter: private SelectSorterAlgorithm
  {
  public:
    template<typename T>
    std::vector<int> operator(const std::vector<T> &elements) const;

  private:
    template<typename T>
    std::vector<int> STLsort(const std::vector<T> &elements) const;
  };

  /* ==================[ IMPLEMENTATION ]=========================================== */
  template<typename T>
  std::vector<int> sort(const std::vector<T> &elements)
  {
    if (std::is_same<T, int>::value || std::is_same<T, unsigned int>::value 
      || std::is_same<T, char>::value || std::is_same<T, unsigned char>::value 
      || std::is_same<T, unsigned short>::value || std::is_same<T, short>::value 
      || std::is_same<T, bool>::value)
      return this->derived().fastSort(elements);
    else
      return this->derived().STLsort(elements);
  }

  template<typename T>
  std::vector<int> STLsort(const std::vector<T> &elements, 
    std::function<bool(const T&, const T&)> cmp)
  {
    std::vector<T> idx(elements.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(), [&elemens, cmp](int i1, int i2) { 
      return cmp(elements[i1], elements[i2]);
    });
    return idx;
  }
}
#endif
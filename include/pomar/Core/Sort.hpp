#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <type_traits>

#ifndef SORT_HPP_INCLUDED
#define SORT_HPP_INCLUDED

namespace pomar
{
    /* =================== [INTERFACE ] ======================================== */
    template<typename T>
    bool isLowSizeType();

    template<typename T>
    std::vector<int> STLsortIndex(const std::vector<T> &v, 
      std::function<bool(const T&, const T&)> cmp);

    template<typename T>
    std::vector<int> incresingCountingSortIndex(const std::vector<T> &v);

    template<typename T>
    std::vector<int> decreasingCountingSortIndex(const std::vector<T> &v);                     

    /* =================== [ IMPLEMENTATION ] ===================================== */
    template<typename T>
    bool isLowSizeType()         
    {
      return std::is_same<T, int>::value || std::is_same<T, unsigned int>::value 
        || std::is_same<T, char>::value || std::is_same<T, unsigned char>::value 
        || std::is_same<T, unsigned short>::value || std::is_same<T, short>::value 
        || std::is_same<T, bool>::value;
    }

    template<typename T>
    std::vector<int> STLsortIndex(const std::vector<T> &v, 
      std::function<bool(const T&, const T&)> cmp)
    {
      std::vector<int> idx(v.size());
      std::iota(idx.begin(), idx.end(), 0);
      std::sort(idx.begin(), idx.end(), [&v, cmp](int i1, int i2) { 
        return cmp(v[i1], v[i2]);
      });
      return idx;
    }

    template<typename T>
    std::vector<int> incresingCountingSortIndex(const std::vector<T> &v)
    {
      T maxValue = std::numeric_limits<T>::max();
      std::vector<int> counter(maxValue+1, 0);
      std::vector<int> idx(v.size());      

      for (size_t i = 0; i < v.size(); i++)
        counter[v[i]]++;      

      for (T i = 1; i < maxValue; i++)
        counter[i] += counter[i-1];
      counter[maxValue] += counter[maxValue-1];
           
      for (int i = v.size()-1; i >= 0; --i)
        idx[--counter[v[i]]] = i;

      return idx;
    }

    template<typename T>
    std::vector<int> decreasingCountingSortIndex(const std::vector<T> &v)
    {
      T maxValue = std::numeric_limits<T>::max();
      std::vector<int> counter(maxValue + 1, 0);
      std::vector<int> idx(v.size());

      for (size_t i = 0; i < v.size(); i++)
        counter[maxValue - v[i]]++;

      for (T i = 1; i < maxValue; i++)
        counter[i] += counter[i - 1];
      counter[maxValue] += counter[maxValue - 1];

      for (int i = v.size() - 1; i >= 0; --i)
        idx[--counter[maxValue - v[i]]] = i;

      return idx;
    }
}

#endif
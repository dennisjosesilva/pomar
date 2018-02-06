#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <type_traits>

namespace pomar
{
    /* =================== [INTERFACE ] ======================================== */
    template<typename T>
    bool isLowSizeType();

    template<typename T>
    std::vector<int> STLsortIndex(std::vector<T> v, 
      std::function<bool(const T&, const T&));

    template<typename T>
    std::vector<int> incresingCountingSortIndex(const std::vector<T> &v);

    template<typename T>
    std::vector<int> descreasingCountingSortIndex(const std::vector<T> &v);


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
      std::vector<T> idx(elements.size());
      std::iota(idx.begin(), idx.end(), 0);
      std::sort(idx.begin(), idx.end(), [&elemens, cmp](int i1, int i2) { 
        return cmp(elements[i1], elements[i2]);
      });
      return idx;
    }

    template<typename T>
    std::vector<int> incresingCountingSortIndex(const std::vector<T> &v)
    {
      
    }
}


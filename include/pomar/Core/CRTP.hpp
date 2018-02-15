#ifndef CRTP_HPP_INCLUDED
#define CRTP_HPP_INCLUDED

namespace pomar
{
  template <typename T, template<typename> class crtpType>
  class CRTP
  {
    protected:
      T& underlying() { return static_cast<T&>(*this); }
      const T& underlying() const { return static_cast<const T&>(*this); }
      CRTP() {}
    
     private:
      friend crtpType<T>;
  };
}

#endif
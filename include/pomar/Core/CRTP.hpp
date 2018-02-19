#ifndef CRTP_HPP_INCLUDED
#define CRTP_HPP_INCLUDED

/** @file */

namespace pomar
{
  /** 
   * This class provide basic functionalities for classes which should be used 
   * as a CRTP base class. This class is based on 
   * https://www.fluentcpp.com/2017/05/19/crtp-helper/ 
   */

  template <typename T, template<typename> class crtpType>
  class CRTP
  {
    protected:
      /** Return the derived class object in a CRTP hierarchy as a reference. */
      T& underlying() { return static_cast<T&>(*this); }

      /** Return the derived class object in a CRTP hierarchy as a const reference. */
      const T& underlying() const { return static_cast<const T&>(*this); }
      CRTP() {}
    
     private:
      friend crtpType<T>;
  };
}

#endif
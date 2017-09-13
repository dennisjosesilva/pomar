#ifndef POMAR_POINT_H_INCLUDED
#define POMAR_POINT_H_INCLUDED

namespace pomar
{
  template<class T>
  class Point2D
  {
  public:
    Point2D(T px, T py);

    inline T x() const { return _x; }
    inline void x(int px) { _x = px; }

    inline T y() const { return _y; }
    inline void y(int py) { _y = py; }


    Point2D<T> sum(const Point2D<T>& q) const;
    Point2D<T> sub(const Point2D<T>& q) const;
    bool isEqual(const Point2D<T> &q) const;
    
    template<typename V> Point2D<T> mult(const V& value) const;
    template<typename V> Point2D<T> div(const V& value) const;

    Point2D<T> operator+(const Point2D<T> &q) const;
    void operator+= (const Point2D<T> &q);
    
    Point2D operator-(const Point2D<T> &q) const;
    void operator-=(const Point2D<T> &q);

    bool operator==(const Point2D<T>& q) const;
    
    template<typename V> Point2D<T> operator*(const V& value) const;
    template<typename V> void operator*=(const V& value);
    template<typename V> Point2D<T> operator/(const V& value) const;
    template<typename V> void operator/=(const V& value);
    
  private:
    T _x;
    T _y;
  };

  using ULPoint2D = Point2D<unsigned long>;
  using LPoint2D = Point2D<long>;
  using IPoint2D = Point2D<int>;
  using UIPoint2D = Point2D<unsigned int>;
  using CPoint2D = Point2D<char>;
  using UCPoint2D = Point2D<unsigned char>;
  using FPoint2D = Point2D<float>;
  using DPoint2D = Point2D<double>;

  template<class T>
  template<typename V>
  Point2D<T> Point2D<T>::mult(const V& value) const
  {
    return Point2D<T>(_x * value, _y * value);
  }

  template<class T>
  template<typename V>
  Point2D<T> Point2D<T>::operator*(const V& value) const
  {
    return mult(value);
  }

  template<class T>
  template<typename V>
  void Point2D<T>::operator*=(const V& value)
  {
    _x *= value;
    _y *= value;
  }

  template<class T>
  template<typename V>
  Point2D<T> Point2D<T>::div(const V& value) const
  {
    return Point2D<T>(_x / value, _y / value);
  }

  template<class T>
  template<typename V>
  Point2D<T> Point2D<T>::operator/(const V& value) const
  {
    return div(value);
  }

  template<class T>
  template<typename V>
  void Point2D<T>::operator/=(const V& value)
  {
    _x /= value;
    _y /= value;
  }
  
  template<class T>
  Point2D<T>::Point2D(T px, T py)
    :_x(px), _y(py)
  {}

  template<class T>
  Point2D<T> Point2D<T>::sum(const Point2D<T>& q) const
  {
    return Point2D<T>(_x + q._x, _y + q._y);
  }

  template<class T>
  Point2D<T> Point2D<T>::sub(const Point2D<T>& q) const
  {
    return Point2D<T>(_x - q._x, _y - q._y);
  }

  template<class T>
  bool Point2D<T>::isEqual(const Point2D<T> &q) const
  {
    return _x == q._x && _y == q._y;
  }
  
  template<class T>
  Point2D<T> Point2D<T>::operator+(const Point2D<T> &q) const
  {
    return sum(q);
  }

  template<class T>
  void Point2D<T>::operator+= (const Point2D<T> &q)
  {
    _x += q._x;
    _y += q._y;
  }

  template<class T>
  Point2D<T> Point2D<T>::operator-(const Point2D<T> &q) const
  {
    return sub(q);
  }

  template<class T>
  void Point2D<T>::operator-=(const Point2D &q)
  {
    _x -= q._x;
    _y -= q._y;
  }

  template<class T>
  bool Point2D<T>::operator==(const Point2D<T>& q) const
  {
    return isEqual(q);
  }
}

#endif

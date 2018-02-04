#ifndef POMAR_POINT_H_INCLUDED
#define POMAR_POINT_H_INCLUDED

namespace pomar
{

  /**
  * Class to represent 2D points of any type.
  */
  template<class T>
  class Point2D
  {
  public:
    /** Construct a point from 2 values of a type T. */
    Point2D(T px, T py);

    /** Get the value of the coordinate x. */
    inline T x() const { return _x; }

    /** Set a value of the coordinate x. */
    inline void x(int px) { _x = px; }

    /** Get the value of the coordinate y */
    inline T y() const { return _y; }

    /** Get the value of the coordinate y. */
    inline void y(int py) { _y = py; }

    /** Sum this point with a point q  using + operator of the type T */
    Point2D<T> sum(const Point2D<T>& q) const;

    /** Subtract point q from this point using - operator of the type T */
    Point2D<T> sub(const Point2D<T>& q) const;

    /** Check whether this point is equal to a point q or not. */
    bool isEqual(const Point2D<T> &q) const;

    /** Multiplicate this point by a value of type V. */
    template<typename V> Point2D<T> mult(const V& value) const;

    /** Divide this point by a value of type V. */
    template<typename V> Point2D<T> div(const V& value) const;

    /** Shortcut for sum(). */
    Point2D<T> operator+(const Point2D<T> &q) const;
    /** Update this point by summing it with q. */
    void operator+= (const Point2D<T> &q);

    /** Shortcut for sub().*/
    Point2D operator-(const Point2D<T> &q) const;
    /** Update this point by subtracting q from it */
    void operator-=(const Point2D<T> &q);

    /** Shortcut for isEqual() */
    bool operator==(const Point2D<T>& q) const;

    /** Shortcut for mult() */
    template<typename V> Point2D<T> operator*(const V& value) const;
    /** Update this point by multiplying it by value. */
    template<typename V> void operator*=(const V& value);
    /** Shortcut by div() */
    template<typename V> Point2D<T> operator/(const V& value) const;
    /** Update this point by dividing it by value. */
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

#ifndef POMAR_POINT_H_INCLUDED
#define POMAR_POINT_H_INCLUDED

namespace pomar
{
  class Point2D
  {
  public:
    Point2D(int px, int py);

    inline int x() const { return _x; }
    inline void x(int px) { _x = px; }

    inline int y() const { return _y; }
    inline void y(int py) { _y = py; }


    Point2D sum(const Point2D& q) const;
    Point2D sub(const Point2D& q) const;
    template<typename T> Point2D mult(const T& value) const;

    Point2D operator+(const Point2D &q) const;
    void operator+= (const Point2D &q);
    
    Point2D operator-(const Point2D &q) const;
    void operator-=(const Point2D &q);
    
    template<typename T> Point2D operator*(const T& value) const;
    template<typename T> void operator*=(const T& value);
    
  private:
    int _x;
    int _y;
  };


  template<typename T>
  Point2D Point2D::mult(const T& value) const
  {
    return Point(_x * value, _y * value);
  }

  template<typename T>
  Point2D Point2D::operator*(const T& value) const
  {
    return mult(value);
  }

  template<typename T>
  void Point2D::operator*=(const T& value)
  {
    _x *= value;
    _y *= value;
  }
  
}

#endif

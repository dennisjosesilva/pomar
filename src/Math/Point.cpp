#include <pomar/Math/Point.hpp>

namespace pomar
{

  Point2D::Point2D(int px=0, int py=0)
    :_x(px), _y(py)
  {}
  
  Point2D Point2D::sum(const Point2D& q) const
  {
    return Point2D(_x + q._x, _y + q._y);
  }
  
  Point2D Point2D::sub(const Point2D& q) const
  {
    return Point2D(_x - q._x, _y - q._y);
  }

  Point2D Point2D::operator+(const Point2D &q) const
  {
    return sum(q);
  }
  
  void Point2D::operator+= (const Point2D &q)
  {
    _x += q._x;
    _y += q._y;
  }
    
  Point2D Point2D::operator-(const Point2D &q) const
  {
    return sub(q);
  }
  
  void Point2D::operator-=(const Point2D &q)
  {
    _x -= q._x;
    _y -= q._y;
  }
}

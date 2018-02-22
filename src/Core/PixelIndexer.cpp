#include <pomar/Core/PixelIndexer.hpp>
#include <sstream>

namespace pomar
{
  /*------------------- [ PIXEL INDEXER ] ----------------------------------------- */
  PixelIndexer::PixelIndexer(int pwidth, int pheight)
    : _width{pwidth}, _height{pheight}
  {}

  IPoint2D PixelIndexer::pixel(int p) const
  {
    auto i = index(p);
    return IPoint2D(i % _width, i / _height);
  }

  /*------------------------ [ PIXEL INDEXER NEAREST BORDER ] ---------------------- */
  PixelIndexerNearestBorder::PixelIndexerNearestBorder(int pwidth, int pheight)
    :PixelIndexer(pwidth, pheight)
  {}

  int PixelIndexerNearestBorder::index(int px, int py) const
  {
    if (px < 0)  px = 0;
    else if (px >= _width)  px = _width - 1;
    
    if (py < 0) py = 0;
    else if (py >= _height) py = _height - 1;

    return (_width * py) + px;
  }

  /*------------------------- [ PIXEL INDEXER DEFAULT VALUE ] -------------------------- */
  PixelIndexerDefaultValue::PixelIndexerDefaultValue(int pwidth, int pheight, 
    int pdefaultValue): PixelIndexer{pwidth, pheight}, _defaultValue{pdefaultValue}
  {}

  int PixelIndexerDefaultValue::index(int px, int py) const
  {
    if (px < 0 || px >= _width || py < 0 || py >= _height)
      return _defaultValue;
    else
      return (_width * py) + px;
  }

  IPoint2D PixelIndexerDefaultValue::pixel(int p) const {
    auto r = index(p%_width, p/_height);
    if (r == _defaultValue) return IPoint2D(r, r);
    return IPoint2D{r%_width, r/_height};
  }

  /*--------------------------- [PIXEL INDEXER MIRROR IMAGE ] ----------------------------- */
  PixelIndexerMirrorImage::PixelIndexerMirrorImage(int pwidth, int pheight)
    :PixelIndexer(pwidth, pheight)
  {}

  int PixelIndexerMirrorImage::index(int px, int py) const
  {
    px %= _width;
    if (px < 0) px += _width;
    py %= _height;
    if (py < 0) py += _height;
    return (_width * py) + px;
  }

  /*-------------------------- [OUT OF BOUNDS PIXEL INDEXER EXCEPTION] ----------------------- */
  OutOfBoundsPixelIndexerException::OutOfBoundsPixelIndexerException(std::string message)
   :std::exception(), _message{std::move(message)}
  {} 

  /*-------------------------- [ PIXEL INDEXER EXCEPTION ] ------------------------------------ */
  PixelIndexerException::PixelIndexerException(int pwidth, int pheight)
    :PixelIndexer(pwidth, pheight)
  {}

  int PixelIndexerException::index(int px, int py) const
  {
    if (px < 0 || px >= _width || py < 0 || py >= _height) {
      std::ostringstream ss;
      ss << "Pixel index out of bounds: p = (" << px << ", " << py << ")";
      throw OutOfBoundsPixelIndexerException(ss.str().c_str());
    }
    return (_width * py) + px;
  }
}
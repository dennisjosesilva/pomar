#include <pomar/Math/Point.hpp>
#include <string>
#include <exception>

#ifndef PIXEL_INDEXER_HPP_INCLUDED
#define PIXEL_INDEXER_HPP_INCLUDED

namespace pomar
{
  class PixelIndexer
  {
  public:
    PixelIndexer() = delete;    
    PixelIndexer(int pwidth, int pheight);

    virtual int index(int px, int py) const = 0;
    virtual int index(int p) const { return index(p%_width, p/_width); }    
    virtual inline int index(const IPoint2D &p) const { return index(p.x(), p.y()); }
    virtual IPoint2D pixel(int p) const;
    
    virtual ~PixelIndexer() {}

  protected:
    int _width;
    int _height;
  };

  class PixelIndexerNearestBorder : public virtual PixelIndexer
  {
  public:
    PixelIndexerNearestBorder() = delete;
    PixelIndexerNearestBorder(int pwidth, int pheight);

    int index(int px, int py) const;        
  };

  class PixelIndexerDefaultValue : public virtual PixelIndexer
  {
  public:
    PixelIndexerDefaultValue() = delete;
    PixelIndexerDefaultValue(int pwidth, int pheight, int pdefaultValue = -1);

    int index(int px, int py) const;
    virtual IPoint2D pixel(int p) const;
  private:
    int _defaultValue;
  };

  class PixelIndexerMirrorImage : public virtual PixelIndexer
  {
  public:
    PixelIndexerMirrorImage() = delete;
    PixelIndexerMirrorImage(int pwidth, int pheight);

    int index(int px, int py) const;
  };

  class OutOfBoundsPixelIndexerException : public std::exception
  {
  public:
    OutOfBoundsPixelIndexerException(std::string message);    

    const char * what() throw() { return _message.c_str(); }
  private: 
    std::string _message;
  };

  class PixelIndexerException : public virtual PixelIndexer
  {
  public:
    PixelIndexerException() = delete;
    PixelIndexerException(int pwidth, int pheight);
    
    int index(int px, int py) const;
  };
}
#endif
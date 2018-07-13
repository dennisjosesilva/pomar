#include <pomar/Math/Point.hpp>
#include <string>
#include <exception>

#ifndef PIXEL_INDEXER_HPP_INCLUDED
#define PIXEL_INDEXER_HPP_INCLUDED

/** @file */

namespace pomar
{
  /**
  * Base class to classes which provide indexing pixels support(2D images).
  */
  class PixelIndexer
  {
  public:
    PixelIndexer() = delete;    
    /** 
    * Constructor which take the image width and height. 
    */
    PixelIndexer(int pwidth, int pheight);

    /** Interface to methods which take x and y coordinates and transform them in theirs index. */
    virtual int index(int px, int py) const = 0;
    /** Interface to methods which take a pixel index and transform it into a valid index or (show/return) an error. */
    virtual int index(int p) const { return index(p%_width, p/_width); }    
    /** Interface to methods which take 2D point and transform it in its index. */ 
    virtual inline int index(const IPoint2D &p) const { return index(p.x(), p.y()); }
    /** Interface to methods which take a valid index and transform it into a Point2D. */
    virtual IPoint2D pixel(int p) const;
    
    virtual ~PixelIndexer() {}

  protected:
    int _width; /*< Image's width. */
    int _height; /*< Image's height. */
  };

  
  /** Class which takes the nearest border pixel value for out of bounds pixels. */
  class PixelIndexerNearestBorder : public virtual PixelIndexer
  {
  public:
    PixelIndexerNearestBorder() = delete;
    /** Constructor. */ 
    PixelIndexerNearestBorder(int pwidth, int pheight);

    /** Transform point (px, py) into an index using the nearest border pixel 
    *   for out of bounds pixels . 
    */
    int index(int px, int py) const;        
  };

  /**
   *  Class which return a default value (provided by the user) for out of bounds pixels.
  */
  class PixelIndexerDefaultValue : public virtual PixelIndexer
  {
  public:
    PixelIndexerDefaultValue() = delete;
    /** Constructor which needs width, height and a default value (default=-1). */
    PixelIndexerDefaultValue(int pwidth, int pheight, int pdefaultValue = -1);

    /** Transform point (px, py) into an index using a default value for out of bounds 
     * pixels. 
    */
    int index(int px, int py) const;
    /**
     * Transform a pixel index (p) into its corresponding Point2D using a default
     * value for out of bounds index.
    */
    virtual IPoint2D pixel(int p) const;
  private:
    int _defaultValue;
  };

  /** Class which return mirror the image pixels for out of bounds pixels. */
  class PixelIndexerMirrorImage : public virtual PixelIndexer
  {
  public:  
    PixelIndexerMirrorImage() = delete;
    /** Constructor. */
    PixelIndexerMirrorImage(int pwidth, int pheight);

    /** Transfrom point (px, py) into an index using image mirrored pixels for out
     * of bounds pixels.
    */
    int index(int px, int py) const;
  };

  /** Exception class for out of bounds pixels.  */
  class OutOfBoundsPixelIndexerException : public std::exception
  {
  public:
    /** Constructor. */
    OutOfBoundsPixelIndexerException(std::string message);    

    /** exception message. */ 
    const char * what() throw() { return _message.c_str(); }
  private: 
    std::string _message;
  };

  /** Class which throws an exception when a pixel out of bounds is accessed. */ 
  class PixelIndexerException : public virtual PixelIndexer
  {
  public:
    PixelIndexerException() = delete;
    PixelIndexerException(int pwidth, int pheight);
    
    /** Transfrom point (px, py) into an index or throws an exception if 
     * (px, py) is out of bounds.
    */
    int index(int px, int py) const;
  };
}
#endif
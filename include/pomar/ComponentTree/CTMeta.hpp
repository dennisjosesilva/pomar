#ifndef CT_META_HPP_INCLUDED
#define CT_META_HPP_INCLUDED

namespace pomar
{

  /** Class to represent component tree Meta Information. */
  class CTMeta 
  {
  public:
    CTMeta() {}
    virtual ~CTMeta() {}
  };

  /** Component tree from Image2D meta-information*/
  class CTMetaImage2D : public virtual CTMeta
  {
  public:
    CTMetaImage2D() = delete;
    /** Constructor meta-information using image input's width,
     * height and number of channel. */ 
    CTMetaImage2D(int pwidth, int pheight, int pnchannel);

    inline int width() const { return _width; } /**< Input image width. */
    inline int height() const { return _height; } /**< Input image height. */
    inline int nchannel() const { return _nchannel; } /**< Input image number of channels. */
  private:
    int _width;
    int _height;
    int _nchannel;
  };
}
#endif
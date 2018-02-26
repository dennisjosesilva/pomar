#ifndef CT_META_HPP_INCLUDED
#define CT_META_HPP_INCLUDED

namespace pomar
{
  class CTMeta 
  {
  public:
    CTMeta() {}
  };

  class CTMetaImage2D : public virtual CTMeta
  {
  public:
    CTMetaImage2D() = delete;
    CTMetaImage2D(int pwidth, int pheight, int pnchannel);

    inline int width() const { return _width; }
    inline int height() const { return _height; }
    inline int nchannel() const { return _nchannel; }
  private:
    int _width;
    int _height;
    int _nchannel;
  };
}
#endif
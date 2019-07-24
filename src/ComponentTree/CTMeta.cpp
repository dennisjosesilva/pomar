#include <pomar/ComponentTree/CTMeta.hpp>

namespace pomar
{
  CTMetaImage2D::CTMetaImage2D(int pwidth, int pheight, int pnchannel)
    :_width{pwidth}, _height{pheight}, _nchannel{pnchannel}
  {}

  CTMetaImage2D::~CTMetaImage2D()
  {}
}
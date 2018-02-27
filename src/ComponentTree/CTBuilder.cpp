#include <pomar/ComponentTree/CTBuilder.hpp>

namespace pomar 
{
  /* ========================================[ FIND ROOT ]========================================================= */
  int CTBuilder::findRoot(std::vector<int>& zpar, int p) const
  {
    if (zpar[p] != p)
      zpar[p] = findRoot(zpar, zpar[p]);
    return zpar[p];
  }
}
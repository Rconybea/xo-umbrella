/* file PxSize.cpp
 *
 * author: Roland Conybeare, Sep 2022
 */

#include "PxSize.hpp"
#include "xo/reflect/StructReflector.hpp"

namespace xo {
  using xo::reflect::StructReflector;

  namespace option {
    void
    PxSize::reflect_self()
    {
      StructReflector<PxSize> sr;

      if (sr.is_incomplete()) {
    REFLECT_MEMBER(sr, px);
    REFLECT_MEMBER(sr, sz);
      }
    } /*reflect_self*/
  } /*namespace option*/
} /*namespace xo*/

/* end PxSize.cpp */

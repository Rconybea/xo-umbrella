/* file Px2.cpp
 *
 * author: Roland Conybeare, Aug 2022
 */

#include "Px2.hpp"
#include "xo/reflect/StructReflector.hpp"

namespace xo {
  using xo::reflect::StructReflector;

  namespace option {
    void
    Px2::reflect_self()
    {
      StructReflector<Px2> sr;

      if (sr.is_incomplete()) {
    REFLECT_MEMBER(sr, px_v);
      }
    } /*reflect_self*/
  } /*namespace option*/
} /*namespace xo*/

/* end Px2.cpp */

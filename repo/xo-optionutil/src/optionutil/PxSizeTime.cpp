/* @file PxSizeTime.cpp */

#include "PxSizeTime.hpp"
#include "xo/reflect/StructReflector.hpp"

namespace xo {
  using xo::time::hms_msec;
  using xo::reflect::StructReflector;

  namespace option {
    void
    PxSizeTime::reflect_self()
    {
      StructReflector<PxSizeTime> sr;

      if (sr.is_incomplete()) {
          REFLECT_EXPLICIT_MEMBER(sr, "px", &PxSize::px_);
          REFLECT_EXPLICIT_MEMBER(sr, "sz", &PxSize::sz_);
          REFLECT_MEMBER(sr, exch_tm);
      }
    } /*reflect_self*/

    void
    PxSizeTime::display(std::ostream & os) const
    {
        os << "{pzt "
           << size() << "x "
           << px()
           << " @" << hms_msec::utc(exch_tm_)
           << "}";
    } /*display*/
  } /*namespace option*/
} /*namespace xo*/

/* end PxSizeTime.cpp */

/* @file PxSizeTime2.cpp */

#include "PxSizeTime2.hpp"
#include "xo/reflect/StructReflector.hpp"

namespace xo {
  using xo::reflect::StructReflector;
  using xo::time::hms_msec;
  using xo::fixed;

  namespace option {
      void
      PxSizeTime2::reflect_self()
      {
          StructReflector<PxSizeTime2> sr;

          if (sr.is_incomplete()) {
              REFLECT_EXPLICIT_MEMBER(sr, "sz2", &PxSize2::sz2_);
              REFLECT_EXPLICIT_MEMBER(sr, "px2", &PxSize2::px2_);
              REFLECT_MEMBER(sr, exch_tm2);
          }
      } /*reflect_self*/

      void
      PxSizeTime2::display(std::ostream & os) const
      {
          os << "{pzt2 "
             << hms_msec::utc(this->bid_exch_tm())
             << " " << this->bid_sz() << "x"
             << " " << fixed(this->bid_px().to_double(), 2)
             << " - " << fixed(this->ask_px().to_double(), 2)
             << " x" << this->ask_sz()
             << " " << hms_msec::utc(this->ask_exch_tm())
             << "}";
      } /*display*/
  } /*namespace option*/
} /*namespace xo*/

/* end PxSizeTime2.cpp */

/* @file PxSizeTime.hpp */

#pragma once

#include "PxSize.hpp"
#include "xo/indentlog/timeutil/timeutil.hpp"

namespace xo {
  namespace option {
    /* a price+size+time */
    class PxSizeTime : public PxSize {
    public:
      using utc_nanos = xo::time::utc_nanos;

    public:
      PxSizeTime() = default;
      PxSizeTime(PxSize const & pxz, utc_nanos t0) : PxSize(pxz), exch_tm_{t0} {}
      PxSizeTime(Price px, Size z, utc_nanos t0) : PxSize(px, z), exch_tm_{t0} {}

      static void reflect_self();

      utc_nanos exch_tm() const { return exch_tm_; }

      void assign_exch_tm(utc_nanos t0) { exch_tm_ = t0; }

      /* e.g.
       *   {pzt 1x 1.20 @15:12:45.123}
       */
      void display(std::ostream & os) const;

      // ----- Inherited from PxSize -----

      // Price px() const;
      // Size size() const;
      // bool is_present() const;
      // void assign_size(Size x);
      // void assign_px(Price x);

    private:
      utc_nanos exch_tm_;
    }; /*PxSizeTime*/

  } /*namespace option*/
} /*namespace xo*/

/* end PxSizeTime.hpp */

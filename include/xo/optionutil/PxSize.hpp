/* @file PxSize.hpp */

#pragma once

#include "Price.hpp"
#include "Size.hpp"

namespace xo {
  namespace option {
    /* a price+size */
    class PxSize {
    public:
      PxSize() = default;
      PxSize(Price px, Size z) : px_{px}, sz_{z} {}

      static bool is_equal(PxSize const & x, PxSize const & y) {
    return ((x.size() == y.size())
        && (x.px() == y.px()));
      } /*is_equal*/

      /* reflect PxSize object representation */
      static void reflect_self();

      Price px() const { return px_; }
      Size size() const { return sz_; }

      bool is_present() const { return this->sz_.is_valid(); }

      void assign_size(Size x) { this->sz_ = x; }
      void assign_px(Price x) { this->px_ = x; }

    private:
      friend class PxSizeTime;

    private:
      Price px_;
      Size sz_;
    }; /*PxSize*/
  } /*namespace option*/
} /*namespace xo*/

/* end PxSize.hpp */

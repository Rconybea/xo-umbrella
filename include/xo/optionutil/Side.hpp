/* @file Side.hpp */

#pragma once

#include "Price.hpp"

namespace xo {
  namespace option {
    enum class Side { bid=0, ask=1, end=2 };

    /* get integer representation for side x */
    inline int32_t side2int(Side x) { return static_cast<int32_t>(x); }

    /* s=Side::bid:
     *   -ve if x < y    (fades)
     *     0 if x == y
     *   +ve if x > y    (improves)
     * s=Side::ask:
     *   -ve if x > y    (fades)
     *     0 if x == y
     *   +ve if x < y    (improves)
     */
    inline int32_t side_compare_px(Side s, Price x, Price y) {
      switch(s) {
      case Side::bid:
      case Side::end:
        return (x.rep() - y.rep());
      case Side::ask:
          return (y.rep() - x.rep());
      }

      return 0;
    } /*side_compare_px*/

    /* s=Side::bid:
     *   +ve if x > y   (bid x improves y)
     *     0 if x == y
     *   -ve if x < y
     * s=Side::ask:
     *   -ve if x > y
     *     0 if x == y
     *   +ve if x < y   (offer x improves y)
     */
    inline Price side_improvement_dpx(Side s, Price x, Price y) {
        switch (s) {
        case Side::bid:
            return x - y;
        case Side::ask:
            return y - x;
        case Side::end:
            return Price::zero();
        }

        return Price::zero();
    } /*side_improvement_dpx*/

    inline bool side_matches_or_improves_px(Side s, Price x, Price y) {
      return side_compare_px(s, x, y) >= 0;
    } /*side_matches_or_improves_px*/

    /* fade price px by amount dy */
    inline double fade_by(Side s, double px, double dy) {
      switch(s) {
      case Side::bid:
          return px - dy;
      case Side::ask:
          return px + dy;
      case Side::end:
          return 0.0;
      }

      return 0.0;
    } /*fade_by*/

    /* Use:
     *   for(Side s : SideIter()) {
     *     // do something with s
     *   }
     */
    class SideIter {
    public:
      SideIter() : side_{Side::bid} {}
      explicit SideIter(Side x) : side_(x) {}

      SideIter begin() const { return SideIter(Side::bid); }
      SideIter end() const { return SideIter(Side::end); }

      SideIter & operator++() { side_ = static_cast<Side>(static_cast<int32_t>(side_) + 1); return *this; }

      Side operator*() const { return side_; }
      operator Side () const { return side_; }

    private:
      Side side_;
    }; /*SideIter*/
  } /*namespace option*/
} /*namespace xo*/

/* end Side.hpp */

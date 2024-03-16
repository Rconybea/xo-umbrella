/* @file PxSize2.cpp */

#include "PxSize2.hpp"
#include "PxSize.hpp"
#include "xo/reflect/StructReflector.hpp"

namespace xo {
  using xo::reflect::StructReflector;

  namespace option {
    PxSize2
    PxSize2::with_size(Size2 const & z2, Px2 const & px2)
    {
      return PxSize2(z2.bid_z(), px2.bid_px(), px2.ask_px(), z2.ask_z());
    } /*with_size*/

    bool
    PxSize2::is_equal(PxSize2 const & x, PxSize2 const & y)
    {
      return ((x.sz2() == y.sz2())
          && (x.px2() == y.px2()));
    } /*is_equal*/

    void
    PxSize2::reflect_self()
    {
      StructReflector<PxSize2> sr;

      if (sr.is_incomplete()) {
    REFLECT_MEMBER(sr, sz2);
    REFLECT_MEMBER(sr, px2);
      }
    } /*reflect_self*/

    void
    PxSize2::display(std::ostream & os) const
    {
      /* e.g.
       *   {pxz2 1x 1.20-1.25 x1}
       */

      os << "{pxz2 "
     << sz2_.bid_z() << "x "
     << px2_
     << " x" << sz2_.ask_z()
     << "}";
    } /*display*/

    void
    PxSize2::assign_pxz(Side s,
            PxSize const & pxz)
    {
      this->sz2_.assign_size(s, pxz.size());
      this->px2_.assign_px(s, pxz.px());
    } /*assign_pxz*/

    void
    PxSize2::assign_pxz(Side s,
            PxSize2 const & pxz2)
    {
      this->sz2_.assign_size(s, pxz2.size(s));
      this->px2_.assign_px(s, pxz2.px(s));
    } /*set_side*/
  } /*namespace option*/
} /*namespace xo*/

/* end PxSize2.cpp */

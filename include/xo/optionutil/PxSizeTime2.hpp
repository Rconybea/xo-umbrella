/* @file PxSizeTime2.hpp */

#include "PxSize2.hpp"
#include "PxSizeTime.hpp"
#include "xo/indentlog/timeutil/timeutil.hpp"

namespace xo {
  namespace option {
    class PxSizeTime2 : public PxSize2 {
    public:
      using utc_nanos = xo::time::utc_nanos;

    public:
      PxSizeTime2() = default;
      PxSizeTime2(utc_nanos bid_tm, Size bid_sz, Price bid_px,
          Price ask_px, Size ask_sz, utc_nanos ask_tm)
    : PxSize2(bid_sz, bid_px, ask_px, ask_sz), exch_tm2_{bid_tm, ask_tm} {}
      PxSizeTime2(PxSize2 const & pxz2, std::array<utc_nanos,2> const & exch_tm2)
    : PxSize2(pxz2), exch_tm2_{exch_tm2} {}

      /* reflect PxSizeTime2 object representation */
      static void reflect_self();

      utc_nanos exch_tm(Side s) const { return exch_tm2_[side2int(s)]; }
      utc_nanos bid_exch_tm() const { return exch_tm2_[0]; }
      utc_nanos ask_exch_tm() const { return exch_tm2_[1]; }

      PxSizeTime to_pzt(Side s) const { return PxSizeTime(this->px(s), this->size(s), this->exch_tm(s)); }
      PxSizeTime to_bid_pzt() const { return PxSizeTime(this->bid_px(), this->bid_sz(), this->bid_exch_tm()); }
      PxSizeTime to_ask_pzt() const { return PxSizeTime(this->ask_px(), this->ask_sz(), this->ask_exch_tm()); }

      /* writes like:
       *   {pzt2 15:15:01.123 1x 1.20-1.25 x3 15:14:59.052}
       */
      void display(std::ostream & os) const;

      void assign_pzt(Side s, PxSizeTime const & pzt) {
    this->assign_pxz(s, pzt);
    exch_tm2_[side2int(s)] = pzt.exch_tm();
      } /*assign_pzt*/

      // ----- Inherited from PxSize2 -----

      //Size bid_sz() const;
      //Price bid_px() const;
      //Price ask_px() const;
      //Size ask_sz() const;

      //Size size(Side s) const;
      //Price px(Side s) const;

      //Price mid_px() const;

      //bool is_side_present(Side s) const;
      //bool is_bid_present() const;
      //bool is_ask_present() const;

      //void display(std::ostream & os) const;

      //void assign_pxz(Side s, PxSize2 const & pxz2);

    private:
      /* bid/ask exchange timestamp */
      std::array<utc_nanos, 2> exch_tm2_;
    }; /*PxSizeTime2*/
  } /*namespace option*/
} /*namespace xo*/


/* end PxSizeTime2.hpp */

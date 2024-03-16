/* @file PxSize2.hpp */

#pragma once

#include "Px2.hpp"
#include "Size2.hpp"
#include "PxSize.hpp"
#include <array>

namespace xo {
    namespace option {
        /* a bid/ask pair with size */
        class PxSize2 {
        public:
            PxSize2() = default;
            PxSize2(Size bid_sz, Price bid_px, Price ask_px, Size ask_sz)
                : sz2_{bid_sz, ask_sz}, px2_{bid_px, ask_px} {}

            static PxSize2 with_size(Size2 const & z2, Px2 const & px2);
            /* memberwise equality test */
            static bool is_equal(PxSize2 const & x, PxSize2 const & y);

            /* reflect PxSize2 object representation */
            static void reflect_self();

            Size2 const & sz2() const { return sz2_; }
            Px2 const & px2() const { return px2_; }

            Size bid_sz() const { return sz2_.size(Side::bid); }
            Price bid_px() const { return px2_.px(Side::bid); }
            Price ask_px() const { return px2_.px(Side::ask); }
            Size ask_sz() const { return sz2_.size(Side::ask); }

            Size size(Side s) const { return sz2_.size(s); }
            Price px(Side s) const { return px2_.px(s); }

            Price spread() const { return px2_.px(Side::ask) - px2_.px(Side::bid); }
            Price mid_px() const { return 0.5 * (px2_.px(Side::bid) + px2_.px(Side::ask)); }

            PxSize to_pxz(Side s) const { return PxSize(this->px(s), this->size(s)); }

            bool is_side_present(Side s) const { return sz2_.size(s).is_valid(); }

            bool is_bid_present() const { return this->is_side_present(Side::bid); }
            bool is_ask_present() const { return this->is_side_present(Side::ask); }

            /* e.g.
             *   PxSize2 p(1, 1.2, 1.25, 3);
             *   p.display(os)
             * writes
             *   {pxz2 1x 1.20-1.25 x3}
             * on stream os
             */
            void display(std::ostream & os) const;

            void assign_pxz(Side s, PxSize const & pxz);
            /* set state for side s from corresponding fields in pxz2 */
            void assign_pxz(Side s, PxSize2 const & pxz2);

        private:
            friend class PxSizeTime2;

        private:
            /* bid,ask size */
            Size2 sz2_;
            /* bid,ask price */
            Px2 px2_;
        }; /*PxSize2*/

        inline bool operator==(PxSize2 const & x, PxSize2 const & y) {
            return PxSize2::is_equal(x, y);
        } /*operator==*/

        inline bool operator!=(PxSize2 const & x, PxSize2 const & y) {
            return !PxSize2::is_equal(x, y);
        } /*operator!=*/

        inline std::ostream &
        operator<<(std::ostream & os, PxSize2 const & x) {
            x.display(os);
            return os;
        } /*operator<<*/
    } /*namespace option*/
} /*namespace xo*/

/* end PxSize2.hpp */

/* @file Px2.hpp */

#pragma once

#include "Price.hpp"
#include "Side.hpp"
#include <array>

namespace xo {
    namespace option {
        /* a bid/ask pair */
        class Px2 {
        public:
            Px2() = default;
            Px2(Price bid, Price ask) : px_v_{bid, ask} {}

            static bool equals(Px2 const & x, Px2 const & y) {
                return (x.bid_px() == y.bid_px()) && (x.ask_px() == y.ask_px());
            } /*equals*/

            /* reflect Px2 object representation */
            static void reflect_self();

            Price bid_px() const { return px_v_[side2int(Side::bid)]; }
            Price ask_px() const { return px_v_[side2int(Side::ask)]; }
            Price px(Side s) const { return px_v_[side2int(s)]; }

            Price spread() const { return this->ask_px() - this->bid_px(); }

            bool fades(Side s, Px2 const & px2) const {
                return side_compare_px(s, this->px(s), px2.px(s)) < 0;
            }

            /* compare x with price on side s
             *   s   return
             * -------------------
             *  bid  x - .bid_px()
             *  ask  .ask_px() - x
             */
            double improvement(Side s, double x) {
                return side_improvement_dpx(s, Price::from_double(x), this->px(s)).to_double();
            } /*improvement*/

            Px2 & assign_bid_px(Price x) { return this->assign_px(Side::bid, x); }
            Px2 & assign_ask_px(Price x) { return this->assign_px(Side::ask, x); }

            Px2 & assign_px(Side s, Price x) {
                this->px_v_[side2int(s)] = x;
                return *this;
            } /*assign_px*/

            Px2 & assign_px(Side s, Px2 const & px2) {
                return this->assign_px(s, px2.px(s));
            } /*assign_px*/

        private:
            /* .px_v[0] = bid price
             * .px_v[1] = ask price
             */
            std::array<Price, 2> px_v_;
        }; /*Px2*/

        inline int32_t side_compare_px(Side s, Px2 const & x, Px2 const & y) {
            return side_compare_px(s, x.px(s), y.px(s));
        } /*side_compare_px*/

        inline bool side_matches_or_improves_px(Side s, Px2 const & x, Px2 const & y) {
            return side_matches_or_improves_px(s, x.px(s), y.px(s));
        } /*side_matches_or_improves_px*/

        inline Price side_improvement_dpx(Side s, Px2 const & x, Px2 const & y) {
            return side_improvement_dpx(s, x.px(s), y.px(s));
        } /*side_improvement_dpx*/

        inline bool operator==(Px2 const & x, Px2 const & y) {
            return Px2::equals(x, y);
        }

        inline bool operator!=(Px2 const & x, Px2 const & y) {
            return !Px2::equals(x, y);
        } /*operator!=*/

        inline std::ostream &
        operator<<(std::ostream & os,
                   Px2 const & px2)
        {
            using xo::fixed;

            os << fixed(px2.bid_px().to_double(), 2) << "-" << fixed(px2.ask_px().to_double(), 2);
            return os;
        } /*operator<<*/

    } /*namespace option*/
} /*namespace xo*/

/* end Px2.hpp */

/* @file Size2.hpp */

#pragma once

#include "Size.hpp"
#include "Side.hpp"
#include <array>

namespace xo {
    namespace option {
        /* a pair of Sizes,  for bid/ask */
        class Size2 {
        public:
            Size2() = default;
            Size2(Size bid_z, Size ask_z) : sz_v_{bid_z, ask_z} {}

            static Size2 dup(Size x) { return Size2(x, x); }

            static Size2 from_int2(int32_t bid_z, int32_t ask_z) {
                return Size2(Size::from_int(bid_z), Size::from_int(ask_z));
            } /*from_int2*/

            static Size2 max(Size2 const & x, Size2 const & y) {
                return Size2(Size::max(x.bid_z(), y.bid_z()),
                             Size::max(x.ask_z(), y.ask_z()));
            } /*max*/

            static bool equals(Size2 const & x, Size2 const & y) {
                return (x.bid_z() == y.bid_z()) && (x.ask_z() == y.ask_z());
            } /*equals*/

            /* reflect Size2 object representation */
            static void reflect_self();

            Size bid_z() const { return this->size(Side::bid); }
            Size ask_z() const { return this->size(Side::ask); }
            Size size(Side s) const { return this->sz_v_[side2int(s)]; }

            /* round (down) to multiple of m */
            Size2 round(uint32_t m) const {
                return Size2(this->bid_z().round(m),
                             this->ask_z().round(m));
            } /*round*/

            Size2 & assign_bid_sz(Size x) { this->assign_size(Side::bid, x); return *this; }
            Size2 & assign_ask_sz(Size x) { this->assign_size(Side::ask, x); return *this; }

            Size2 & assign_size(Side s, Size x) {
                this->sz_v_[side2int(s)] = x;
                return *this;
            } /*assign_size*/

            Size2 & assign_size(Side s, Size2 const & x2) {
                return this->assign_size(s, x2.size(s));
            } /*assign_size*/

        private:
            /* sz_v[0] = bid size
             * sz_v[1] = ask size
             */
            std::array<Size, 2> sz_v_;
        }; /*Size2*/

        inline bool operator==(Size2 const & x, Size2 const & y) {
            return Size2::equals(x, y);
        } /*operator==*/

        inline bool operator!=(Size2 const & x, Size2 const & y) {
            return !Size2::equals(x, y);
        } /*operator!=*/

        inline std::ostream & operator<<(std::ostream & os,
                                         Size2 const & x2)
        {
            os << x2.bid_z() << "x/" << x2.ask_z() << "x";
            return os;
        } /*operator<<*/

    } /*namespace option*/
} /*namespace xo*/

/* end Size2.hpp */

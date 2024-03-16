/* file Ivol2.hpp
 *
 * author: Roland Conybeare, Sep 2022
 */

#pragma once

#include "Ivol.hpp"
#include "Side.hpp"
#include <array>

namespace xo {
    namespace option {
        /* a (bid/ask)-associated pair of ivols */
        class Ivol2 {
        public:
            Ivol2() = default;
            Ivol2(Ivol bid, Ivol ask) : ivol_v_{bid, ask} {}

            /* reflect Ivol2 object representation */
            static void reflect_self();

            Ivol bid_ivol() const { return ivol_v_[side2int(Side::bid)]; }
            Ivol ask_ivol() const { return ivol_v_[side2int(Side::ask)]; }
            Ivol ivol(Side s) const { return ivol_v_[side2int(s)]; }

            Ivol2 & assign_ivol(Side s, Ivol x) {
                this->ivol_v_[side2int(s)] = x;
                return *this;
            }

        private:
            /* .ivol_v[0] = bid implied vol
             * .ivol_v[1] = ask implied vol
             */
            std::array<Ivol, 2> ivol_v_;
        }; /*Ivol2*/

        inline std::ostream &
        operator<<(std::ostream & os,
                   Ivol2 const & iv2)
        {
            using xo::fixed;

            os << "<Ivol2 ";
            iv2.bid_ivol().display_unwrapped(os);
            os << "-";
            iv2.ask_ivol().display_unwrapped(os);
            os << ">";

            return os;
        } /*operator<<*/
    } /*namespace option*/
} /*namespace xo*/

/* end Ivol2.hpp */

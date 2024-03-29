/* @file linear_interpolation.hpp */

#pragma once

#include "xo/indentlog/scope.hpp"
#include "xo/indentlog/print/vector.hpp"
#include "xo/indentlog/print/tag.hpp"
#include <array>
#include <cstdint>

namespace xo {
    namespace obs {
        /** @brief compare two values (x,y);  result r: r<0 if x<y;  r=0 if x=y;  r>0 if x>y
         *
         *  Note: may be able to specialize this for other types to use 1 comparison instead of 2.
         **/
        template <typename Value>
        inline int
        compare(Value x, Value y)
        {
            if (x < y) {
                return -1;
            } else if (x == y) {
                return 0;
            } else {
                return +1;
            }
        }

        /** @brief search for greatest lower bound of x in array v[].
         *
         *  Returns index in v[] of lower bound;  -1 if lower bound does not exist
         *  Accept a solution i with x=v[i] iff is_closed=true.
         *
         *  Use proportional search to guess next index.
         *
         *  Require:
         *  - Value is comparable (has operators ==, <, >, !=, <=, >=)
         *  - Values are totally ordered
         *  - Value - Value -> T,  with:
         *  - double * T -> T
         *  - T / T -> double
         *  - T + T -> T
         *
         *  - IndexType(-1) is defined
         *  - IndexType + int -> IndexType
         *  - IndexType - int -> IndexType
         *  - IndexType - IndexType -> IndexType
         *  - double * IndexType -> IndexType
         *  - Note these requirements satisfied for integral types int32_t etc.
         *
         *  Remarks:
         *  - Value can be an affine type like utc_nanos: we don't require Value+Value
         **/
        template <typename Array,
                  typename Value = typename Array::value_type,
                  typename IndexType = std::int32_t>
        auto
        proportional_glb_index(Value const & x,
                               Array const & v,
                               bool is_closed,
                               bool debug_flag = false) -> IndexType
        {
            IndexType i_lo = 0;
            IndexType i_hi = std::size(v);

            scope log(XO_DEBUG(debug_flag),
                               xtag("x", x),
                               xtag("v", v),
                               xtag("is_closed", is_closed));
            log && log(xtag("i_hi", i_hi));

            if (i_hi == 0)
                return IndexType(-1);

            auto v0 = v[0];

            int cmp;

            cmp = compare(x, v0);
            if ((cmp < 0) || (!is_closed && (cmp  == 0)))
                return IndexType(-1);

            /* v0 is a lower bound for x */

            auto vn1 = v[i_hi - 1];

            cmp = compare(x, vn1);
            if ((cmp > 0) || (is_closed && (cmp == 0))) {
                log && log(xtag("i_hi", i_hi), xtag("vn1", vn1), xtag("cmp", cmp), xtag("return", vn1));
                return IndexType(i_hi - 1);
            }

            size_t n_loop = 0;

            while (i_lo + 1 < i_hi) {
                /* invariant:
                 * - v[lo] is a lower bound for x
                 * - v[hi] is an upper bound for x
                 * - therefore target lower bound index in [lo, hi)
                 */

                log && log(xtag("i_lo", i_lo), xtag("i_hi", i_hi));

                IndexType i_lo1 = i_lo + 1;
                IndexType i_hi1 = i_hi - 1;

                auto v_lo1 = v[i_lo1];
                auto v_hi1 = v[i_hi1];

                /* next estimate 'mid' will be in [lo+1, hi)
                 *
                 * Any p in [0.0, 1.0] will eventually converge to solution,
                 * but can contrive examples where search costs O(n) instead of O(log n)
                 *
                 * Address by narrowing p to a smaller interval [0.1, 0.9] around 0.5,
                 * so we guarantee logarithmic convergence.
                 */

                auto p = std::max(0.1,
                                  std::min((x - v_lo1) / static_cast<double>(v_hi1 - v_lo1),
                                           0.9));

                /* next estimate.  Sharp if values are equally spaced */
                IndexType i_mid = i_lo1 + (p * (i_hi1 - i_lo1));

                auto v_mid = v[i_mid];

                log && log(xtag("n_loop", n_loop), xtag("p", p), xtag("i_mid", i_mid), xtag("v_mid", v_mid));

                cmp = compare(x, v_mid);
                if ((cmp > 0) || (is_closed && (cmp == 0)))
                    i_lo = i_mid;
                else
                    i_hi = i_mid;

                ++n_loop;
            }

            return i_lo;
        } /*proportional_glb_index*/

        /**
         * Require:
         * - IndexType is signed (or at least implements IndexType(-1))
         * - xv is sorted in ascending order
         * - size(xv)=size(yv)
         **/
        template <typename XValues, typename YValues, typename IndexType = std::int32_t>
        struct linear_interpolation {
            using domain_value_type = typename XValues::value_type;
            using range_value_type = typename YValues::value_type;

            /** @brief evaluate interpolated function at x, subject to f(xv[i]) = yv[i] **/
            range_value_type operator()(domain_value_type const & x,
                                        XValues const & xv,
                                        YValues const & yv,
                                        range_value_type const & y_lo_sentinel,
                                        range_value_type const & y_hi_sentinel) const
                {
                    std::int32_t i = proportional_glb_index(x, xv, true /*is_closed*/);

                    if (i == -1) {
                        /* no lower bound: -> use sentinel */
                        return y_lo_sentinel;
                    } else if (i == std::size(xv) - 1) {
                        return y_hi_sentinel;
                    } else /* i in [0, .., n-2] */ {
                        /* interpolate linearly between p1 and p2:
                         *   p1 = (xv[i], yv[i])
                         *   p2 = (xv[i+1], yv[i+1])
                         */
                        auto x_lo = xv[i];
                        auto y_lo = yv[i];
                        auto x_hi = xv[i+1];
                        auto y_hi = yv[i+1];

                        double p = (x - x_lo) / (x_hi - x_lo);

                        return y_lo + p * (y_hi - y_lo);
                    }
                } /*operator()*/
        }; /*linear_interpolation*/
    } /*namespace obs*/

} /*namespace xo*/


/* end linear_interpolation.hpp */

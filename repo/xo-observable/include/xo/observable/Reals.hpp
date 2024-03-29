/* @file Reals.hpp */

#pragma once

#include "AbstractSet.hpp"
#include "IntervalSet.hpp"

namespace xo {
    namespace obs {
        /** @class Reals
            @brief represents the set IR of real numbers
            (we will avert our eyes + pretend every real number can be represented
            in double precision)
        **/
        class Reals : public AbstractSet {
        public:
            static ref::rp<Reals> instance();
            static ref::rp<IntervalSet<double>> interval(double lo, double hi,
                                                         bool lo_closed = true, bool hi_closed = false);

            virtual bool is_totally_ordered() const override { return true; }
            virtual bool is_enumerable() const override { return false; }
            virtual bool is_finite() const override { return false; }

            virtual int64_t cardinality() const override { return -1; }

            virtual void display(std::ostream & os) const override;
        };
    } /*namespace obs*/

} /*namespace xo*/


/* end Reals.hpp */

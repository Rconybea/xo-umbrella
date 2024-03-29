/* @file Integers.hpp */

#pragma once

#include "AbstractSet.hpp"
#include "IntervalSet.hpp"

namespace xo {
    namespace obs {
        class Integers : public AbstractSet {
        public:
            using int64_t = std::int64_t;

        public:
            static ref::rp<Integers> instance();
            static ref::rp<IntervalSet<int64_t>> interval(int64_t lo, int64_t hi,
                                                          bool lo_closed = true, bool hi_closed = false);

            virtual bool is_totally_ordered() const override { return true; }
            virtual bool is_enumerable() const override { return false; }
            virtual bool is_finite() const override { return false; }
            virtual int64_t cardinality() const override { return -1; }

            virtual void display(std::ostream & os) const override;
        };
    } /*namespace obs*/
} /*namespace xo*/

/* end Integers.hpp */

/* @file Integers.cpp */

#include "Integers.hpp"

namespace xo {
    namespace obs {
        using namespace std;

        ref::rp<Integers>
        Integers::instance() {
            static ref::rp<Integers> s_instance;

            if (!s_instance)
                s_instance = new Integers();

            return s_instance;
        }

        ref::rp<IntervalSet<int64_t>>
        Integers::interval(int64_t lo, int64_t hi,
                           bool lo_closed, bool hi_closed)
        {
            return IntervalSet<int64_t>::make(instance(),
                                              lo, hi,
                                              lo_closed,
                                              hi_closed);
        } /*interval*/

        void
        Integers::display(ostream & os) const {
            os << "<Integers>";
        }
    } /*namespace obs*/

} /*namespace xo*/


/* end Integers.cpp */

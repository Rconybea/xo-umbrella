/* @file Reals.cpp */

#include "Reals.hpp"

namespace xo {
    namespace obs {
        using namespace std;

        ref::rp<Reals>
        Reals::instance() {
            static ref::rp<Reals> s_instance;

            if (!s_instance)
                s_instance = new Reals();

            return s_instance;
        }

        ref::rp<IntervalSet<double>>
        Reals::interval(double lo, double hi,
                        bool lo_closed, bool hi_closed)
        {
            return IntervalSet<double>::make(instance(),
                                             lo, hi,
                                             lo_closed,
                                             hi_closed);
        } /*interval*/

        void
        Reals::display(ostream & os) const {
            os << "<Reals>";
        }
    } /*namespace obs*/
} /*namespace xo*/

/* end Reals.cpp */

/* @file interpolated_fn_test.cpp */

#include "xo/observable/interpolated_fn.hpp"
#include <catch2/catch.hpp>

namespace xo {
    using xo::obs::interpolated_fn;
    using namespace std;

    namespace ut {
        namespace {
            TEST_CASE("interpolated-fn", "[interpolated-fn]") {
                //constexpr bool c_debug_flag = false;

                // can get bits from /dev/random by uncommenting the 2nd line below
                //uint64_t seed = xxx;
                //rng::Seed<xoshio256ss> seed;

                //auto rng = xo::rng::xoshiro256ss(seed);

                //scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.interpolated-fn"), xtag("foo", foo), ...);
                //log && log("(A)", xtag("foo", foo));

                array<double, 3> xvalues { -1.0, 0.0, 1.0 };
                array<double, 3> yvalues {  2.0, 3.0, 1.0 };

                interpolated_fn<domain, range,
                                decltype(xvalues), decltype(yvalues)> fn;

            } /*TEST_CASE(interpolated-fn)*/
        } /*namespace */
    } /*namespace ut*/
} /*namespace xo*/


/* end interpolated_fn_test.cpp */

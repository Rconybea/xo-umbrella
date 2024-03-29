/* @file quantity.test.cpp */

#include "xo/observable/quantity.hpp"
//#include <xo/randomgen/random_seed.hpp>
//#include <xo/randomgen/xoshiro256.hpp>
#include <xo/indentlog/scope.hpp>
#include <xo/indentlog/print/tag.hpp>
#include <catch2/catch.hpp>

namespace xo {
    using xo::obs::quantity;
#ifdef OBSOLETE
    using xo::obs::qty::time;
#endif
    using xo::obs::qty::seconds;
    using xo::obs::qty::minutes;
    namespace units = xo::obs::units;

    namespace ut {
        /* use 'testcase' snippet to add test cases here */
        TEST_CASE("quantity", "[quantity]") {
            //constexpr bool c_debug_flag = false;

            // can get bits from /dev/random by uncommenting the 2nd line below
            //uint64_t seed = xxx;
            //rng::Seed<xoshio256ss> seed;

            //auto rng = xo::rng::xoshiro256ss(seed);

            //scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.quantity"), xtag("foo", foo), ...);
            //log && log("(A)", xtag("foo", foo));

            quantity<units::second, int64_t> t = seconds(1L);

            REQUIRE(t.scale() == 1);
        } /*TEST_CASE(quantity)*/

        TEST_CASE("add1", "[quantity]") {
            constexpr bool c_debug_flag = true;

            scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.add1"));

            quantity<units::second, int64_t> t1 = seconds(1);
            quantity<units::second, int64_t> t2 = seconds(2);

            auto sum = t1 + t2;

            log && log(xtag("sum.unit", sum.unit_cstr()));

            static_assert(std::same_as<decltype(sum)::unit_type, units::second>);

            REQUIRE(sum.scale() == 3);

        } /*TEST_CASE(add1)*/

        TEST_CASE("add2", "[quantity]") {
            constexpr bool c_debug_flag = true;

            scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.add2"));

            quantity<units::second, int64_t> t1 = seconds(1);
            auto m2 = minutes(2);

            log && log(xtag("m2.scale", m2.scale()), xtag("m2.unit", m2.unit_cstr()));

            log && log(xtag("m2_sec", m2.in_units_of<units::second, int64_t>()));

            quantity<units::second, int64_t> t2 = m2;

            log && log(xtag("t1.unit", t1.unit_cstr()), xtag("t2.unit", t2.unit_cstr()));

            auto sum = t1 + t2;

            log && log(xtag("sum.unit", sum.unit_cstr()));

            static_assert(std::same_as<decltype(sum)::unit_type, units::second>);

            REQUIRE(sum.scale() == 121);

        } /*TEST_CASE(add1)*/

        TEST_CASE("add3", "[quantity]") {
            quantity<units::second, int64_t> t1 = seconds(1);
            quantity<units::minute, int64_t> t2 = minutes(2);

            /* sum will take unit from lhs argument to + */
            auto sum = t1 + t2;

            static_assert(std::same_as<decltype(sum)::unit_type, units::second>);

            REQUIRE(sum.scale() == 121);
        } /*TEST_CASE(add3)*/
    } /*namespace ut*/
} /*namespace xo*/

/* end quantity.test.cpp */

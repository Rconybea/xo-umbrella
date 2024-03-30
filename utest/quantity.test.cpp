/* @file quantity.test.cpp */

#include "xo/observable/quantity.hpp"
//#include <xo/randomgen/random_seed.hpp>
//#include <xo/randomgen/xoshiro256.hpp>
#include <xo/indentlog/scope.hpp>
#include <xo/indentlog/print/tag.hpp>
#include <catch2/catch.hpp>

namespace xo {
    using xo::obs::quantity;

    using xo::obs::qty::milliseconds;
    using xo::obs::qty::seconds;
    using xo::obs::qty::minutes;

    using xo::obs::unit_conversion_factor_t;
    using xo::obs::unit_cartesian_product_t;
    using xo::obs::unit_invert_t;
    using xo::obs::unit_abbrev_v;
    using xo::obs::same_dimension_v;
    using xo::obs::from_ratio;
    using xo::obs::stringliteral_from_ratio;
    using xo::obs::ratio2str_aux;
    using xo::obs::cstr_from_ratio;
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

        TEST_CASE("add4", "[quantity]") {
            constexpr bool c_debug_flag = true;

            scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.add4"));

            using u_kgps = unit_cartesian_product_t<units::kilogram, unit_invert_t<units::second>>;
            using u_gpm = unit_cartesian_product_t<units::gram, unit_invert_t<units::minute>>;

            log && log(xtag("u_kgps", unit_abbrev_v<u_kgps>.c_str()));
            log && log(xtag("u_gpm", unit_abbrev_v<u_gpm>.c_str()));

            static_assert(same_dimension_v<u_kgps, u_gpm>);

            using convert_type = unit_conversion_factor_t<u_kgps, u_gpm>;

            log && log(xtag("u_kgps->u_gpm", cstr_from_ratio<convert_type>()));

            CHECK(strcmp(cstr_from_ratio<convert_type>(), "60000") == 0);
            CHECK(from_ratio<int64_t, convert_type>() == 60000);

            auto q1 = quantity<u_kgps, double>::promote(0.1);
            auto q2 = quantity<u_gpm, double>();

            q2 = q1;

            log && log(xtag("q1", q1), xtag("q2", q2));
        } /*TEST_CASE(add4)*/

        TEST_CASE("mult1", "[quantity]") {
            constexpr bool c_debug_flag = true;

            // can get bits from /dev/random by uncommenting the 2nd line below
            //uint64_t seed = xxx;
            //rng::Seed<xoshio256ss> seed;

            //auto rng = xo::rng::xoshiro256ss(seed);

            scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.mult1"));
            //log && log("(A)", xtag("foo", foo));

            auto q0 = milliseconds(5);
            auto q1 = seconds(60);
            auto q2 = minutes(1);

            {
                auto r = q0 * q1;

                /* taking unit from LHS */
                REQUIRE(strcmp(r.unit_cstr(), "ms^2") == 0);

                log && log(xtag("q0", q0), xtag("q1", q1), xtag("q0*q1", r));

                REQUIRE(r.scale() == 300000);
            }

            {
                auto r = q1 * q2;

                /* taking unit from LHS */
                REQUIRE(strcmp(r.unit_cstr(), "s^2") == 0);

                log && log(xtag("q1", q1), xtag("q2", q2), xtag("q1*q2", r));

                REQUIRE(r.scale() == 3600);
            }

            {
                auto r = q2 * q1;

                /* taking unit from LHS */
                REQUIRE(strcmp(r.unit_cstr(), "min^2") == 0);

                log && log(xtag("q1", q1), xtag("q2", q2), xtag("q2*q1", r));

                REQUIRE(r.scale() == 1);
            }
        } /*TEST_CASE(mult1)*/

    } /*namespace ut*/
} /*namespace xo*/

/* end quantity.test.cpp */

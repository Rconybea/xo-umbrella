/* @file quantity.test.cpp */

#include "xo/observable/quantity.hpp"
#include "xo/reflect/Reflect.hpp"
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

    using xo::obs::unit_find_bpu_t;
    using xo::obs::unit_conversion_factor_t;
    using xo::obs::unit_cartesian_product_t;
    using xo::obs::unit_invert_t;
    using xo::obs::unit_abbrev_v;
    using xo::obs::same_dimension_v;
    using xo::obs::native_dim_id;

    using xo::obs::from_ratio;
    using xo::obs::stringliteral_from_ratio;
    using xo::obs::ratio2str_aux;
    using xo::obs::cstr_from_ratio;

    using xo::reflect::Reflect;

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

            static_assert(t.basis_power<native_dim_id::time> == 1);
            static_assert(t.basis_power<native_dim_id::mass> == 0);
        } /*TEST_CASE(quantity)*/

        TEST_CASE("add1", "[quantity]") {
            constexpr bool c_debug_flag = true;

            scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.add1"));

            quantity<units::second, int64_t> t1 = seconds(1);
            quantity<units::second, int64_t> t2 = seconds(2);

            auto sum = t1 + t2;

            log && log(xtag("sum.unit", sum.unit_cstr()));

            static_assert(std::same_as<decltype(sum)::unit_type, units::second>);
            static_assert(t1.basis_power<native_dim_id::time> == 1);
            static_assert(t2.basis_power<native_dim_id::time> == 1);

            REQUIRE(sum.scale() == 3);

        } /*TEST_CASE(add1)*/

        TEST_CASE("add2", "[quantity]") {
            constexpr bool c_debug_flag = true;

            scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.add2"));

            quantity<units::second, int64_t> t1 = seconds(1);
            auto m2 = minutes(2);

            static_assert(m2.basis_power<native_dim_id::time> == 1);

            log && log(xtag("m2.scale", m2.scale()), xtag("m2.unit", m2.unit_cstr()));

            log && log(xtag("m2_sec", m2.in_units_of<units::second, int64_t>()));

            quantity<units::second, int64_t> t2 = m2;

            log && log(xtag("t1.unit", t1.unit_cstr()), xtag("t2.unit", t2.unit_cstr()));

            auto sum = t1 + t2;

            static_assert(m2.basis_power<native_dim_id::time> == 1);

            log && log(xtag("sum.unit", sum.unit_cstr()));

            static_assert(std::same_as<decltype(sum)::unit_type, units::second>);

            REQUIRE(sum.scale() == 121);

        } /*TEST_CASE(add1)*/

        TEST_CASE("add3", "[quantity]") {
            quantity<units::second, int64_t> t1 = seconds(1);
            quantity<units::minute, int64_t> t2 = minutes(2);

            /* sum will take unit from lhs argument to + */
            auto sum = t1 + t2;

            static_assert(sum.basis_power<native_dim_id::time> == 1);
            static_assert(std::same_as<decltype(sum)::unit_type, units::second>);

            REQUIRE(sum.scale() == 121);
        } /*TEST_CASE(add3)*/

        TEST_CASE("add4", "[quantity]") {
            constexpr bool c_debug_flag = true;

            scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.add4"));

            using u_kgps = unit_cartesian_product_t<units::kilogram, unit_invert_t<units::second>>;
            using u_gpm = unit_cartesian_product_t<units::gram, unit_invert_t<units::minute>>;

            static_assert(std::same_as<unit_find_bpu_t<u_kgps, native_dim_id::mass>::power_type, std::ratio<1>>);
            static_assert(std::same_as<unit_find_bpu_t<u_kgps, native_dim_id::time>::power_type, std::ratio<-1>>);
            static_assert(std::same_as<unit_find_bpu_t<u_gpm, native_dim_id::mass>::power_type, std::ratio<1>>);
            static_assert(std::same_as<unit_find_bpu_t<u_gpm, native_dim_id::time>::power_type, std::ratio<-1>>);

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

            static_assert(q1.basis_power<native_dim_id::mass> == 1);
            static_assert(q1.basis_power<native_dim_id::time> == -1);
            static_assert(q2.basis_power<native_dim_id::mass> == 1);
            static_assert(q2.basis_power<native_dim_id::time> == -1);

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

                static_assert(r.basis_power<native_dim_id::time> == 2);

                /* taking unit from LHS */
                REQUIRE(strcmp(r.unit_cstr(), "ms^2") == 0);

                log && log(xtag("q0", q0), xtag("q1", q1), xtag("q0*q1", r));

                REQUIRE(r.scale() == 300000);
            }

            {
                auto r = q1 * q2;

                static_assert(r.basis_power<native_dim_id::time> == 2);

                /* taking unit from LHS */
                REQUIRE(strcmp(r.unit_cstr(), "s^2") == 0);

                log && log(xtag("q1", q1), xtag("q2", q2), xtag("q1*q2", r));

                REQUIRE(r.scale() == 3600);
            }

            {
                auto r = q2 * q1;

                static_assert(r.basis_power<native_dim_id::time> == 2);

                /* taking unit from LHS */
                REQUIRE(strcmp(r.unit_cstr(), "min^2") == 0);

                log && log(xtag("q1", q1), xtag("q2", q2), xtag("q2*q1", r));

                REQUIRE(r.scale() == 1);
            }

            {
                auto r = q2 * 60;

                static_assert(r.basis_power<native_dim_id::time> == 1);

                log && log(xtag("q2*60", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                static_assert(std::same_as<decltype(r)::repr_type, int>);

                /* preserve units of existing quantity */
                REQUIRE(strcmp(r.unit_cstr(), "min") == 0);
                REQUIRE(r.scale() == 60);
            }

            {
                auto r = q2 * 60U;

                static_assert(r.basis_power<native_dim_id::time> == 1);

                log && log(xtag("q2*60U", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                static_assert(std::same_as<decltype(r)::repr_type, uint32_t>);

                /* preserve units of existing quantity */
                REQUIRE(strcmp(r.unit_cstr(), "min") == 0);
                REQUIRE(r.scale() == 60U);
            }

            {
                log && log(xtag("q2*60.5", q2*60.5));

                auto r = (q2 * 60.5);

                static_assert(r.basis_power<native_dim_id::time> == 1);

                //log && log(xtag("r=q2*60.5", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                static_assert(std::same_as<decltype(r)::repr_type, double>);

                /* preserve units of existing quantity */
                REQUIRE(strcmp(r.unit_cstr(), "min") == 0);
                REQUIRE(r.scale() == 60.5);
            }

            {
                log && log(xtag("q2*60.5f", q2*60.5f));

                auto r = (q2 * 60.5f);

                static_assert(r.basis_power<native_dim_id::time> == 1);

                //log && log(xtag("r=q2*60.5f", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                static_assert(std::same_as<decltype(r)::repr_type, float>);

                /* preserve units of existing quantity */
                REQUIRE(strcmp(r.unit_cstr(), "min") == 0);
                REQUIRE(r.scale() == 60.5f);
            }

            {
                auto r = 60 * q2;

                log && log(xtag("60*q2", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                static_assert(r.basis_power<native_dim_id::time> == 1);
                static_assert(std::same_as<decltype(r)::repr_type, int>);

                /* preserve units of existing quantity */
                REQUIRE(strcmp(r.unit_cstr(), "min") == 0);
                REQUIRE(r.scale() == 60);
            }

            {
                log && log(xtag("60.5*q2", 60.5*q2));

                auto r = 60.5 * q2;

                //log && log(xtag("60.0*q2", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                static_assert(r.basis_power<native_dim_id::time> == 1);
                static_assert(std::same_as<decltype(r)::repr_type, double>);

                /* preserve units of existing quantity */
                REQUIRE(strcmp(r.unit_cstr(), "min") == 0);
                REQUIRE(r.scale() == 60.5);
            }

            {
                log && log(xtag("60.5f*q2", 60.5f*q2));

                auto r = 60.5f * q2;

                //log && log(xtag("60.0*q2", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                static_assert(r.basis_power<native_dim_id::time> == 1);
                static_assert(std::same_as<decltype(r)::repr_type, float>);

                /* preserve units of existing quantity */
                REQUIRE(strcmp(r.unit_cstr(), "min") == 0);
                REQUIRE(r.scale() == 60.5);
            }
        } /*TEST_CASE(mult1)*/

        TEST_CASE("div1", "[quantity]") {
            constexpr bool c_debug_flag = true;

            // can get bits from /dev/random by uncommenting the 2nd line below
            //uint64_t seed = xxx;
            //rng::Seed<xoshio256ss> seed;

            //auto rng = xo::rng::xoshiro256ss(seed);

            scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.div1"));
            //log && log("(A)", xtag("foo", foo));

            auto q0 = milliseconds(5);
            auto q1 = milliseconds(10);

            {
                /* repr_type adopts argument to milliseconds() */
                static_assert(std::same_as<decltype(q0)::repr_type, int>);
                static_assert(std::same_as<decltype(q1)::repr_type, int>);

                auto r = q0/q1;

                REQUIRE(r == 0);
                static_assert(std::same_as<decltype(r), int>);

                log && log(xtag("q0", q0), xtag("q1", q1), xtag("q0/q1", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));
            }

            auto q0p = milliseconds(5.0);

            {
                static_assert(std::same_as<decltype(q0p)::repr_type, double>);

                auto r = q0p/q1;
                static_assert(std::same_as<decltype(r), double>);

                REQUIRE(r == 0.5);

                log && log(xtag("q0p", q0p), xtag("q0p/q1", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));
            }

            auto r1 = 1.0 / q0;

            {
                log && log(xtag("r1", r1));
            }
        } /*TEST_CASE(div1)*/

        TEST_CASE("div2", "[quantity]") {
            constexpr bool c_debug_flag = true;

            // can get bits from /dev/random by uncommenting the 2nd line below
            //uint64_t seed = xxx;
            //rng::Seed<xoshio256ss> seed;

            //auto rng = xo::rng::xoshiro256ss(seed);

            scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.div2"));
            //log && log("(A)", xtag("foo", foo));

            auto q0 = milliseconds(5);
            auto q1 = milliseconds(20.0);

            {
                auto r = q0/q1;

                log && log(xtag("q0", q0), xtag("q1", q1), xtag("q0/q1", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                REQUIRE(r == 0.25);
            }

            {
                auto r = q1/q0;

                log && log(xtag("q0", q0), xtag("q1", q1), xtag("q1/q0", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                REQUIRE(r == 4.0);
            }

            {
                auto r = q0/(q1*q1);

                log && log(xtag("q0", q0), xtag("q1", q1), xtag("q0/(q1*q1)", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                static_assert(r.basis_power<native_dim_id::time> == -1);

                REQUIRE(r.scale() == 0.0125);
            }

            {
                auto r = (q0*q0)/q1;

                log && log(xtag("q0", q0), xtag("q1", q1), xtag("(q0*q0)/q1", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                static_assert(r.basis_power<native_dim_id::time> == 1);

                REQUIRE(r.scale() == 1.25);
            }

        } /*TEST_CASE(div2)*/

        TEST_CASE("div3", "[quantity]") {
            constexpr bool c_debug_flag = true;

            // can get bits from /dev/random by uncommenting the 2nd line below
            //uint64_t seed = xxx;
            //rng::Seed<xoshio256ss> seed;

            //auto rng = xo::rng::xoshiro256ss(seed);

            scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.div3"));
            //log && log("(A)", xtag("foo", foo));

            auto q0 = milliseconds(5);
            auto q1 = milliseconds(20.0);

            {
                auto r = q0/q1;

                log && log(xtag("q0", q0), xtag("q1", q1), xtag("q0/q1", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                REQUIRE(r == 0.25);
            }

            {
                auto r = q1/q0;

                log && log(xtag("q0", q0), xtag("q1", q1), xtag("q1/q0", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                REQUIRE(r == 4.0);
            }

            {
                auto r = q0/(q1*q1);

                log && log(xtag("q0", q0), xtag("q1", q1), xtag("q0/(q1*q1)", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                static_assert(r.basis_power<native_dim_id::time> == -1);

                REQUIRE(r.scale() == 0.0125);
            }

            {
                auto r = (q0*q0)/q1;

                log && log(xtag("q0", q0), xtag("q1", q1), xtag("(q0*q0)/q1", r));
                log && log(xtag("r.type", Reflect::require<decltype(r)>()->canonical_name()));

                static_assert(r.basis_power<native_dim_id::time> == 1);

                REQUIRE(r.scale() == 1.25);
            }

        } /*TEST_CASE(div3)*/


    } /*namespace ut*/
} /*namespace xo*/

/* end quantity.test.cpp */

/* @file unit.test.cpp */

#ifdef OBSOLETE
#include "xo/observable/unit.hpp"
#include "xo/indentlog/scope.hpp"
#include "xo/indentlog/print/tag.hpp"
#include <catch2/catch.hpp>

namespace xo {
    //using xo::obs::unit;
    namespace units = xo::obs::units;

    using xo::obs::unit_abbrev_v;
    using xo::obs::unit_cartesian_product_t;

    namespace ut {
        TEST_CASE("unit", "[unit]") {
            constexpr bool c_debug_flag = true;

            // can get bits from /dev/random by uncommenting the 2nd line below
            //uint64_t seed = xxx;
            //rng::Seed<xoshio256ss> seed;

            //auto rng = xo::rng::xoshiro256ss(seed);

            scope log(XO_DEBUG2(c_debug_flag, "TEST_CASE.unit"));
            //log && log("(A)", xtag("foo", foo));

            static_assert(xo::obs::from_ratio< double, std::ratio<1,10> >() == 0.1);
            static_assert(xo::obs::from_ratio< double, std::ratio<10,1> >() == 10.0);

#ifdef NOT_YET_22mar24
            REQUIRE(units::u_gram::abbrev() == "gm");
#endif

            static_assert(std::same_as<
                          units::u_nanosecond::scale_factor_to_unit<units::u_microsecond>,
                          std::ratio<1,1000> >);

            static_assert(std::same_as<
                          units::u_microsecond::scale_factor_to_unit<units::u_nanosecond>,
                          std::ratio<1000,1> >);

            static_assert(std::same_as<
                          units::u_minute::scale_factor_to_unit<units::u_minute>,
                          std::ratio<1,1> >);

            static_assert(std::same_as<
                          units::u_minute::scale_factor_to_unit<units::u_second>,
                          std::ratio<60,1> >);

            static_assert(std::same_as<
                          units::u_second::scale_factor_to_unit<units::u_minute>,
                          std::ratio<1,60> >);

            static_assert(units::u_minute::scale_factor_to<int, units::u_second>() == 60);
            static_assert(units::u_second::scale_factor_to<int, units::u_minute>() ==  0);

            //static_assert(units::u_sec::scale_factor_to<double, units::u_min>() == 1.0/60.0);

            log && log(xtag("u_microsecond", unit_abbrev_v<units::u_microsecond>.c_str()));
            log && log(xtag("u_second", unit_abbrev_v<units::u_second>.c_str()));
            log && log(xtag("u_minute", unit_abbrev_v<units::u_minute>.c_str()));

            log && log(xtag("u_gram.u_microsecond", unit_abbrev_v<unit_cartesian_product_t<units::u_gram, units::u_microsecond>>.c_str()));

        } /*TEST_CASE(unit)*/
    } /*namespace ut*/
} /*namespace xo*/

#endif

/* end unit.test.cpp */

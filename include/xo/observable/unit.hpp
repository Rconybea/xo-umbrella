/* @file unit.hpp */

#pragma once

#include "dimension.hpp"
#include "stringliteral.hpp"

namespace xo {
    namespace obs {
        // ----- unit ----

        /** @class unit
         *
         *  @brief compile-time representation for a unit quantity.
         *
         *  Scale.  a ratio type representing that multiple of dimension's 'native unit'
         *
         *  Require:
         *  - each Ps is a basis_power_unit type
         *  - expecting Scale to be a std::ratio type
         *  - arithmetic on Repr:
         *    - Repr / Repr
         *    - Repr + Repr
         *    - double * Repr
         *    - Repr / double
         *
         * Note: requiring c++20 here for non-type template parameters
         **/
        template <typename Dimension>
        struct unit {
        public:
            using scalefactor_type = typename Dimension::scalefactor_type;
            using dimension_type = typename Dimension::dim_type;
            using canon_type = typename Dimension::canon_type;

            /** Example:
             *    scale_factor_to_unit< units::u_sec, units::u_min > --> std::raio<1,60>
             **/
            template <typename Unit2>
            using scale_factor_to_unit = std::ratio_divide<unit::scale_type, typename Unit2::scale_type>::type;

            /** @brief Report scale factor TO Unit2.
             *
             *  Require: Unit2 has compatible dimension
             *
             **/
            template <typename Repr, typename Unit2>
            static constexpr Repr scale_factor_to() {
                static_assert(std::same_as< typename unit::dimension_type::canon_type, typename Unit2::dimension_type::canon_type >);

                return from_ratio< Repr, scale_factor_to_unit< Unit2 > >();

                //constexpr Repr num = unit::scale_type::num * Unit2::scale_type::den;
                //constexpr Repr den = unit::scale_type::den * Unit2::scale_type::num;
                //constexpr Repr gcd = std::gcd(num, den);
                //constexpr Repr num2 = num / gcd;
                //constexpr Repr den2 = den / gcd;
                //return num2 / den2;
            }
        };

        template <typename Unit1, typename Unit2>
        using unit_cartesian_product_t = unit<
            typename std::ratio_multiply<typename Unit1::scale_type, typename Unit2::scale_type>::type,
            dim_cartesian_product_t<typename Unit1::dimension_type, typename Unit2::dimension_type> >;

        /** NOTE: using struct wrapper to allow for specialization
         *
         *        For example to express work as "N.m" instead of "kg.m.s^-2"
         *
         *  Promise:
         *  - value is a stringliteral type
         *    - value.c_str() is [char const *]
         **/
        template <typename Unit>
        struct unit_assemble_abbrev {
            /* DEFAULT only works for native unit i.e. when Unit::Scale = 1 */
            static constexpr auto value = dim_abbrev_v<typename Unit::dimension_type>;
        };

        template <typename Unit>
        auto constexpr unit_abbrev_v = unit_assemble_abbrev<Unit>::value;

        namespace units {
            /* see init_observable.cpp for names */

            /* PROBLEM: scale isn't preserved by arithmetic,  so quantities won't print in-scale
             *
             *          Idea would be to prepare dimension conversion for addition
             */

            using u_gram        = unit<std::ratio<1>,            dimension<native_bpu<std::ratio<1>, native_dim_id::mass, std::ratio<1>>> >;
            using u_kilogram    = unit<std::ratio<1000>,         dimension<native_bpu<std::ratio<1>, native_dim_id::mass, std::ratio<1>>> >;

            using u_nanosecond  = unit<std::ratio<1,1000000000>, dimension<native_bpu<std::ratio<1>, native_dim_id::time, std::ratio<1>>> >;
            using u_microsecond = unit<std::ratio<1>,            dimension<native_bpu<std::ratio<1>, native_dim_id::time, std::ratio<1,1000000>>> >;
            using u_millisecond = unit<std::ratio<1,1000>,       dimension<native_bpu<std::ratio<1>, native_dim_id::time, std::ratio<1>>> >;
            using u_second      = unit<std::ratio<1>,            dimension<native_bpu<std::ratio<1>, native_dim_id::time, std::ratio<1>>> >;
            using u_minute      = unit<std::ratio<60>,           dimension<native_bpu<std::ratio<1>, native_dim_id::time, std::ratio<1>>> >;
            using u_hour        = unit<std::ratio<3600>,         dimension<native_bpu<std::ratio<1>, native_dim_id::time, std::ratio<1>>> >;
            using u_day         = unit<std::ratio<24*3600>,      dimension<native_bpu<std::ratio<1>, native_dim_id::time, std::ratio<1>>> >;

            using u_currency    = unit<std::ratio<1>,            dimension<native_bpu<std::ratio<1>, native_dim_id::currency, std::ratio<1>>> >;

            using u_price       = unit<std::ratio<1>,            dimension<native_bpu<std::ratio<1>, native_dim_id::price, std::ratio<1>>> >;
        }

#define UNIT_ABBREV(unit,text) template<> struct unit_assemble_abbrev< unit > { static constexpr auto value = stringliteral(text); };

        UNIT_ABBREV(units::u_gram,        "g");
        UNIT_ABBREV(units::u_kilogram,    "kg");

        UNIT_ABBREV(units::u_nanosecond,  "ns");
        UNIT_ABBREV(units::u_microsecond, "us");
        UNIT_ABBREV(units::u_millisecond, "ms");
        UNIT_ABBREV(units::u_minute,      "min");
        UNIT_ABBREV(units::u_hour,        "hr");
        UNIT_ABBREV(units::u_day,         "d");

        UNIT_ABBREV(units::u_currency,    "ccy");
        UNIT_ABBREV(units::u_price,       "px");

    } /*namespace obs*/
} /*namespace xo*/


/* end unit.hpp */

/* @file dim_util.hpp */

#pragma once

#include "stringliteral.hpp"

namespace xo {
    namespace obs {
        enum class native_dim_id {
            /** weight.  native unit = 1 gram **/
            mass,
            /** time. native unit = 1 second **/
            time,
            /** a currency amount. native unit depends on actual currency.
             *  For USD: one US dollar.
             *
             *  NOTE: unit system isn't suitable for multicurrency work:
             *        (1usd + 1eur) is well-defined,  but (1sec + 1m) is not.
             **/
            currency,
            /** a screen price. dimensionless **/
            price,
        };

        enum class native_unit_id {
            gram,
            second,
            currency,
            price
        };

        template <native_dim_id Dim>
        struct native_unit_for;

        template <>
        struct native_unit_for<native_dim_id::mass> { static constexpr auto value = native_unit_id::gram; };

        template <>
        struct native_unit_for<native_dim_id::time> { static constexpr auto value = native_unit_id::second; };

        template <>
        struct native_unit_for<native_dim_id::currency> { static constexpr auto value = native_unit_id::currency; };

        template <>
        struct native_unit_for<native_dim_id::price> { static constexpr auto value = native_unit_id::price; };

        template <native_dim_id Dim>
        constexpr auto native_unit_for_v = native_unit_for<Dim>::value;
    } /*namespace obs*/
} /*namespace xo*/


/* end dim_util.hpp */

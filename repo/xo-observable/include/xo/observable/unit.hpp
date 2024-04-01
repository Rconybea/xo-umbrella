/* @file dimension.hpp */

#pragma once

#include "dimension_impl.hpp"

namespace xo {
    namespace obs {
        // ----- wrap_unit -----

        template <typename Scalefactor, typename BpuList>
        struct wrap_unit {
            static_assert(ratio_concept<Scalefactor>);
            static_assert(bpu_list_concept<BpuList>);

            //using _norm_type = bpu_normalize<BpuList>;

            using scalefactor_type = Scalefactor;
            using dim_type = BpuList;

            /* canon_type just orders dimensions by increasing native_dim_id */
            using canon_type = canonical_impl<dim_type>::dim_type;

            static_assert(bpu_list_concept<canon_type>);
        };

        // ----- normalize_unit -----

        template <typename Unit>
        struct normalize_unit {
            static_assert(unit_concept<Unit>);

            using type = wrap_unit<std::ratio<1>, typename Unit::dim_type>;
        };

        template <typename Unit>
        using normalize_unit_t = normalize_unit<Unit>::type;

        // ----- dimensionless_v -----

        template <typename Unit>
        constexpr auto dimensionless_v = std::same_as<typename Unit::dim_type, void>;

        // ----- unit_abbrev_v -----

        /** @brief canonical stringliteral abbreviation for dimension D. **/
        template <typename U>
        constexpr auto unit_abbrev_v = di_assemble_abbrev<typename U::canon_type>::value;

        // ----- unit_invert -----

        template <typename U>
        struct unit_invert {
            static_assert(unit_concept<U>);

            using _sf = std::ratio_divide<std::ratio<1>, typename U::scalefactor_type>;
            using _di = di_invert< typename U::dim_type >::type;

            using type = wrap_unit< _sf, _di >;

            static_assert(unit_concept<type>);
        };

        template <typename U>
        using unit_invert_t = unit_invert<U>::type;

        // ----- unit_cartesian_product -----

        template <typename U1, typename U2>
        struct unit_cartesian_product {
            static_assert(unit_concept<U1>);
            static_assert(unit_concept<U2>);

            using _mult_type = di_cartesian_product<
                typename U1::dim_type,
                typename U2::dim_type>;

            using _mult_sf_type = _mult_type::outer_scalefactor_type;
            using _mult_di_type = _mult_type::bpu_list_type;

            using _sf1_type = typename std::ratio_multiply<
                typename U1::scalefactor_type,
                typename U2::scalefactor_type>::type;

            using _sf_type = typename std::ratio_multiply<_mult_sf_type, _sf1_type>::type;

            using type = wrap_unit< _sf_type, _mult_di_type >;

            static_assert(unit_concept<type>);
        };

        template <typename U1, typename U2>
        using unit_cartesian_product_t = unit_cartesian_product<U1, U2>::type;

        // ----- unit_divide -----

        template <typename U1, typename U2>
        struct unit_divide {
            static_assert(unit_concept<U1>);
            static_assert(unit_concept<U2>);

            using type = unit_cartesian_product_t<U1, unit_invert_t<U2>>;
        };

        template <typename U1, typename U2>
        using unit_divide_t = unit_divide<U1, U2>::type;

        // ----- same_dimension -----

        /* true iff U1 and U2 represent the same dimension,  i.e. differ only in dimensionless scaling factor
         *
         * To verify scale also, use same_unit<U1,U2> instead
         */
        template <typename U1, typename U2>
        struct same_dimension {
            static_assert(unit_concept<U1>);
            static_assert(unit_concept<U2>);

            using _unit_ratio_type = typename unit_cartesian_product<U1, unit_invert_t<U2>>::type;

            static_assert(std::same_as<typename _unit_ratio_type::dim_type, void>);

            static constexpr bool value = std::same_as<typename _unit_ratio_type::dim_type, void>;
        };

        template <typename U1, typename U2>
        constexpr bool same_dimension_v = same_dimension<U1, U2>::value;

        // ----- same_unit -----

        template <typename U1, typename U2>
        struct same_unit {
            static_assert(unit_concept<U1>);
            static_assert(unit_concept<U2>);

            using _unit_ratio_type = typename unit_cartesian_product<U1, unit_invert_t<U2>>::type;

            static_assert(std::same_as<typename _unit_ratio_type::scalefactor_type, std::ratio<1>>);
            static_assert(std::same_as<typename _unit_ratio_type::dim_type, void>);

            static constexpr bool value = (std::same_as<typename _unit_ratio_type::scalefactor_type, std::ratio<1>>
                                           && std::same_as<typename _unit_ratio_type::dim_type, void>);
        };

        template <typename U1, typename U2>
        constexpr bool same_unit_v = same_unit<U1, U2>::value;

        // ----- unit_conversion_factor -----

        template <typename U1, typename U2>
        struct unit_conversion_factor {
            static_assert(same_dimension_v<U1, U2>);

            using _unit_ratio_type = typename unit_cartesian_product<U1, unit_invert_t<U2>>::type;
            using type = _unit_ratio_type::scalefactor_type;
        };

        /** conversion factor from U1 to U2:
         *    U1 = x.U2
         *  with:
         *    x = R::num / R::den
         *    R = unit_conversion_factor_t<U1,U2>
         **/
        template <typename U1, typename U2>
        using unit_conversion_factor_t = unit_conversion_factor<U1, U2>::type;

        // ----- units -----

        namespace units {
            /* computing abbreviations:
             *  - unit_abbrev_v<Unit> :: stringliteral<...>
             *  - unit_abbrev_v<Unit>.c_str() :: const char *
             *
             * relies on
             *  - di_assemble_abbrev, di_assemble_abbrev_helper  [dimension_impl.hpp]
             *
             *  - bpu_assemble_abbrev<native_bpu>()              [native_bpu.hpp]
             *  - bpu_assemble_abbrev_helper< native_bpu::c_native_dim,
             *                                native_bpu::inner_scalefactor_type,
             *                                native_bpu::power_type >
             *     -> stringliteral
             *
             *    + can specialize for specific combinations
             *
             *  - native_unit_abbrev_helper< native_bpu::c_native_dim,
             *                               native_bpu::power_type >
             */

            // ----- weight -----

            using milligram  = wrap_unit< std::ratio<1>,
                                          bpu_list< native_bpu<native_dim_id::mass,
                                                               std::ratio<1, 1000>> > >;

            template <>
            struct scaled_native_unit_abbrev<native_dim_id::mass, std::ratio<1, 1000>> {
                static constexpr auto value = stringliteral("mg");
            };

            using gram       = wrap_unit< std::ratio<1>,
                                          bpu_list< native_bpu<native_dim_id::mass,
                                                               std::ratio<1>> > >;
            using kilogram   = wrap_unit< std::ratio<1>,
                                          bpu_list< native_bpu<native_dim_id::mass,
                                                               std::ratio<1000>> > >;

            template <>
            struct scaled_native_unit_abbrev<native_dim_id::mass, std::ratio<1000>> {
                static constexpr auto value = stringliteral("kg");
            };

            // ----- distance -----

            using millimeter = wrap_unit< std::ratio<1>,
                                          bpu_list< native_bpu<native_dim_id::distance,
                                                               std::ratio<1,1000>> > >;
            template <>
            struct scaled_native_unit_abbrev<native_dim_id::distance, std::ratio<1,1000>> {
                static constexpr auto value = stringliteral("mm");
            };

            using meter      = wrap_unit< std::ratio<1>,
                                          bpu_list< native_bpu<native_dim_id::time,
                                                               std::ratio<1>> > >;

            using kilometer  = wrap_unit< std::ratio<1>,
                                          bpu_list< native_bpu<native_dim_id::time,
                                                               std::ratio<1000>> > >;
            template <>
            struct scaled_native_unit_abbrev<native_dim_id::distance, std::ratio<1000>> {
                static constexpr auto value = stringliteral("km");
            };

            // ----- time -----

            using nanosecond = wrap_unit< std::ratio<1>,
                                          bpu_list< native_bpu<native_dim_id::time,
                                                               std::ratio<1, 1000000000>> > >;

            template <>
            struct scaled_native_unit_abbrev<native_dim_id::time, std::ratio<1,1000000000>> {
                static constexpr auto value = stringliteral("ns");
            };

            using microsecond = wrap_unit< std::ratio<1>,
                                           bpu_list< native_bpu<native_dim_id::time,
                                                                std::ratio<1, 1000000>> > >;

            template <>
            struct scaled_native_unit_abbrev<native_dim_id::time, std::ratio<1,1000000>> {
                static constexpr auto value = stringliteral("us");
            };

            using millisecond = wrap_unit< std::ratio<1>, bpu_list< native_bpu<native_dim_id::time,
                                                                               std::ratio<1,1000>> > >;

            template <>
            struct scaled_native_unit_abbrev<native_dim_id::time, std::ratio<1,1000>> {
                static constexpr auto value = stringliteral("ms");
            };

            using second     = wrap_unit< std::ratio<1>, bpu_list< native_bpu<native_dim_id::time,
                                                                              std::ratio<1>> > >;
            using minute     = wrap_unit< std::ratio<1>, bpu_list< native_bpu<native_dim_id::time,
                                                                              std::ratio<60>> > >;
            template <>
            struct scaled_native_unit_abbrev<native_dim_id::time, std::ratio<60>> {
                static constexpr auto value = stringliteral("min");
            };

            using hour       = wrap_unit< std::ratio<1>,
                                          bpu_list< native_bpu<native_dim_id::time,
                                                               std::ratio<3600>> > >;

            template <>
            struct scaled_native_unit_abbrev<native_dim_id::time, std::ratio<3600>> {
                static constexpr auto value = stringliteral("hr");
            };

            using day       = wrap_unit< std::ratio<1>,
                                         bpu_list< native_bpu<native_dim_id::time,
                                                              std::ratio<24*3600>> > >;

            template <>
            struct scaled_native_unit_abbrev<native_dim_id::time, std::ratio<24*3600>> {
                static constexpr auto value = stringliteral("dy");
            };

            /* this would be volatility in per-root-second units;  probably want something else */
            using volatility = wrap_unit< std::ratio<1>,
                                          bpu_list< native_bpu<native_dim_id::time,
                                                               std::ratio<1>,
                                                               std::ratio<1,2>> > >;

            using currency   = wrap_unit< std::ratio<1>,
                                          bpu_list< native_bpu<native_dim_id::currency,
                                                               std::ratio<1>> > >;
            using price      = wrap_unit< std::ratio<1>,
                                          bpu_list< native_bpu<native_dim_id::price,
                                                               std::ratio<1>> > >;
        }

    } /*namespace obs*/
} /*namespace xo*/


/* end dimension.hpp */

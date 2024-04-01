/* @file native_bpu.hpp */

#pragma once

#include "native_bpu_concept.hpp"
#include "dim_util.hpp"
#include "ratio_util.hpp"
#include <ratio>
//#include <ratio>

namespace xo {
    namespace obs {
        /* TODO:
         * - native_bpu -> bpu
         */

        /** @class native_unit
         *
         *  @brief A dimensionless multiple with natively-specified (i.e. at compile-time) dimension
         **/
        template <dim DimId,
                  native_unit_id NativeUnitId = native_unit_for_v<DimId>,
                  typename InnerScale = std::ratio<1>>
        struct basis_unit {
            static_assert(ratio_concept<InnerScale>);

            static constexpr dim c_native_dim = DimId;
            static constexpr basis_unit c_native_unit = NativeUnitId;

            using inner_scalefactor_type = InnerScale;
        };

        /** Using struct wrapper so we can partially specialize
         *  Specializations in [dimension.hpp], see also
         **/
        template <dim dim_id>
        struct native_unit_abbrev_helper;


        template <>
        struct native_unit_abbrev_helper<dim::mass> {
            static constexpr auto value = stringliteral("g");
        };

        template <>
        struct native_unit_abbrev_helper<dim::distance> {
            static constexpr auto value = stringliteral("m");
        };

        template <>
        struct native_unit_abbrev_helper<dim::time> {
            static constexpr auto value = stringliteral("s");
        };

        template<>
        struct native_unit_abbrev_helper<dim::currency> {
            static constexpr auto value = stringliteral("ccy");
        };

        template<>
        struct native_unit_abbrev_helper<dim::price> {
            static constexpr auto value = stringliteral("px");
        };

        template<dim DimId>
        constexpr auto native_unit_abbrev_v = native_unit_abbrev_helper<DimId>::value;

        // ----- scaled_native_unit_abbrev_helper -----

        namespace units {
            /* Require: InnerScale is ratio type; InnerScale >= 1 */
            template <dim DimId, typename InnerScale>
            struct scaled_native_unit_abbrev;

            template <dim DimId>
            struct scaled_native_unit_abbrev<DimId, std::ratio<1>> {
                static constexpr auto value = native_unit_abbrev_v<DimId>;
            };

            template <dim DimId, typename InnerScale>
            struct scaled_native_unit_abbrev {
                /* e.g. unit of '10000 grams' will have abbrev '1000g' in absence
                 *      of a specialization for scaled_native_unit_abbrev
                 */
                static constexpr auto value = stringliteral_concat(stringliteral_from_ratio<InnerScale>().value_,
                                                                   native_unit_abbrev_helper<DimId>::value.value_);
            };

            template <dim DimId, typename InnerScale>
            constexpr auto scaled_native_unit_abbrev_v = scaled_native_unit_abbrev<DimId, InnerScale>::value;
        }

        // ----- native_bpu -----

        /** @class native_bpu

            @brief represent product of a compile-time scale-factor with a rational power of a native unit

            Example:
            native_bpu<universal::time, ratio<1>, ratio<-1,2>> represents unit of 1/sqrt(t)
         **/
        template<
            dim DimId,
            typename InnerScale,
            typename Power = std::ratio<1> >
        struct native_bpu : basis_unit<DimId, native_unit_for_v<DimId>, InnerScale> {
            static_assert(ratio_concept<Power>);

            /* native_unit provides
             * - inner_scalefactor_type
             * - c_native_dim :: dim
             * - c_native_unit :: native_unit
             */

            using power_type = Power;

            static const int c_num = Power::num;
            static const int c_den = Power::den;
        };

        /** @class bpu_assemble_abbrev
         *
         *  @brief generate abbreviation literal.
         *
         *  Abbreviation literal ignores outer scale factor;
         *  (outer scale factor should be multiplied by run-time scale when printing a quantity)
         *
         *  Separate template from native_bpu so that abbrev can independently be specialized
         **/
        template < dim dim_id,
                   typename InnerScale,
                   typename Power = std::ratio<1> >
        constexpr auto bpu_assemble_abbrev_helper() {
            static_assert(ratio_concept<Power>);

            return stringliteral_concat(units::scaled_native_unit_abbrev_v<dim_id, InnerScale>.value_,
                                        stringliteral_from_exponent<Power>().value_);
        };

        /** Expect:
         *  - BPU is a native_bpu type:
         *    - BPU::inner_scalefactor_type = std::ratio<..>
         *    - BPU::c_native_dim :: dim
         *    - BPU::power_type = std::ratio<..>
         *    - BPU::c_num :: int
         *    - BPU::c_den :: int
         **/
        template < typename BPU >
        constexpr auto bpu_assemble_abbrev() {
            static_assert(native_bpu_concept<BPU>);

            return bpu_assemble_abbrev_helper< BPU::c_native_dim,
                                               typename BPU::inner_scalefactor_type,
                                               typename BPU::power_type >();
        };

        // ----- bpu_rescale -----

        /** We have B satisfying native_bpu_concept:
         *  B represents a basis-power-unit
         *          p
         *     (b.u)
         *
         *  with
         *    b = B::inner_scalefactor_type, e.g. 60 for a 1-minute unit
         *    u = B::dim, e.g. 1second for time
         *    p = B::power_type
         *
         *  We want to construct something with similar form:
         *
         *             p
         *    a'.(b'.u)
         *
         *  representing the same dimensioned unit,
         *  i.e.
         *          p            p'
         *     (b.u)  = a'.(b'.u)
         *
         *  with NewInnerScale -> b'
         *
         *          p         p        p            p
         *     (b.u)  = (b/b') . (b'.u)  = a'.(b'.u)
         *
         *                 p
         * with a' = (b/b')
         *
         * For example: if we have B(b=60,u=time,p=2),  NewInnerScale=1:
         * then we want a'=3600, B'(b=1,u=time,p=2)
         *
         * Result represented with
         *   bpu_rescale<B,NewInnerScale>::outer_scalefactor_type -> 'a
         *   bpu_rescale<B,NewInnerScale>::native_bpu_type -> B'
         *
         **/
        template <typename B,
                  typename NewInnerScale>
        struct bpu_rescale {
            static_assert(native_bpu_concept<B>);
            static_assert(ratio_concept<NewInnerScale>);

            /* TODO:
             * - native_unit::c_scale -> std::ratio,  call it c_inner_scalefactor
             * - ++ native_bpu::c_outer_scalefactor,  will be a std::ratio
             */

            /* b/b' */
            using _t1_type = std::ratio_divide
                < typename B::inner_scalefactor_type, NewInnerScale >;

            /** require p must be integral **/
            static_assert(B::power_type::den == 1);

            /**            p
             *  a' = (b/b')
             **/
            using outer_scalefactor_type = ratio_power_t< _t1_type, B::power_type::num >;

            /**
             *          p
             *    (b'.u)
             **/
            using native_bpu_type = native_bpu < B::c_native_dim,
                                                 NewInnerScale,
                                                 typename B::power_type >;
        };

        // ----- bpu_invert -----

        /** invert a native bpu:  create type for space 1/B **/
        template <typename B>
        struct bpu_invert {
            using type = native_bpu <
                B::c_native_dim,
                typename B::inner_scalefactor_type,
                std::ratio_multiply<std::ratio<-1>, typename B::power_type>
                >;
        };

        // ----- bpu_inner_multiply -----

        /** Suppose we have two native_bpu's that scale the same dim:
         *
         *                            p1
         *    B1 (b1, u, p1)  = (b1.u)
         *
         *                            p2
         *    B2 (b2, u, p2)  = (b2.u)
         *
         *  we want a representation in similar form:
         *
         *                                    p'
         *    a' . B' (b', u, p')  = a'.(b'.u)
         *
         *  for the product (B1 x B2),  i.e.
         *
         *             p'         p1      p2
         *    a'.(b'.u)   = (b1.u)  (b2.u)
         *
         *  We can use bpu_rescale to rewrite B2 in the form
         *
         *                   p2
         *    B2' = a'.(b1.u)
         *
         *  so we have
         *
         *                      p1         p2
         *    (B1 x B2) = (b1.u)  a'.(b1.u)
         *
         *                         p1+p2
         *              = a'.(b1.u)
         **/
        template < typename B1, typename B2 >
        struct bpu_inner_multiply {
            // TODO: rename -> bpu_cartesian_product ?

            static_assert(native_bpu_concept<B1>);
            static_assert(native_bpu_concept<B2>);
            static_assert(B1::c_native_dim == B2::c_native_dim);

            /* a'.B2' = a'.(b1.u)^p2 */
            using _b2p_rescaled_type = bpu_rescale<B2,
                                                   typename B1::inner_scalefactor_type>;
            /* (b1.u)^p2 */
            using _b2p_sf_bpu_type = _b2p_rescaled_type::native_bpu_type;

            /* p1+p2 */
            using _p_type = std::ratio_add<
                typename B1::power_type,
                typename B2::power_type
                >;

            /* a' */
            using outer_scalefactor_type = _b2p_rescaled_type::outer_scalefactor_type;

            /* (b1.u)^(p1+p2) */
            using native_bpu_type = native_bpu <
                B1::c_native_dim,
                typename B1::inner_scalefactor_type,
                _p_type /*Power*/ >;
        };

    } /*namespace obs*/
} /*namespace xo*/

/* end native_bpu.hpp */

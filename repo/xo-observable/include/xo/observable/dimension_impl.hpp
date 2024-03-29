/* @file dimension_impl.hpp */

#pragma once

#include "dimension_concept.hpp"
#include "native_bpu.hpp"
#include "dim_util.hpp"
#include "ratio_util.hpp"
#include <json/value.h>
#include <ratio>
#include <algorithm>
#include <numeric>
#include <string_view>

namespace xo {
    namespace obs {
        // ----- lookup_bpu -----

        /**
         *  Select from dimension_impl by known index value
         *
         *  Example:
         *    using t1 = basic_power_unit<dim::currency, std::ratio<1,1>>;
         *    using t2 = basic_power_unit<dim::time,     std::ratio<-1,2>>;
         *    using dim = dimension_impl<t1,t2>
         *
         * then
         *    lookup_bpu<dim,0> --> t1
         *    lookup_bpu<dim,1> --> t2
         **/
        template <typename Dim, int Index>
        struct lookup_bpu  {
            using power_unit_type = lookup_bpu<typename Dim::rest_type, Index-1>::power_unit_type;
        };

        template <typename Dim>
        struct lookup_bpu<Dim, 0> {
            using power_unit_type = Dim::front_type;
        };

        // ----------------------------------------------------------------

        template <typename P, typename D = void>
        struct bpu_list;

        // ----------------------------------------------------------------

        template <typename Front,
                  typename Rest>
        constexpr bool FrontHasZeroPower = std::ratio_equal_v< typename Front::power_type, std::ratio<0,1> >;

        template <typename Front,
                  typename Rest,
                  bool FHZP = FrontHasZeroPower<Front, Rest>>
        struct bpu_smart_cons;

        template <typename Front,
                  typename Rest>
        struct bpu_smart_cons<Front, Rest, /*FrontHasZeroPower*/ true> {
            using type = Rest;
        };

        template <typename Front,
                  typename Rest>
        struct bpu_smart_cons<Front, Rest, /*FrontHasZeroPower*/ false> {
            using type = bpu_list<Front, Rest>;
        };

        template <typename Front, typename Rest>
        using bpu_smart_cons_t = bpu_smart_cons<Front, Rest>::type;

        // ----------------------------------------------------------------

        /** @class bwp

            @brief represent (compile-time) result of search in a bpu_list<> type

            short for (basis-with-native-power-unit)
         **/
        template <int index_arg, native_dim_id basis_arg>
        struct bwp {
            static constexpr int c_index = index_arg;
            static constexpr native_dim_id c_basis = basis_arg;
        };

        template <typename T>
        using bwp_incr_pos_type = bwp<T::c_index + 1, T::c_basis>;

        // ----- lo_basis_with_pos_type -----

        template < typename BasisWithPos1, typename BasisWithPos2>
        using lo_basis_with_pos_type = std::conditional_t<(BasisWithPos1::c_basis < BasisWithPos2::c_basis),
                                                          BasisWithPos1, BasisWithPos2>;

        // ----- native_lo_bwp_of -----

        /* helper for canonically-ordering native dimension power-units */
        template <typename Dim>
        struct native_lo_bwp_of {
            using _bwp_front = bwp<0, Dim::front_type::c_native_dim>;
            using _pu_rest  = native_lo_bwp_of<typename Dim::rest_type>;
            using _bwp_rest = typename _pu_rest::bwp_type;

            using bwp_type = lo_basis_with_pos_type<_bwp_front,
                                                    bwp_incr_pos_type<_bwp_rest>>;
        };

        template <typename P0>
        struct native_lo_bwp_of<bpu_list<P0>> {
            using bwp_type = bwp<0, P0::c_native_dim>;
        };

        // ----- without_elt -----

        template <typename Dim, int Index>
        struct without_elt {
            using _without_rest_type = typename without_elt<typename Dim::rest_type, Index - 1>::dim_type;

            using dim_type = bpu_list< typename Dim::front_type, _without_rest_type >;
        };

        template <typename Dim>
        struct without_elt<Dim, 0> {
            using dim_type = typename Dim::rest_type;
        };

        // ----- bpu_list -----

        /** Represents the cartesian product of a list of 'native basis power units';
         *  represents something with dimensions
         *
         *  Expect:
         *  - P isa native_bpu type
         *  - D satisfies bpu_list_concept
         **/
        template <typename P, typename D>
        struct bpu_list {
            static_assert(native_bpu_concept<P>);
            static_assert(bpu_list_concept<D>);

            /** For example:
             *    using b1 = basis_power_unit<native_dim_id::currency, std::ratio<1, 1>>;
             *    using b2 = basis_power_unit<native_dim_id::time, std::ratio<-1, 2>>;
             *    using foo = dimension_impl<b1,dimension_impl<b2>>;
             *
             *  then
             *    foo::lookup_bpu<0> -> b1
             *    foo::lookup_bpu<1> -> b2
             *    foo::lookup_bpu<2> -> not defined
             **/
            using front_type = P;
            using rest_type = D;

            static constexpr std::uint32_t n_dimension = rest_type::n_dimension + 1;
        };

        /** @class dimension

            @brief represent a composite dimension
        **/
        template <typename P0>
        struct bpu_list<P0, void> {
            static_assert(native_bpu_concept<P0>);
            static_assert(bpu_list_concept<void>);

            using front_type = P0;
            using rest_type = void;

            /** For example:
             *    using b1 = basis_power_unit<native_dim_id::time, std::ratio<-1, 2>>;
             *    using foo = dimension_impl<b1>;
             *  then
             *    foo::lookup_bpu<0> --> b1
             *    foo::lookup_bpu<1> --> not defined
             **/

            /** number of dimensions represented by this struct **/
            static constexpr std::uint32_t n_dimension = 1;
        };

#ifdef OBSOLETE
        // ----- bpu_normalize -----

        template <typename BpuList>
        struct bpu_normalize;

        template <typename Front, typename Rest>
        struct bpu_normalize_helper;

        template <typename Front>
        struct bpu_normalize_helper<Front, void>
        {
            static_assert(native_bpu_concept<Front>);

            using _norm_front_type = native_bpu<Front::c_native_dim,
                                                typename Front::inner_scalefactor_type,
                                                typename Front::power_type>;

            //using scalefactor_type = typename Front::outer_scalefactor_type;
            using norm_type = bpu_list<_norm_front_type, void>;
        };

        template <typename Front, typename Rest>
        struct bpu_normalize_helper
        {
            static_assert(native_bpu_concept<Front>);
            //static_assert(bpulist_concept<Rest>);

            using _norm_front_type = native_bpu<
                Front::c_native_dim,
                typename Front::inner_scalefactor_type,
                typename Front::power_type>;

            using _norm_rest_type = bpu_normalize<Rest>;

            //using scalefactor_type = std::ratio_multiply<
            //    typename Front::outer_scalefactor_type,
            //    typename _norm_rest_type::scalefactor_type>;

            using norm_type = bpu_list<
                _norm_front_type,
                typename _norm_rest_type::norm_type
                >;
        };

        template <>
        struct bpu_normalize<void> {
            using norm_type = void;
            //using scalefactor_type = std::ratio<1>;
        };

        template <typename BpuList>
        struct bpu_normalize {
            using _helper_type = bpu_normalize_helper<
                typename BpuList::front_type,
                typename BpuList::rest_type >;

            using norm_type = typename _helper_type::norm_type;
            //using scalefactor_type = typename _helper_type::scalefactor_type;
        };
#endif

        // ----- bpu_cartesian_product -----

        /** Require:
         *  - B isa native_bpu type
         *  - DI_Front is a native_bpu type
         *  - DI_Rest is a dimension_impl type
         *
         *  Promise:
         *  - type isa dimension_impl type
         **/
        template < typename B,
                   typename DI_Front,
                   typename DI_Rest,
                   bool MatchesFront = (B::c_native_dim == DI_Front::c_native_dim) >
        struct bpu_cartesian_product_helper;

        /** require:
         * - B isa native_bpu type
         * - DI isa (bpu_list | void) type
         **/
        template < typename B, typename DI >
        struct bpu_cartesian_product {
            static_assert(native_bpu_concept<B>);
            static_assert(bpu_list_concept<DI>);

            using _tmp = bpu_cartesian_product_helper<B,
                                                      typename DI::front_type,
                                                      typename DI::rest_type>;

            using outer_scalefactor_type = typename _tmp::outer_scalefactor_type;
            using bpu_list_type = typename _tmp::bpu_list_type;

            static_assert(ratio_concept<outer_scalefactor_type>);
            static_assert(bpu_list_concept<bpu_list_type>);
        };

        /** Reminder: void represents the 'no dimension' of a dimensionless quantity **/
        template < typename B >
        struct bpu_cartesian_product<B, void> {
            using outer_scalefactor_type = std::ratio<1>;
            using bpu_list_type = bpu_list<B, void>;

            static_assert(ratio_concept<outer_scalefactor_type>);
            static_assert(bpu_list_concept<bpu_list_type>);
        };

        /* specialize for matching front */
        template <typename B, typename DI_Front, typename DI_Rest>
        struct bpu_cartesian_product_helper<B, DI_Front, DI_Rest, /*MatchesFront*/ true> {
            static_assert(native_bpu_concept<B>);
            static_assert(native_bpu_concept<DI_Front>);
            static_assert(bpu_list_concept<DI_Rest>);

            /* _mult_type may have zero exponent (power_type);
             *  in that case bpu_smart_cons will collapse to DI_Rest
             */
            using _front_mult_type = bpu_inner_multiply<B, DI_Front>;

            using _front_type = typename _front_mult_type::native_bpu_type;
            using _rest_type = DI_Rest;

            using outer_scalefactor_type = typename _front_mult_type::outer_scalefactor_type;
            using bpu_list_type = bpu_smart_cons_t<_front_type, DI_Rest>;

            static_assert(ratio_concept<outer_scalefactor_type>);
            static_assert(bpu_list_concept<bpu_list_type>);
        };

        /* specialize for not-matching-front */
        template <typename B, typename DI_Front, typename DI_Rest>
        struct bpu_cartesian_product_helper<B, DI_Front, DI_Rest, /*MatchesFront*/ false> {
            static_assert(native_bpu_concept<B>);
            static_assert(native_bpu_concept<DI_Front>);
            static_assert(bpu_list_concept<DI_Rest>);

            using _rest_mult_type = bpu_cartesian_product< B, DI_Rest >;

            using _front_type = DI_Front;
            using _rest_type = typename _rest_mult_type::bpu_list_type;

            using outer_scalefactor_type = typename _rest_mult_type::outer_scalefactor_type;
            using bpu_list_type = bpu_list<DI_Front, _rest_type>;

            static_assert(ratio_concept<outer_scalefactor_type>);
            static_assert(bpu_list_concept<bpu_list_type>);
        };

#ifdef OBSOLETE
        template < typename B, typename DI >
        using bpu_cartesian_product_t = bpu_cartesian_product<B, DI>::type;
#endif

        // ----- di_cartesian_product -----

        template < typename D1, typename D2 > struct di_cartesian_product;

        // ----- bpu_cartesian_product1 -----

        template < typename B1, typename R1, typename D2 >
        struct di_cartesian_product1 {
            static_assert(native_bpu_concept<B1>);
            static_assert(bpu_list_concept<R1>);
            static_assert(bpu_list_concept<D2>);

            using _tmp1_mult_type = bpu_cartesian_product<B1, D2>;
            using _tmp1_scalefactor_type = _tmp1_mult_type::outer_scalefactor_type;
            using _tmp1_bpu_list_type = _tmp1_mult_type::bpu_list_type;

            using _tmp2_mult_type = di_cartesian_product<R1, _tmp1_bpu_list_type>::type;
            using _tmp2_scalefactor_type = _tmp2_mult_type::outer_scalefactor_type;
            using _tmp2_bpu_list_type = _tmp2_mult_type::bpu_list_type;

            using outer_scalefactor_type = std::ratio_multiply<
                _tmp1_scalefactor_type,
                _tmp2_scalefactor_type>;

            using bpu_list_type = _tmp2_bpu_list_type;

            static_assert(ratio_concept<outer_scalefactor_type>);
            static_assert(bpu_list_concept<bpu_list_type>);
        };

        template < typename B1, typename D2 >
        struct di_cartesian_product1<B1, void, D2> {
            static_assert(native_bpu_concept<B1>);
            static_assert(bpu_list_concept<D2>);

            using _tmp_mult_type = bpu_cartesian_product<B1, D2>;

            using outer_scalefactor_type = _tmp_mult_type::outer_scalefactor_type;
            using bpu_list_type = _tmp_mult_type::bpu_list_type;
        };

        // ----- di_invert -----

        template <typename BpuList>
        struct di_invert;

        template <>
        struct di_invert<void> {
            using type = void;
        };

        template <typename BpuList>
        struct di_invert {
            using type = bpu_list<
                typename bpu_invert<typename BpuList::front_type>::type,
                typename di_invert<typename BpuList::rest_type>::type
                >;
        };

        // ----- di_cartesian_product -----

        template < typename D1, typename D2 >
        struct di_cartesian_product {
            static_assert(bpu_list_concept<D1>);
            static_assert(bpu_list_concept<D2>);

            using _mult_type = di_cartesian_product1<
                typename D1::front_type,
                typename D1::rest_type,
                D2>;

            using outer_scalefactor_type = _mult_type::outer_scalefactor_type;
            using bpu_list_type = _mult_type::bpu_list_type;

            static_assert(ratio_concept<outer_scalefactor_type>);
            static_assert(bpu_list_concept<bpu_list_type>);
        };

        template < typename D2 >
        struct di_cartesian_product< void, D2 > {
            static_assert(bpu_list_concept<D2>);

            using outer_scalefactor_type = std::ratio<1>;
            using bpu_list_type = D2;
        };

        template <typename D1 >
        struct di_cartesian_product< D1, void > {
            static_assert(bpu_list_concept<D1>);

            using outer_scalefactor_type = std::ratio<1>;
            using bpu_list_type = D1;
        };

        // ----- di_assemble_abbrev -----

        /* reminder: can't partially specialize a template function -> need struct wrapper */
        template < typename DI >
        struct di_assemble_abbrev;

        /** Expect:
         *  - P isa native_bpu type
         *    - P::power_type = std::ratio<..>
         *    - P::c_native_dim  :: native_dim_id
         *    - P::c_num :: int
         *    - P::c_den :: int
         *  - D isa dimension_impl type
         *    - D::front_type = native_bpu<..>
         *    - D::rest_type = dimension_impl<..>
         *    - D::n_dimension :: int
         **/
        template <typename P, typename D>
        struct di_assemble_abbrev_helper {
            static_assert(native_bpu_concept<P>);
            static_assert(bpu_list_concept<D>);

            static constexpr auto _prefix = bpu_assemble_abbrev<P>();
            static constexpr auto _suffix = di_assemble_abbrev<D>::value;

            static constexpr auto value = stringliteral_concat(_prefix.value_,
                                                               ".",
                                                               _suffix.value_);
        };

        template <typename P>
        struct di_assemble_abbrev_helper<P, void> {
            static constexpr auto value = bpu_assemble_abbrev<P>();
        };

        template < typename DI >
        struct di_assemble_abbrev {
            static_assert(bpu_list_concept<DI>);

            using _helper_type = di_assemble_abbrev_helper <typename DI::front_type, typename DI::rest_type>;

            static constexpr auto value = _helper_type::value;
        };

        template <>
        struct di_assemble_abbrev<void> {
            static constexpr auto value = stringliteral("");
        };

        // ----- canonical_impl -----

        template <typename D>
        struct canonical_impl {
            /*
             * bwp_front::c_index
             * bwp_front::c_native_dim
             */
            using _bwp_front = native_lo_bwp_of<D>::bwp_type;

            using _front_type = typename lookup_bpu<D, _bwp_front::c_index>::power_unit_type;
            using _rest0_type = typename without_elt<D, _bwp_front::c_index>::dim_type;
            using _rest_type = canonical_impl<_rest0_type>::dim_type;

            using dim_type = bpu_list<_front_type, _rest_type>;
        };

        /** compute canonical renumbering of a dimension
         **/
        template <>
        struct canonical_impl<void> {
            using dim_type = void;
        };

        template<typename D>
        using canonical_t = canonical_impl<D>::dim_type;

    } /*namespace obs*/
} /*namespace xo*/

/* end dimension_impl.hpp */

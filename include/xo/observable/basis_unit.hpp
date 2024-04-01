/** @file basis_unit.hpp **/

#include "dim_util.hpp"
#include "ratio_util.hpp"

namespace xo {
    namespace obs {
        /** @class basis_unit
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
    } /*namespace obs*/
} /*namespace xo*/

/** end basis_unit.hpp **/

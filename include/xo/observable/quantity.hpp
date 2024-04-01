/* @file quantity.hpp */

#pragma once

#include "quantity_concept.hpp"
#include "unit.hpp"

namespace xo {
    namespace obs {
        // ----- quantity -----

        /** @class quantity
         *
         *  @brief represets a scalar quantity;  enforces dimensional consistency at compile time
         *
         *  Repr representation.
         *  Unit unit
         *  Assert use to specify required unit dimension
         *
         *  Require:
         *  - Repr copyable, assignable
         *  - Repr = 0
         *  - Repr = 1
         *  - Repr + Repr -> Repr
         *  - Repr - Repr -> Repr
         *  - Repr * Repr -> Repr
         *  - Repr / Repr -> Repr
         **/
        template <typename Unit, typename Repr, typename RequiredDimensionType = Unit>
        class quantity {
        public:
            using unit_type = Unit;
            using repr_type = Repr;

            /* non-unity compile-time scale factors can arise during unit conversion;
             * for example see method quantity::in_units_of()
             */
            static_assert(std::same_as< typename Unit::scalefactor_type, std::ratio<1> >);
            static_assert(std::same_as< typename Unit::canon_type, typename Unit::canon_type >);

        public:
            constexpr quantity() = default;
            constexpr quantity(quantity const & x) = default;
            constexpr quantity(quantity && x) = default;

            /** @brief get scale value (relative to unit) (@ref scale_) **/
            constexpr Repr scale() const { return scale_; }
            /** @brief abbreviation for this quantity's units **/
            static constexpr char const * unit_cstr() { return unit_abbrev_v<unit_type>.c_str(); }
            /** @brief return unit quantity -- amount with this Unit that has representation = 1 **/
            static constexpr quantity unit_quantity() { return quantity(1); }
            /** @brief promote representation to quantity.  Same as multiplying by Unit **/
            static constexpr quantity promote(Repr x) { return quantity(x); }

            template <typename Quantity2>
            auto multiply(Quantity2 y) const {
                // static_assert(quantity_concept<Quantity2>);

                /* unit: may have non-unit scalefactor_type */
                using unit_type = unit_cartesian_product_t<Unit, typename Quantity2::unit_type>;
                using norm_unit_type = normalize_unit_t<unit_type>;

                using repr_type = std::common_type_t<repr_type, typename Quantity2::repr_type>;

                repr_type r_scale = ((scale() * y.scale() * unit_type::scalefactor_type::num)
                                     / unit_type::scalefactor_type::den);

                return quantity<norm_unit_type, repr_type>::promote(r_scale);
            }

            template <typename Quantity2>
            quantity operator+=(Quantity2 y) {
                static_assert(std::same_as<
                              typename unit_type::canon_type,
                              typename Quantity2::unit_type::canon_type >);

                /* relying on assignment that correctly converts-to-lhs-units */
                quantity y2 = y;

                this->scale_ += y2.scale();

                return *this;
            }

            template <typename Quantity2>
            quantity operator-=(Quantity2 y) {
                static_assert(std::same_as<
                              typename unit_type::canon_type,
                              typename Quantity2::unit_type::canon_type >);

                quantity y2 = y;

                /* relying on assignment that correctly converts-to-lhs-units */
                this->scale_ -= y2.scale();

                return *this;
            }

            template <typename Unit2, typename Repr2>
            repr_type in_units_of() const {
                // static_assert(dimension_of<Unit> == dimension_of<Unit2>);  // discard all the scaling values

                static_assert(same_dimension_v<Unit, Unit2>);

                using _convert_to_u2_type = unit_cartesian_product_t<Unit, unit_invert_t<Unit2>>;

                // _convert_u2_type
                //  - scalefactor_type
                //  - dim_type
                //  - canon_type

                /* if _convert_u2_type isn't dimensionless,  then {Unit2, Unit} have different dimensions */

                return ((this->scale_ * _convert_to_u2_type::scalefactor_type::num) / _convert_to_u2_type::scalefactor_type::den);
            }

            /* convert to quantity with same dimension, different {unit_type, repr_type} */
            template <typename Quantity2>
            operator Quantity2 () const {
                /* avoid truncating precision when converting:
                 * use best available representation
                 */
                using tmp_repr_type = std::common_type_t<repr_type, typename Quantity2::repr_type>;

                return Quantity2::promote(this->in_units_of<typename Quantity2::unit_type, tmp_repr_type>());
            }

            void display(std::ostream & os) const {
                os << this->scale() << unit_cstr();
            }

            quantity & operator=(quantity const & x) = default;
            quantity & operator=(quantity && x) = default;

        private:
            explicit constexpr quantity(Repr x) : scale_{x} {}

        private:
            /** @brief quantity represents this multiple of a unit (that has compile-time outer-scalefactor of 1) **/
            Repr scale_ = 0;
        }; /*quantity*/

        template <typename Quantity1, typename Quantity2>
        inline Quantity1 operator+ (Quantity1 x, Quantity2 y) {
            static_assert(same_dimension_v<typename Quantity1::unit_type, typename Quantity2::unit_type>);

            /* convert y to match units used by x;
             * would fail at compile time if this isn't well-defined
             */
            Quantity1 y2 = y;

            return Quantity1::promote(x.scale() + y2.scale());
        }

        template <typename Quantity1, typename Quantity2>
        inline Quantity1 operator- (Quantity1 x, Quantity2 y) {
            static_assert(std::same_as
                          < typename Quantity1::unit_type::dimension_type::canon_type,
                          typename Quantity2::unit_type::dimension_type::canon_type >);

            /* convert y to match units used by x */
            Quantity1 y2 = y;

            return Quantity1::promote(x.scale() - y2.scale());
        }

        template <typename Quantity>
        inline Quantity operator- (Quantity x) {
            return Quantity::promote(- x.scale());
        }

        template <typename Quantity1, typename Quantity2>
        inline auto operator* (Quantity1 x, Quantity2 y) {
            //static_assert(quantity_concept<Quantity1>);
            //static_assert(quantity_concept<Quantity2>);

            return x.multiply(y);
        }

        template <typename Unit, typename Repr>
        inline std::ostream &
        operator<< (std::ostream & os, quantity<Unit, Repr> const & x) {
            x.display(os);
            return os;
        }

        namespace qty {
            // ----- mass -----

            template <typename Repr = double>
            inline auto milligrams(Repr x) -> quantity<units::milligram, Repr> {
                return quantity<units::milligram, Repr>::promote(x);
            };

            template <typename Repr = double>
            inline auto grams(Repr x) -> quantity<units::gram, Repr> {
                return quantity<units::gram, Repr>::promote(x);
            };

            template <typename Repr = double>
            inline auto kilograms(Repr x) -> quantity<units::gram, Repr> {
                return quantity<units::kilogram, Repr>::promote(x);
            };

            // ----- distance -----

            template <typename Repr = double>
            inline auto millimeters(Repr x) -> quantity<units::millimeter, Repr> {
                return quantity<units::millimeter, Repr>::promote(x);
            }

            template <typename Repr = double>
            inline auto meters(Repr x) -> quantity<units::meter, Repr> {
                return quantity<units::meter, Repr>::promote(x);
            }

            template <typename Repr = double>
            inline auto kilometers(Repr x) -> quantity<units::kilometer, Repr> {
                return quantity<units::kilometer, Repr>::promote(x);
            }

            // ----- time -----

            template <typename Repr = double>
            inline auto nanoseconds(Repr x) -> quantity<units::nanosecond, Repr> {
                return quantity<units::nanosecond, Repr>::promote(x);
            }

            template <typename Repr = double>
            inline auto microseconds(Repr x) -> quantity<units::microsecond, Repr> {
                return quantity<units::microsecond, Repr>::promote(x);
            }

            template <typename Repr = double>
            inline auto milliseconds(Repr x) -> quantity<units::millisecond, Repr> {
                return quantity<units::millisecond, Repr>::promote(x);
            }

            template <typename Repr = double>
            inline auto seconds(Repr x) -> quantity<units::second, Repr> {
                return quantity<units::second, Repr>::promote(x);
            }

            template <typename Repr = double>
            inline auto minutes(Repr x) -> quantity<units::minute, Repr> {
                return quantity<units::minute, Repr>::promote(x);
            }

            template <typename Repr = double>
            inline auto hours(Repr x) -> quantity<units::hour, Repr> {
                return quantity<units::hour, Repr>::promote(x);
            }

            template <typename Repr = double>
            inline auto days(Repr x) -> quantity<units::day, Repr> {
                return quantity<units::day, Repr>::promote(x);
            }
        }
    } /*namespace obs*/
} /*namespace xo*/

/* end quantity.hpp */

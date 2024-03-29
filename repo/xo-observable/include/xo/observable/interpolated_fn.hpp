/* @file interpolated_fn.hpp */

#pragma once

#include "interpolated_fn_concept.hpp"
#include "linear_interpolation.hpp"
#include "function_concept.hpp"
#include <array> /* for std::size() */
#include <utility>

namespace xo {
    namespace obs {
        /** @class interpolated_fn
         *
         *  @brief represent a 1D continuous function f(x), obtained by interpolating between a set of points
         *
         *  Domain       dimension of argument values x
         *               (see obs::dimension<..>). Expect this to be dimension or observable
         *  Range        dimension of function values f(x)
         *               (see obs::dimension<..>). Expect this to be dimension or observable
         *  Interpolate  interpolation method
         *  XValues      array of domain values
         *  YValues      array of range values
         *
         * Requires:
         * - Domain::value_type
         * - Range::value_type
         * - Interpolate(Domain::elment_type,XValue,YValues)
         * - XValues::value_type == Domain::value_type
         * - YValues::value_type == Range::value_type
         **/
        template< typename Domain,
                  typename Range,
                  typename XValues,
                  typename YValues,
                  typename Interpolate = xo::obs::linear_interpolation<XValues, YValues> >
        class interpolated_fn {
        public:
            using domain_type = Domain;
            using range_type = Range;
            using domain_value_type = typename Domain::value_type;
            using range_value_type = typename Range::value_type;
            using interpolate_type = Interpolate;
            using xvalues_type = XValues;
            using yvalues_type = YValues;

            static_assert(interpolated_fn_concept<interpolated_fn>);

        public:
            interpolated_fn(Domain d, Range r, Interpolate interp, XValues x_v, YValues y_v)
                : domain_{std::move(d)},
                  range_{std::move(r)},
                  interpolator_{std::move(interp)},
                  x_v_{std::move(x_v)},
                  y_v_{std::move(y_v)} {}

            Domain const & domain() const { return domain_; }
            Range const & range() const { return range_; }

            /** @brief evaluate function at @p x,  by interpolating between points in @ref x_v_, @ref y_v_. **/
            range_value_type evaluate(domain_value_type const & x) const;
            range_value_type operator()(domain_value_type const & x) const { return this->evaluate(x); }

        private:
            /** @brief describes inputs x to this function.  can attach units and event identity **/
            Domain domain_;
            /** @brief describes values f(x) of this function.  can attach units and event identity **/
            Range range_;
            /** @brief interpolate between adjacent members of (@ref x_v_, @ref y_v_) values **/
            Interpolate interpolator_;
            /** @brief input values.
             *  Require @ref x_v_.size() == @ref y_v_.size()
             **/
            XValues x_v_;
            /** @brief output values **/
            YValues y_v_;
        }; /*interpolated_fn*/

        template<typename Domain,
                 typename Range,
                 typename Interpolate,
                 typename XValues,
                 typename YValues>
        auto interpolated_fn<Domain, Range, Interpolate, XValues, YValues>::evaluate(domain_value_type const & x) const -> range_value_type {
            auto y_lo_sentinel = y_v_[0];
            auto y_hi_sentinel = y_v_[std::size(y_v_) - 1];

            return interpolator_(x, x_v_, y_v_, y_lo_sentinel, y_hi_sentinel);
        } /*evaluate*/

    } /*namespace obs*/
} /*namespace xo*/


/* end interpolated_fn.hpp */

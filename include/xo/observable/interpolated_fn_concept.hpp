/* @file interpolated_fn_concept.hpp */

#pragma once

#include "function_concept.hpp"

namespace xo {
    namespace obs {
        template <typename Fn,
                  typename Domain = Fn::domain_type,
                  typename Range = Fn::range_type,
                  typename Interpolate = Fn::interpolate_type,
                  typename XValues = Fn::xvalues_type,
                  typename YValues = Fn::yvalues_type>
        concept interpolated_fn_concept = requires(Fn fn,
                                                   Domain domain,
                                                   Range range,
                                                   Interpolate interpolate,
                                                   XValues xv,
                                                   YValues yv)
        {
            typename Fn::domain_type;
            typename Fn::range_type;
            typename Fn::interpolate_type;
            typename XValues::value_type;
            typename YValues::value_type;

            requires std::same_as<typename Fn::domain_type::value_type,
                                  typename Fn::xvalues_type::value_type>;

            requires std::same_as<typename Fn::range_type::value_type, typename Fn::yvalues_types::value_type>;

            //typename Fn::value_type;
            //{ fn.mymethod(example2) } -> std::same_as<typename Fn::value_type>;
        }
            && (function_concept<Fn>
                && std::copyable<XValues>
                && std::copyable<YValues>)
            ;

    } /*namespace obs*/
} /*namespace xo*/


/* end interpolated_fn_concept.hpp */

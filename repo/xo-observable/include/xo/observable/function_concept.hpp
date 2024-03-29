/* @file function_concept.hpp */

#pragma once

#include <concepts>

namespace xo {
    namespace obs {
        template <typename Function>
        concept function_concept = requires(Function fn,
                                            typename Function::domain_value_type x)
        {
            typename Function::domain_type;
            typename Function::range_type;
            typename Function::domain_value_type;
            typename Function::range_value_type;
            //typename Domain::value_type;

            { fn.domain() } -> std::same_as<typename Function::domain_type>;
            { fn.range() } -> std::same_as<typename Function::range_type>;
            { fn.evaluate(x) } -> std::same_as<typename Function::range_value_type>;
            { fn(x) } -> std::same_as<typename Function::range_value_type>;
        }
        // && std::copyable<Domain>
            ;
    } /*namespace obs*/
} /*namespace xo*/


/* end function_concept.hpp */

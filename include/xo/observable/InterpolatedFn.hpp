/* @file InterpolatedFn.hpp */

#pragma once

#include "AbstractFunction.hpp"

namespace xo {
    namespace obs {
        /** @class InterpolatedFn

            @brief define a function by interpolating between values supplied for some finite set of points.

            Requires: domain is totally ordered.  For example can't be used on a cartesian product domain
         **/
        template <typename SomeT>
        class Interpolated1dFn : public AbstractFunction {
        private:
        };
    } /*namespace obs*/
} /*namespace xo*/

/* end InterpolatedFn.hpp */

/* @file AbstractFunction.hpp */

#pragma once

#include "xo/refcnt/Refcounted.hpp"

namespace xo {
    namespace obs {
        /** @class AbstractFunction

            @brief a function over outcomes for some observable

            Having reliable meaning for function domains
            (equivalence classes on observables)
            removes ambiguity when combining functions.

            This class provides entry point for runtime polymorphism.
         **/
        class AbstractFunction : public ref::Refcount {
        public:

        };
    } /*namespace obs*/
} /*namespace xo*/

/* end AbstractFunction.hpp */

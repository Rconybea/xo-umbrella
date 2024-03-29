/* @file AbstractSet.hpp */

#pragma once

#include "xo/refcnt/Refcounted.hpp"
#include <iostream>

namespace xo {
    namespace obs {
        /** @class AbstractSet

            @brief represent a set (in the set-theoretic sense)
        **/
        class AbstractSet : public ref::Refcount {
        public:
            using int64_t = std::int64_t;

        public:
            /** @brief true if a total ordering is defined for members of this set

                Total ordering:  relations =, <, such that:
                1. = is an equivalence relation
                2. (a < b) ==> not(b < a)
                3. (a < b) & (b < c) ==> (a < c)
             **/
            virtual bool is_totally_ordered() const = 0;
            /** @brief true if this set is (known to be) finite,  with known cardinality,  representable in a size_t **/
            virtual bool is_enumerable() const = 0;
            /** @brief true if this set is (known to be) finite **/
            virtual bool is_finite() const = 0;

            /** @brief compute cardinality for an enumerable set.  -1 for non-enumerable or non-finite set
                @pre is_enumerable() is true
             */
            virtual int64_t cardinality() const = 0;

            /** @brief write description to stream **/
            virtual void display(std::ostream & os) const = 0;
        };
    } /*namespace obs*/
} /*namespace xo*/

/* end AbstractSet.hpp */

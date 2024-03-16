/* @file Size2.cpp */

#include "Size2.hpp"
#include "xo/reflect/StructReflector.hpp"

namespace xo {
    using xo::reflect::StructReflector;

    namespace option {
        void Size2::reflect_self() {
            StructReflector<Size2> sr;

            if (sr.is_incomplete()) {
                REFLECT_MEMBER(sr, sz_v);
            }
        } /*reflect_self*/
    } /*namespace option*/
} /*namespace xo*/

/* end Size2.cpp */

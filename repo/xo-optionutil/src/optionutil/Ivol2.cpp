/* file Ivol2.cpp
 *
 * author: Roland Conybeare, Sep 2022
 */

#include "Ivol2.hpp"
#include "xo/reflect/StructReflector.hpp"

namespace xo {
    using xo::reflect::StructReflector;

    namespace option {
        void
        Ivol2::reflect_self()
        {
            StructReflector<Ivol2> sr;

            if (sr.is_incomplete()) {
                REFLECT_MEMBER(sr, ivol_v);
            }
        } /*reflect_self*/
    } /*namespace option*/
} /*namespace xo*/

/* end Ivol2.cpp */

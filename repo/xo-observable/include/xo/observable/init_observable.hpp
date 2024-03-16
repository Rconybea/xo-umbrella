/* @file init_observable.hpp
 *
 * author: Roland Conybeare, Mar 2024
 */

#pragma once

#include "xo/subsys/Subsystem.hpp"

namespace xo {
    enum S_observable_tag {};

    template<>
    struct InitSubsys<S_observable_tag> {
        static void init();
        static InitEvidence require();
    };
} /*namespace xo*/


/* end init_observable.hpp */

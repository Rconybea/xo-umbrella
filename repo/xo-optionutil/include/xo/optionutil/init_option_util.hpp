/* file init_option_util.hpp
 *
 * author: Roland Conybeare, Aug 2022
 */

#pragma once

#include "xo/subsys/Subsystem.hpp"

namespace xo {
    /* tag to represent the option_util/ subsystem within ordered initialization */
    enum S_option_util_tag {};

    template<>
    struct InitSubsys<S_option_util_tag> {
        static void init();
        static InitEvidence require();
    };
} /*namespace xo*/

/* end init_option_util.hpp */

/* @file init_observable.hpp
 *
 * author: Roland Conybeare, Mar 2024
 */

#pragma once

#include "xo/subsys/Subsystem.hpp"

namespace xo {
    enum S_observable_tag {};

    /** @class InitSubsys<S_observable_tag>
        @brief provide initialization plugin for the xo-observable library.
    **/
    template<>
    struct InitSubsys<S_observable_tag> {
        /** @brief performs run-once initialization steps specific to xo-observable **/
        static void init();
        /** @brief schedules (but does not immediately perform) initialization steps for xo-observable **/
        static InitEvidence require();
    };
} /*namespace xo*/


/* end init_observable.hpp */

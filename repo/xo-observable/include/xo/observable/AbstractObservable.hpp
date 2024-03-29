/* @file AbstractObservable.hpp */

#pragma once

#include "xo/refcnt/Refcounted.hpp"

namespace xo {
    namespace obs {
        /** @class AbstractObservable

            @brief represent an event that would have a specific outcome.

            For example:
            - "temperature in NY central park asof 16mar2024 5:46pm ET"
            - "price of MSFT stock at NYSE asof 15mar2024 3:59pm ET"

            Observables have "true names" that correspond to their identity.
            Two observables with the same true name represent the same event,
            and therefore share the same outcome.

            It's expected that observables often belong to a timeseries;
            e.g. "price of MSFT stock at NYSE as a function of time";
            with this in mind the identity of an observable includes the time
            at which such timeseries is being taken.
         **/
        class AbstractObservable : public ref::Refcount {
        public:
            /** @brief report the true name of this observable */
            virtual std::string const & true_name() const = 0;

            ref::rp<AbstractDimension> units() const = 0;

        private:
        };
    } /*namespace obs*/
} /*namespace xo*/


/* end AbstractObservable.hpp */

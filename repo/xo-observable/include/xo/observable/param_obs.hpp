/* @file param_obs.hpp */

#pragma once

#include <string>

namespace xo {
    namespace obs {
        /** @class params_obs
         *
         *  @brief represent a particular member of a family of events, usually identified by time
         *
         *  Note:
         *  1. common for a param_obs to represent a named event foo(t),
         *     in which case event becomes knowable (aka appears in filtration) at time t
         *  2. consider however an observable representing a stopping time;
         *     in that case event may becomes knowable at a different time on each path,
         *     (and may have probability > 0 that it never occurs).
         *
         * Require:
         * - ObservableSet :: observable-set-concept:
         *   - typename ObservableSet.index_set_type.element_type
         *   - ObservableSet.stem() :: string-like (printable, comparable)
         *   - ObservableSet.indexset() :: set-concept
         *   - ObservableSet.dimension() :: dimension-concept
         *
         * ObservableSet can be:
         * - naked pointer (if using arenas; caller responsible for keeping alive)
         * - smart pointer (e.g. target inherits ref::Refcount)
         **/
        template <typename ObservableSet>
        class param_obs {
        public:
            using observable_set_type = ObservableSet;
            using dimension_type = typename ObservableSet::dimension_type;
            using index_type = typename ObservableSet::index_set_type::element_type;

        public:
            param_obs(ObservableSet p) : parent_{std::move(p)} {}

            dimension_type const & dimension() const { return parent_->dimension(); }

        private:
            /** @brief observable belongs to this set of related events **/
            observable_set_type parent_;

            /** @brief index value identifying (along with @c parent.stem) this observable **/
            index_type index_;
        };
    } /*namespace obs*/

} /*namespace xo*/

/* end param_obs.hpp */

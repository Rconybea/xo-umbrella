/* @file ObservableSet.hpp */

#pragma once

#include "AbstractSet.hpp"

namespace xo {
    namespace obs {
        /** @class ObservableSet

            @brief Represents a  set of related observables,  parameterized by (the members of) an indexing set
        **/
        class ObservableSet : public AbstractSet {
        public:
            /** @brief get observable-set name **/
            std::string const & name();

            // ----- inherited from AbstractSet -----

            /** @brief true if observations (not indexing-set members) are known to be totally ordered.
                In general this isn't computable.
             **/
            virtual bool is_totally_ordered() const override { return false; }
            virtual bool is_enumerable() const override { return indexing_set_->is_enumerable(); }
            virtual bool is_finite() const override { return indexing_set_->is_finite(); }

        private:
            /** @brief true name for this set of observables.  Should be unique:
                two observable-sets are understood to represent the same set of events (up to indexing)
                if they have the same name.
             **/
            std::string name_;
            /** @brief indexing set mapping to individual observables in this observable-set **/
            ref::rp<AbstractSet> indexing_set_;
        }; /*ObservableSet*/
    } /*namespace obs*/

} /*namespace xo*/


/* end ObservableSet.hpp */

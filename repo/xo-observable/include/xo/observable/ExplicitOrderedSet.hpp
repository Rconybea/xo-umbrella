/* @file ExplicitOrderedSet.hpp */

#pragma once

#include "AbstractSet.hpp"
#include "xo/indentlog/machdep/machdep.hpp"   // for LSP wallpaper
#include <Eigen/Dense>

namespace xo {
    namespace obs {
        /** @class ExplicitOrderedSet
            @brief represents a finite set of ordered items,  stored in increasing order
        **/
        template <typename T>
        class ExplicitOrderedSet : public AbstractSet {
        public:
            using VectorXd = Eigen::VectorXd;

        public:
            /** @brief create instance

                @param univ.   universe in which this set is embedded.
                @param elt_v.  elements of this set,  in strictly increasing order
             **/
            static ref::rp<ExplicitOrderedSet> make(
#ifdef NOT_USING
                ref::rp<AbstractSet> univ,
#endif
                                                    std::vector<T> elt_v,
                                                    bool verify_flag = false) {

                if (verify_flag) {
                    for (std::size_t i = 0, n = elt_v.size(); i+1 < n; ++i) {
                        if (! (elt_v[i] < elt_v[i+1]))
                            throw std::runtime_error(tostr("ExplicitOrderedSet::make: expected v[i] < v[i+1]",
                                                           xtag("i", i), xtag("v[i]", elt_v[i]), xtag("v[i+1]", elt_v[i+1])));
                    }
                }

                return new ExplicitOrderedSet<T>(
#ifdef NOT_USING
                    std::move(univ),
#endif
                    std::move(elt_v));
            }

            // ----- inherited from AbstractSet -----
            virtual bool is_totally_ordered() const override { return true; }
            virtual bool is_enumerable() const override { return true; }
            virtual bool is_finite() const override { return true; }

            virtual int64_t cardinality() const override { return elt_v_.size(); }

            virtual void display(std::ostream & os) const override;

        private:
            ExplicitOrderedSet(
#ifdef NOT_USING
                ref::rp<AbstractSet> univ,
#endif
                std::vector<T> elt_v)
                :
#ifdef NOT_USING
                universe_{std::move(univ)},
#endif
                elt_v_{std::move(elt_v)} {}

        private:
#ifdef NOT_USING
            /** @brief this ordered set represents a subset of @ref universe_ **/
            ref::rp<AbstractSet> universe_;
#endif

            /** @brief contents of this set,  in strictly increasing order **/
            std::vector<T> elt_v_;  // TODO: use Eigen here
        }; /*ExplicitOrderedSet*/
    } /*namespace obs*/

} /*namespace xo*/


/* end ExplicitOrderedSet.hpp */

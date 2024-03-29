/* @file IntervalSet.hpp */

#pragma once

#include "AbstractSet.hpp"
#include <cstdint>

namespace xo {
    namespace obs {
        /** @class IntervalSet
            @brief represents an interval xxx
        **/
        template <typename T>
        class IntervalSet : public AbstractSet {
        public:
            static ref::rp<IntervalSet> make(ref::rp<AbstractSet> univ,
                                             T const & lo,
                                             T const & hi,
                                             bool lo_closed,
                                             bool hi_closed) {
                return new IntervalSet(univ, lo_closed, lo, hi, hi_closed);
            }

            // ----- inherited from AbstractSet -----
            virtual bool is_totally_ordered() const override { return true; }
            /** @brief true if interval is known to be enumerable;  in general this may not be computable **/
            virtual bool is_enumerable() const override { return universe_->is_enumerable(); }
            /** @brief true if interval is known to be enumerable;  in general this may not be computable **/
            virtual bool is_finite() const override { return universe_->is_finite(); }

            virtual int64_t cardinality() const override;

            virtual void display(std::ostream & os) const override;

        private:
            IntervalSet(ref::rp<AbstractSet> univ, bool lo_closed, T const & lo, T const & hi, bool hi_closed)
                : universe_{std::move(univ)}, lo_closed_{lo_closed}, lo_{lo}, hi_{hi}, hi_closed_{hi_closed}
                {}

        private:
            /** @brief this interval represents a subset of @ref universe_

                @invariant set_->is_totally_ordered()
             **/
            ref::rp<AbstractSet> universe_;

            /** @brief true iff lower endpoint is closed, i.e. if @ref lo_ belongs to this interval **/
            bool lo_closed_ = false;
            /** @brief lower endpoint of interval **/
            T lo_;
            /** @brief upper endpoint of interval **/
            T hi_;
            /** @brief truee iff upper endpoint is closed, i.e. if @ref hi_ belongs to this interval **/
            bool hi_closed_ = true;
        };

        template <typename T>
        inline int64_t
        IntervalSet<T>::cardinality() const { return -1; }

        namespace detail {
            template <typename T>
            inline T
            integer_cardinality(bool lo_closed, T lo, T hi, bool hi_closed) {
                /* example:
                 *   lo_closed lo  hi hi_closed  cardinality
                 *   ---------------------------------------
                 *           t  9   9 t          1
                 *           f  9   9 t          0
                 *           t  9   9 f          0
                 *           f  9   9 f          0
                 */
                if (lo > hi)
                    return 0;

                if (lo == hi)
                    return ((lo_closed == true && hi_closed == true) ? 1 : 0);

                /* lo_ < hi_ */
                auto x = (1 + hi - lo
                          + (lo_closed ? 0 : -1)
                          + (hi_closed ? 0 : -1));

                return x;
            }
        }

        template <>
        inline int64_t
        IntervalSet<std::int64_t>::cardinality() const {
            return detail::integer_cardinality(lo_closed_, lo_, hi_, hi_closed_);
        }

        template <>
        inline int64_t
        IntervalSet<std::uint64_t>::cardinality() const {
            return detail::integer_cardinality(lo_closed_, lo_, hi_, hi_closed_);
        }

        template <>
        inline int64_t
        IntervalSet<std::int32_t>::cardinality() const {
            return detail::integer_cardinality(lo_closed_, lo_, hi_, hi_closed_);
        }

        template <>
        inline int64_t
        IntervalSet<std::uint32_t>::cardinality() const {
            return detail::integer_cardinality(lo_closed_, lo_, hi_, hi_closed_);
        }

        template <>
        inline int64_t
        IntervalSet<std::int16_t>::cardinality() const {
            return detail::integer_cardinality(lo_closed_, lo_, hi_, hi_closed_);
        }

        template <>
        inline int64_t
        IntervalSet<std::uint16_t>::cardinality() const {
            return detail::integer_cardinality(lo_closed_, lo_, hi_, hi_closed_);
        }

        template <>
        inline int64_t
        IntervalSet<std::int8_t>::cardinality() const {
            return detail::integer_cardinality(lo_closed_, lo_, hi_, hi_closed_);
        }

        template <>
        inline int64_t
        IntervalSet<std::uint8_t>::cardinality() const {
            return detail::integer_cardinality(lo_closed_, lo_, hi_, hi_closed_);
        }

        template<typename T>
        void
        IntervalSet<T>::display(std::ostream & os) const {
            /* e.g. <Interval [0.0, 1.0) > */

            os << "<Interval "
               << (lo_closed_ ? "[" : "(")
               << lo_
               << ", " << hi_
               << (hi_closed_ ? "]" : ")")
               << " >";
        }

        using IntegerInterval = IntervalSet<std::int64_t>;
        using RealInterval = IntervalSet<double>;
    } /*namespace obs*/
} /*namespace xo*/


/* end IntervalSet.hpp */

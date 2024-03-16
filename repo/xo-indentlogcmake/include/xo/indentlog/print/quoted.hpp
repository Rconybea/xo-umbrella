/* file quoted.hpp
 *
 * author: Roland Conybeare, Sep 2022
 */

#pragma once

#include "tostr.hpp"
#include <sstream>
#include <string_view>
#include <utility>
#include <type_traits>

namespace xo {
    namespace print {
        /* use this to avoid template conversion hassles
         * since literal strings get treated as arrays
         */
        template<typename T>
        char const * ccs(T x) { return x; }

        /* Printing cases:
         * 1. T&&:
         *    move into quoted_impl<T>.  T must be moveable!
         * 2. T&:
         *    copy reference into quoted_impl<T&>.
         *    similarly for T const &, copy reference into quoted_impl<T const &>
         */

        template<typename T>
        class quoted_impl {
        public:
            quoted_impl(bool unq_flag, T x) : unq_flag_{unq_flag}, value_{std::move(x)} {}

            bool unq_flag() const { return unq_flag_; }
            T const & value() const { return value_; }

            void print(std::ostream & os) const {
                std::string xs = xo::tostr(value_);

                if (xs.empty()) {
                    /* always print empty string as "" */
                    os << "\"\"";
                } else if ((xs.at(0) == '<') && (xs.at(xs.size() - 1) == '>')) {
                    /* assume string represents output of a well-formed object printer,
                     * and already self-escapes
                     */
                    os << xs;
                } else if (xs.find_first_of(" \"\n\r\\") == std::string::npos) {
                    /* no escapes needed,  just print xs */
                    if (unq_flag_)
                        os << xs;
                    else
                        os << "\"" << xs << "\"";
                } else {
                    /* printed value contains a space
                     * and/or a must-be-escaped character.
                     * in any case, need quotes
                     */

                    os << "\"";

                    /* print contents of ss, with escapes:
                     *   \       => \\
                     *   "       => \"
                     *   newline => \n
                     *   cr      => \r
                     */
                    for (char ch : xs) {
                        switch (ch) {
                        case '"':
                            /* " => \" */
                            os << "\\\"";
                            break;
                        case '\n':
                            /* newline -> \n */
                            os << "\\\n";
                            break;
                        case '\r':
                            /* cr -> \r */
                            os << "\\\r";
                            break;
                        case '\\':
                            /* \ => \\  (mind c++ requires we escape \) */
                            os << "\\\\";
                            break;
                        default:
                            os << ch;
                            break;
                        }
                    }

                    os << "\"";
                }
            } /*print*/

        private:
            /* .unq_flag:  if true,  omit surrounding " chars
             *             if printed value satisfies both:
             *             - no escaped chars
             *             - no spaces
             */
            bool unq_flag_ = false;
            /* .value:  value to be printed */
            T value_;
        }; /*quoted_impl*/

        template<typename T>
        std::ostream &
        operator<<(std::ostream & os, quoted_impl<T> const & x) {
            x.print(os);
            return os;
        } /*operator*/

        /* writing out std::forward<T> behavior for completeness' sake:
         *
         * 1. call quoted(x) with rvalue std::string x, then:
         *    - T will be deduced to [std::string]
         *        (in particular: _not_ std::string &, std::string const &, std::string &&)
         *    - rvalue std::string passed to quoted_impl ctor
         *
         * 2a. call quoted(x) with std::string & x, then:
         *    - T deduced to [std::string &]
         *    - std::string & passed to quoted_impl ctor
         *
         * 2b. call quoted(x) with std::string const & x, then:
         *    - T deduced to [std::string const &]
         *    - std::string const & passed to quoted_impl ctor
         */
        template<typename T>
        auto quoted(T && x) {
            return quoted_impl(false /*unq_flag*/, std::forward<T>(x));
        }

        inline auto qcstr(char const * x) {
            return quoted(x);
        } /*qcstr*/

        template<typename T>
        auto unq(T && x) {
            return quoted_impl(true /*unq_flag*/, std::forward<T>(x));
        }
    } /*namespace print*/
} /*namespace xo*/

/* end quoted.hpp */

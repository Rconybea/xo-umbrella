/* @file ratio_util.hpp */

#pragma once

#include "ratio_concept.hpp"
#include "stringliteral.hpp"
#include <ratio>
#include <numeric>

namespace xo {
    namespace obs {
        // ----- ratio_power -----

        /** ratio to an integer power
         *
         *    ratio_power<Base,Power>::type
         *
         *  yields a std::ratio representing Base^Power.
         **/
        template <typename Base, int Power, bool IsNegative = std::signbit(Power)>
        struct ratio_power_aux;

        template <typename Base, int Power>
        struct ratio_power_aux<Base, Power, true> {

            /** std::ratio representing Base^(-Power) **/
            using _inverse_ratio_type = typename ratio_power_aux<Base, -Power>::type;

            /** Base^(-Power)^-1 = Base^Power **/
            using type = std::ratio_divide<std::ratio<1>, _inverse_ratio_type>;

            static_assert(ratio_concept<type>);
        };

        template <typename Base>
        struct ratio_power_aux<Base, 0, false> {
            using type = std::ratio<1>;
        };

        template <typename Base>
        struct ratio_power_aux<Base, 1, false> {
            using type = Base;
        };

        template <typename Base, int Power>
        struct ratio_power_aux<Base, Power, false> {
            /** Base^(Power/2) **/
            using _p2_ratio_type = ratio_power_aux<Base, Power/2>::type;
            /** Base^(Power%2) :
             *  - Base^0 if Power is even
             *  - Base^1 if Power is odd
             **/
            using _x_ratio_type = ratio_power_aux<Base, Power%2>::type;

            using type = std::ratio_multiply< _x_ratio_type,
                                              std::ratio_multiply<_p2_ratio_type,
                                                                  _p2_ratio_type> >;

            static_assert(ratio_concept<type>);
        };

        // ----- ratio_power_v -----

        /** compute Base^Power at compile time **/
        template <typename Base, int Power>
        using ratio_power_t = ratio_power_aux<Base, Power>::type;

        // ----- from_ratio -----

        /** Example:
         *    from_ratio<double, std::ratio<1,10> --> 0.1
         *    from_ratio<double, std::ratio<4,5>  --> 0.8
         **/
        template <typename Repr, typename Ratio>
        constexpr Repr from_ratio() {
            static_assert(ratio_concept<Ratio>);

            return Ratio::num / static_cast<Repr>(Ratio::den);
        }

        // ----- ratio2str_aux -----

        /* note: using struct wrapper because partial specialization of function templates
         *       is not allowed
         */
        template <int Num,
                  int Den,
                  bool Signbit = std::signbit(Num) ^ std::signbit(Den)>
        struct ratio2str_aux;

        template <int Num,
                  int Den>
        struct ratio2str_aux<Num, Den, false> {
            static constexpr auto value = stringliteral_concat("(",
                                                               stringliteral_from_int_v<Num>().value_,
                                                               "/",
                                                               stringliteral_from_int_v<Den>().value_,
                                                               ")");
        };

        template <int Num,
                  int Den>
        struct ratio2str_aux<Num, Den, true> {
            /* note: using struct wrapper because partial specialization of function templates
             *       is not allowed
             */
            static constexpr auto value = stringliteral_concat("-(",
                                                               stringliteral_from_int_v<-Num>().value_,
                                                               "/",
                                                               stringliteral_from_int_v<Den>().value_,
                                                               ")");
        };

        template <int Num>
        struct ratio2str_aux<Num, /*Den*/ 1, true> {
            static constexpr auto value = stringliteral_concat("-",
                                                               stringliteral_from_int_v<-Num>().value_);
        };

        template <int Num>
        struct ratio2str_aux<Num, /*Den*/ 1, false> {
            static constexpr auto value = stringliteral_from_int_v<Num>();
        };

        template <>
        struct ratio2str_aux<1, 1, false> {
            static constexpr auto value = stringliteral("");
        };

        // ----- stringliteral_from_ratio -----

        /** Example:
         *  - ratio2str<std::ratio<2,3> -> "^(2,3)"
         **/
        template <typename Ratio>
        constexpr auto stringliteral_from_ratio() {
            static_assert(ratio_concept<Ratio>);

            return ratio2str_aux<Ratio::type::num, Ratio::type::den>().value;
        };

        // ----- exponent2str_aux -----

        /* note: using struct wrapper because partial specialization of function templates
         *       is not allowed
         */
        template <int Num,
                  int Den,
                  bool Signbit = std::signbit(Num) ^ std::signbit(Den)>
        struct exponent2str_aux;

        template <int Num,
                  int Den>
        struct exponent2str_aux<Num, Den, false> {
            static constexpr auto value = stringliteral_concat("^(",
                                                               stringliteral_from_int_v<Num>().value_,
                                                               "/",
                                                               stringliteral_from_int_v<Den>().value_,
                                                               ")");
        };

        template <int Num,
                  int Den>
        struct exponent2str_aux<Num, Den, true> {
            /* note: using struct wrapper because partial specialization of function templates
             *       is not allowed
             */
            static constexpr auto value = stringliteral_concat("^-(",
                                                               stringliteral_from_int_v<-Num>().value_,
                                                               "/",
                                                               stringliteral_from_int_v<Den>().value_,
                                                               ")");
        };

        template <int Num>
        struct exponent2str_aux<Num, 1, false> {
            static constexpr auto value = stringliteral_concat("^",
                                                               stringliteral_from_int_v<Num>().value_);
        };

        template <int Num>
        struct exponent2str_aux<Num, 1, true> {
            /* Example:
             * - exponent2str_aux<-1, 1, true> --> "^-1"
             * - exponent2str_aux<-2, 1, true> --> "^-2"
             */
            static constexpr auto value = stringliteral_concat("^",
                                                               stringliteral_from_int_v<Num>().value_);
        };

        template <>
        struct exponent2str_aux<1, 1, false> {
            static constexpr auto value = stringliteral("");
        };

        // ----- stringliteral_from_exponent -----

        template <typename Ratio>
        constexpr auto stringliteral_from_exponent() {
            static_assert(ratio_concept<Ratio>);

            return exponent2str_aux<Ratio::type::num, Ratio::type::den>().value;
        };

    } /*namespace obs*/
} /*namespace xo*/

/* end ratio_util.hpp */

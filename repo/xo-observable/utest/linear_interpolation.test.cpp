/* @file linear_interpolation.test.cpp */

#include "xo/observable/linear_interpolation.hpp"
#include "xo/randomgen/random_seed.hpp"
#include "xo/randomgen/uniformgen.hpp"
#include "xo/randomgen/xoshiro256.hpp"
#include "xo/indentlog/print/tag.hpp"
#include "xo/indentlog/print/array.hpp"
#include "xo/indentlog/print/vector.hpp"
#include <catch2/catch.hpp>
#include <iostream>
#include <set>
#include <type_traits>
#include <vector>


namespace xo {
    using xo::obs::proportional_glb_index;
    using xo::rng::uniformgen;
    using xo::rng::xoshiro256ss;

    using namespace std;

    namespace ut {
        namespace {
/* note: trivial REQUIRE() call in else branch bc we still want
 *       catch2 to count assertions when verification succeeds
 */
#  define REQUIRE_ORCAPTURE(ok_flag, catch_flag, expr) \
    if (catch_flag) {                                  \
        REQUIRE((expr));                               \
    } else {                                           \
        REQUIRE(true);                                 \
        ok_flag &= (expr);                             \
    }

#  define REQUIRE_ORFAIL(ok_flag, catch_flag, expr)    \
    REQUIRE_ORCAPTURE(ok_flag, catch_flag, expr);      \
    if (!ok_flag)                                      \
        return ok_flag

            template <typename Ugen>
            static bool
            random_searches(std::uint32_t n,
                            bool catch_flag,
                            Ugen * p_ugen)
            {
                bool ok_flag = true;

                /* 1. generate a set of random values */
                std::set<double> set;

                for (size_t i = 0; i < n; ++i) {
                    double x = (*p_ugen)();

                    set.insert(x);
                }

                /* 2. copy to vector */
                std::vector<double> v(set.size());

                std::copy(set.begin(), set.end(), v.begin());

                INFO(tostr(xtag("v", v)));

                /* 3. generate random test probes.  use slightly larger range so we
                 *    reach edge cases
                 */

                for (size_t i = 0; i < std::max(10u, 2*n); ++i) {
                    double x = 1.01 * (*p_ugen)();

                    size_t glb_ix = proportional_glb_index(x, v,
                                                           true /*is_closed*/,
                                                           catch_flag);

                    /* also take lower bound from set.  This is a bit awkward,
                     * std library has unintuitive names (according to this author)
                     */

                    /* gives iterator at position _at or after_ x */
                    auto ref_ix = set.upper_bound(x);

                    INFO(tostr(xtag("x", x), xtag("i", i), xtag("glb_ix", glb_ix)));

                    if (ref_ix == set.begin()) {
                        INFO("no upper bound according to std::set");

                        /* there is no lower bound for x in v[] */
                        REQUIRE_ORCAPTURE(ok_flag, catch_flag, glb_ix == -1);
                        //REQUIRE(glb_ix == -1);
                    } else {
                        INFO("has upper bound according to std::set");

                        REQUIRE_ORCAPTURE(ok_flag, catch_flag, glb_ix >= 0);
                        //REQUIRE(glb_ix >= 0);

                        --ref_ix;

                        REQUIRE_ORCAPTURE(ok_flag, catch_flag, v[glb_ix] == *ref_ix);
                        //REQUIRE(v[glb_ix] == *ref_ix);
                    }
                }

                return ok_flag;
            } /*random_searches*/

            TEST_CASE("proportional_glb_index", "[linear_interpolation]") {
                /* uncomment to debug if unit test fails*/
                constexpr bool c_debug_flag = false;

                // 1. generate random sorted arrays of various sizes.
                // 2. do logarithmically spaced points,  for pathology

                uint64_t seed = 8522122016795061163UL;
                // can get bits from /dev/random by uncommenting the next line instead
                //rng::Seed<xoshiro256ss> seed;

                auto rgen = xo::rng::xoshiro256ss(seed);

                // uncomment next 2 lines to print seed
                //xo::scope log(XO_DEBUG2(true, "TEST_CASE.proportional_glb_index"));
                //log && log(xtag("seed", seed));

                /* choose an array size */
                for (size_t n = 0; n <= 1024; n = max(2*n, 1ul)) {
                    bool ok_flag = false;

                    /* attention=0 -- check for errors; do not invoke catch macros; logging disabled
                     * attention=1 -- invoke catch macros; logging enabled
                     *
                     * using same rng for each iteration for deterministic behavior
                     */

                    auto rgen2 = rgen;

                    for (std::uint32_t attention = 0; !ok_flag && (attention < 2); ++attention) {
                        ok_flag = true;

                        bool debug_flag = (attention == 1);

                        auto ugen = uniformgen<xoshiro256ss>::interval(rgen2, -1.0, +1.0);

                        ok_flag &= random_searches(n, debug_flag, &ugen);
                    }

                    /* don't want each n iteration to start with the same seed */
                    rgen = rgen2;
                }
            } /*TEST_CASE(proportional_glb_index)*/
        }
    }
} /*namespace xo*/


/* end linear_interpolation.test.cpp */
